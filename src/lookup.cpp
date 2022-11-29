#include "node.h"
#include "util/proto_messages.h"

using namespace std;
using namespace p2pfilesharing;
contactInfo_t Node::get_next_hop_info(digest_t hash) {
  //TODO
  return make_pair("", "");
}

bool Node::is_responsible_to(digest_t file_hash) {
  // TODO
  return true;
}

void Node::lookup_req_handle(LookupFileRequest res) {
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


