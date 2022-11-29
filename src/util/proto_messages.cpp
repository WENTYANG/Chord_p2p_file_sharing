#include "proto_messages.h"

using namespace std;
using namespace p2pfilesharing;

NodeResponse generate_lookup_response(bool if_exist, string owner_host_name, string owner_port) {
  // TODO
  NodeResponse rsp;
  rsp.set_type(1);
  return rsp;
}

NodeRequest generate_lookup_request(const LookupFileRequest& req) {
  NodeRequest node_req;
  node_req.set_type(3);
  LookupFileRequest *  req_copy = nullptr;
  req_copy->CopyFrom(req);
  node_req.set_allocated_lookup(req_copy);
  return node_req;
}

