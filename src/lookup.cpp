#include "node.h"
#include "util/proto_messages.h"
#include <cmath>

using namespace std;
using namespace p2pfilesharing;
contactInfo_t Node::get_next_hop_info(digest_t hash) {
  if (hash < my_hash) {
    hash +=  pow(2, my_config::finger_table_length);
  }
  if (hash == my_hash) {
    cerr << "I am responsible for this file, should not use get_next_hop_info\n";
    return make_pair(my_hostname, my_config::listening_port_num);
  }
  int index = (int)log2(hash - my_hash);
  return fingerTable[index].first;
}

bool Node::is_responsible_to(digest_t file_hash) {
  if (file_hash > my_hash) {
    file_hash -= pow(2, my_config::finger_table_length);
  }
  if (local_start > my_hash) {
    local_start -= pow(2, my_config::finger_table_length);
  }
  if (file_hash >= local_start && file_hash <= my_hash) {
    return true;
  } else {
    return false;
  }
}

void Node::lookup_req_handle(const LookupFileRequest& res) {
  digest_t filenamehash = res.filenamehash();
  if (is_responsible_to(filenamehash)) {
    string host_name = res.sourcehostname();
    string port = res.sourceport();
    ProtoStreamOut proto_stream_out(host_name, port);
    proto_out* out = proto_stream_out.get_proto_out();
    // check owner's hostname & ip in DHS
    bool does_exist;
    string owner_host_name, owner_port, successor_host_name, successor_port;
    successor_host_name = my_hostname;
    successor_port = my_config::listening_port_num;
    if (DHT.find(filenamehash) ==DHT.end()) {
      does_exist = false;
      owner_host_name = "0.0.0.0";
      owner_port = "0";
    } else {
      does_exist = true;
      owner_host_name = DHT[filenamehash].first;
      owner_port = DHT[filenamehash].second;
    }
    NodeResponse rsp = generate_lookup_response(does_exist, successor_host_name, successor_port, owner_host_name, owner_port);
    sendMesgTo<NodeResponse>(rsp, out);    
  } else{
    // check finger table and forward
    contactInfo_t info = get_next_hop_info(filenamehash);
    NodeRequest req = generate_lookup_request(res);
    ProtoStreamOut proto_stream_out(info.first, info.second);
    proto_out* out = proto_stream_out.get_proto_out();
    sendMesgTo<NodeRequest>(req, out);
  }
}

pair<bool, contactInfo_t> Node::lookup_successor(digest_t hash, const string& port) {
  contactInfo_t successor, owner;
  bool does_exist;
  lookup(hash, port, &does_exist, &successor, &owner);
  return make_pair(does_exist, successor);
}

void Node::lookup(digest_t hash, const string & port, bool * does_exist, contactInfo_t * successor, contactInfo_t * owner) {
  if (is_responsible_to(hash)) {
    successor->first = my_hostname;
    successor->second = my_config::listening_port_num; 
    if (DHT.find(hash) ==DHT.end()) {
      *does_exist = false;
      owner->first = "0.0.0.0";
      owner->second = "0";
    } else {
      *does_exist = true;
      owner->first = DHT[hash].first;
      owner->second = DHT[hash].second; 
    }
    return;
  } else {
    // send out NodeRequest
    contactInfo_t info = get_next_hop_info(hash);
    NodeRequest req = generate_lookup_request(hash, my_hostname, port);
    ProtoStreamOut proto_stream_out(info.first, info.second);
    proto_out * out = proto_stream_out.get_proto_out();
    sendMesgTo<NodeRequest> (req, out);
    proto_stream_out.close_proto_out();    
    
    // wait for NodeResponse from any possible node
    ProtoStreamIn proto_stream_in(port);    
    proto_in* in = proto_stream_in.get_proto_in();
    NodeResponse rsp;
    recvMesgFrom<NodeResponse>(rsp, in);
    
    // parse
    int64_t type = rsp.type();
    
    // Debugging info
    if (type != 2) {
      cerr << "Expecting a LookupFileResponse, but receive a packet type = " << type << endl;
      return;
    }
    const LookupFileResponse& lf_rsp = rsp.lookup();
    *does_exist = lf_rsp.ifexists();
    successor->first = lf_rsp.successorhostname();
    successor->second = lf_rsp.successorport();
    owner->first = lf_rsp.ownerhostname();
    owner->second = lf_rsp.ownerport();
  }
}


