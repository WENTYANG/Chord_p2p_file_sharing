#include "proto_messages.h"

using namespace std;
using namespace p2pfilesharing;

NodeResponse generate_lookup_response(bool if_exist, const string& successor_host_name, const string& successor_port, const string& owner_host_name, const string& owner_port) {
  LookupFileResponse * lrsp = new LookupFileResponse();
  lrsp->set_ifexists(if_exist);
  lrsp->set_successorhostname(successor_host_name);
  lrsp->set_successorport(successor_port);
  lrsp->set_ownerhostname(owner_host_name);
  lrsp->set_ownerport(owner_port);
  NodeResponse rsp;
  rsp.set_allocated_lookup(lrsp);
  rsp.set_type(2);
  return rsp;
}

NodeRequest generate_lookup_request(const LookupFileRequest& req) {
  NodeRequest node_req;
  node_req.set_type(3);
  LookupFileRequest *  req_copy = new LookupFileRequest();
  req_copy->CopyFrom(req);
  node_req.set_allocated_lookup(req_copy);
  return node_req;
}

NodeRequest generate_lookup_request(digest_t hash, const string & hostname, const string & port) {
  NodeRequest node_req;
  node_req.set_type(3);
  LookupFileRequest * req = new LookupFileRequest();
  req->set_filenamehash(hash);
  req->set_sourcehostname(hostname);
  req->set_sourceport(port);
  node_req.set_allocated_lookup(req);
  return node_req;
}


NodeRequest generate_download_request(digest_t hash, const string & hostname, const string & port) {
  NodeRequest node_req;
  node_req.set_type(5);
  DownloadRequest * req = new DownloadRequest();
  req->set_filenamehash(hash);
  req->set_sourcehostname(hostname);
  req->set_sourceport(port);
  node_req.set_allocated_download(req);
  return node_req;
}
