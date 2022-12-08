#include <cmath>
#include <fstream>

#include "node.h"
#include "util/proto_messages.h"

/*
    Used for the first node to initialized the chord circle.
    It initialzies the finger table and leave DHT empty.
    Sharing files need to be added manually through user interface later.
*/
void Node::initial_chord() {
  // set finger table
  contactInfo_t t(my_hostname, my_config::listening_port_num);
  for (int i = 0; i < my_config::finger_table_length; i++) {
    fingerTable[i].first = t;
    fingerTable[i].second = my_hash;
  }

  // set localstart
  local_start = hash_value_add(my_hash, 1);
  cout << my_hostname << " successfully initial chord circle.\n";
}

/*
  Handle joinRequest. Find the responsible node for the new node and
  send HelpJoinRequest to the responsible node.
*/
void Node::join_req_handle(const JoinRequest& join_req) {
  int random_port = get_random_port();
  string r_port = to_string(random_port);

  // find responsible node
  pair<bool, contactInfo_t> res =
      lookup_successor(join_req.newhostnamehash(), r_port);
  cout << "find responsible node: ";
  print_contactInfo(res.second);

  // send HelpJoinRequest to responsible node
  string respon_hostName = res.second.first;
  string respon_port = res.second.second;
  NodeRequest help_join_req = generate_help_join_request(
      join_req.newhostnamehash(), join_req.newhostname(), join_req.newport());
  ProtoStreamOut proto_out_respon(respon_hostName, respon_port);
  proto_out* out_respon = proto_out_respon.get_proto_out();
  sendMesgTo<NodeRequest>(help_join_req, out_respon);
}

/*
  Handle helpjoinRequest. Send HelpJoinResponse to the new node to show the
  identity and send RouteTableInit msg, which contains all the route entries, to
  the new node.
*/
void Node::help_join_req_handle(const HelpJoinRequset& help_join_req) {
  ProtoStreamOut proto_out_new(help_join_req.newhostname(),
                               help_join_req.newport());
  proto_out* out_new = proto_out_new.get_proto_out();

  // send HelpJoinResponse
  cout << my_hostname << " sends HelpJoinResponse to "<<help_join_req.newhostname() << ": " << help_join_req.newport() <<endl;
  NodeResponse help_join_resp = generate_help_join_response(
      my_hash, my_hostname, my_config::listening_port_num);
  sendMesgTo<NodeResponse>(help_join_resp, out_new);

  // generate Route table for new node and send it
  cout << my_hostname << " generates Route table to "<<help_join_req.newhostname()<<endl;
  RouteTableInit* finger_table = new RouteTableInit();
  for (int i = 0; i < my_config::finger_table_length; i++) {
    string r_port = to_string(get_random_port());
    int64_t hash = hash_value_add(help_join_req.newhostnamehash(), pow(2, i));  // n+2^i

    // get successor for given hash
    pair<bool, contactInfo_t> res = lookup_successor(hash, r_port);
    cout<< "entry: " << i <<" hash_value: " << hash; 
    print_contactInfo(res.second);

    RouteTableEntry* entry = finger_table->add_entry();
    entry->set_hostname(res.second.first);
    entry->set_port(res.second.second);
    entry->set_hostnamehash(get_hash(res.second.first));
  }
  NodeResponse route_init_resp = generate_routetable_init(finger_table);
  sendMesgTo<NodeResponse>(route_init_resp, out_new);
  cout << my_hostname << " sends Route table to "<<help_join_req.newhostname()<<endl;

  // locate the furthest prev node for the new node
  string r_port = to_string(get_random_port());
  digest_t target_hash =
      hash_value_add(help_join_req.newhostnamehash(),
                     -1 * pow(2, my_config::finger_table_length - 1));
  pair<bool, contactInfo_t> res = lookup_successor(target_hash, r_port);
  cout << "find furthest prev node: ";
  print_contactInfo(res.second);
  NodeResponse lookup_node_resp = generate_lookup_node_response(
      res.first, res.second.first, res.second.second);
  sendMesgTo<NodeResponse>(lookup_node_resp, out_new);

  // generate file table for new node and send it
  FileTable* file_table = new FileTable();
  int64_t d_new_to_cur = get_distance(help_join_req.newhostnamehash(), my_hash);
  for (auto it = DHT.begin(); it != DHT.end();) {
    int64_t d_file_to_cur = get_distance(it->first, my_hash);
    if (d_file_to_cur > d_new_to_cur) {
      FileTableEntry* entry = file_table->add_entry();
      entry->set_filenamehash(it->first);
      entry->set_ownerhostname(it->second.first);
      entry->set_ownerport(it->second.second);
      it = DHT.erase(it);
    } else {
      ++it;
    }
  }
  NodeResponse file_table_resp = generate_filetable_response(file_table);
  sendMesgTo<NodeResponse>(file_table_resp, out_new);
  cout << my_hostname << " sends file table to "<<help_join_req.newhostname()<<endl;

  // update localstart because new_node is a new predecessor
  local_start = hash_value_add(help_join_req.newhostnamehash(), 1);
  cout << my_hostname << " update local start to "<< local_start << endl;
}

/*
  Update current node's route table to include the newly join node, and
  send back RouteUpdateResponse, which contains the info of next node that
  should update, to the new node.
*/
void Node::handle_update_route_req(const RouteUpdateRequest& update_route_req) {
  // update route table
  int d_cur_to_new = get_distance(my_hash, update_route_req.newhostnamehash());
  for (int i = 0; i < my_config::finger_table_length; i++) {
    int d_min = pow(2, i);
    int d_cur_to_entry = get_distance(my_hash, fingerTable[i].second);
    cout << "i: "<< i;
    cout<<" d_min: "<< d_min << " d_cur_to_entry: "<<d_cur_to_entry<<endl;
    if (d_cur_to_new >= d_min && d_cur_to_new < d_cur_to_entry) {
      fingerTable[i].first.first = update_route_req.newhostname();
      fingerTable[i].first.second = update_route_req.newport();
      fingerTable[i].second = update_route_req.newhostnamehash();
    }
  }

  // send back RouteUpdateResponse
  NodeResponse rsp = generate_route_update_response(
      my_hash, fingerTable[0].second, fingerTable[0].first.first,
      fingerTable[0].first.second);  // next node info
  ProtoStreamOut proto_out_new(update_route_req.newhostname(),
                               update_route_req.newport());
  proto_out* out_new = proto_out_new.get_proto_out();
  sendMesgTo<NodeResponse>(rsp, out_new);
}

/*
    Join the existed chord circle and initialize finger table and DHT.
    Also, it notifies other node in chord circle to update their finger
    table for this new node.
*/
void Node::join_chord() {
  cout << my_hostname << " begins join chord circle.\n";
  // one-time port number for this operation
  string r_port_1 = to_string(get_random_port());

  // send JoinRequest to entry node
  cout << my_hostname << " sends JoinRequest.\n";
  NodeRequest req = generate_join_request(my_hash, my_hostname, r_port_1);
  ProtoStreamOut proto_out_entry(entryNode.first, entryNode.second);
  proto_out* out_entry = proto_out_entry.get_proto_out();
  sendMesgTo<NodeRequest>(req, out_entry);

  // wait for HelpJoinResponse from responsible node
  cout << my_hostname << " waits for HelpJoinResponse on port " << r_port_1 << endl;
  ProtoStreamIn proto_in_respon(r_port_1);
  proto_in* in_respon = proto_in_respon.get_proto_in();
  NodeResponse rsp;
  recvMesgFrom<NodeResponse>(rsp, in_respon);

  // parse, make sure get response from responsible node
  int64_t type = rsp.type();
  if (type != 6) {
    cerr << "Expecting a HelpJoinResponse, but receive a packet type = " << type
         << endl;
    cerr << "Fail to Join chord circle\n";
    exit(EXIT_FAILURE);
  }

  cout << my_hostname << " gets contacted by responsible node "
       << rsp.helpjoin().responsiblehostname() << endl;

  // get route table entries and set finger table
  recvMesgFrom<NodeResponse>(rsp, in_respon);
  type = rsp.type();
  if (type != 4) {
    cerr << "Expecting a RouteTableInit response, but receive a packet type = "
         << type << endl;
    cerr << "Fail to Join chord circle\n";
    exit(EXIT_FAILURE);
  }
  const RouteTableInit& rti_rsp = rsp.routeinit();
  assert(rti_rsp.entry_size() == my_config::finger_table_length);
  for (int i = 0; i < rti_rsp.entry_size(); i++) {
    RouteTableEntry entry = rti_rsp.entry(i);
    contactInfo_t t(entry.hostname(), entry.port());
    fingerTable[i].first = t;
    fingerTable[i].second = entry.hostnamehash();
  }

  cout << my_hostname << " successfully initializes finger table\n";

  // update previous node's route table,
  // start from the furthest node (n-2^(log2N -1))
  recvMesgFrom<NodeResponse>(rsp, in_respon);
  type = rsp.type();
  if (type != 8) {
    cerr << "Expecting a LookupNodeResponse response, but receive a packet "
            "type = "
         << type << endl;
    cerr << "Fail to Join chord circle\n";
    exit(EXIT_FAILURE);
  }
  const LookupNodeResponse& lun_rsp = rsp.lookupnode();
  string update_node_hostname = lun_rsp.hostname();
  string update_node_listen_port = lun_rsp.port();
  cout << my_hostname << " get furthest previous nodes: ";
  cout << update_node_hostname << " port: "<< update_node_listen_port << endl;
  while (1) {
    string r_port = to_string(get_random_port());
    // send RouteUpdateRequest
    NodeRequest req = generate_route_update_request(my_hash, my_hostname, r_port);
    ProtoStreamOut proto_out_prev(update_node_hostname,
                                  update_node_listen_port);
    proto_out* out_prev = proto_out_prev.get_proto_out();
    sendMesgTo<NodeRequest>(req, out_prev);
    
    // wait for RouteUpdateResponse
    ProtoStreamIn proto_in_prev(r_port);
    proto_in* in_prev = proto_in_prev.get_proto_in();
    NodeResponse rsp;
    recvMesgFrom<NodeResponse>(rsp, in_prev);
    int64_t type = rsp.type();
    if (type != 7) {
      cerr << "Expecting a HelpJoinResponse, but receive a packet type = "
           << type << endl;
      cerr << "Fail to Join chord circle\n";
      exit(EXIT_FAILURE);
    }

    // if next updated node is myself, break
    const RouteUpdateResponse& ru_rsp = rsp.updateroute();
    if (ru_rsp.nextupdatehostnamehash() == my_hash) {
      local_start = hash_value_add(ru_rsp.curupdatehostnamehash(), 1);
      break;
    } else {
      update_node_hostname = ru_rsp.nextupdatehostname();
      update_node_listen_port = ru_rsp.nextupdateport();
    }
  }
  cout << my_hostname << " successfully updates previous nodes.\n";

  // get responsible files from responsible node
  recvMesgFrom<NodeResponse>(rsp, in_respon);
  type = rsp.type();
  if (type != 5) {
    cerr << "Expecting a FileTable response, but receive a packet type = "
         << type << endl;
    cerr << "Fail to Join chord circle\n";
    exit(EXIT_FAILURE);
  }
  const FileTable& ft_rsp = rsp.filetable();
  for (int i = 0; i < ft_rsp.entry_size(); i++) {
    FileTableEntry entry = ft_rsp.entry(i);
    contactInfo_t t(entry.ownerhostname(), entry.ownerport());
    DHT.insert({entry.filenamehash(), t});
  }
  cout << my_hostname << " successfully gets responsible files.\n";

  cout << my_hostname << " successfully joins chord circle.\n";
}

// TODO: test
// TODO: refactor join_chord(). It's too long
// TODO: Using exception for exit failure
// TODO: Add block state.
