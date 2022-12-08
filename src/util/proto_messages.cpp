#include "proto_messages.h"

using namespace std;
using namespace p2pfilesharing;

NodeResponse generate_lookup_response(bool if_exist,
                                      const string& successor_host_name,
                                      const string& successor_port,
                                      const string& owner_host_name,
                                      const string& owner_port) {
  LookupFileResponse* lrsp = new LookupFileResponse();
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
  LookupFileRequest* req_copy = new LookupFileRequest();
  req_copy->CopyFrom(req);
  node_req.set_allocated_lookup(req_copy);
  return node_req;
}

NodeRequest generate_lookup_request(digest_t hash, const string& hostname,
                                    const string& port) {
  NodeRequest node_req;
  node_req.set_type(3);
  LookupFileRequest* req = new LookupFileRequest();
  req->set_filenamehash(hash);
  req->set_sourcehostname(hostname);
  req->set_sourceport(port);
  node_req.set_allocated_lookup(req);
  return node_req;
}

NodeRequest generate_download_request(digest_t hash, const string& hostname,
                                      const string& port) {
  NodeRequest node_req;
  node_req.set_type(5);
  DownloadRequest* req = new DownloadRequest();
  req->set_filenamehash(hash);
  req->set_sourcehostname(hostname);
  req->set_sourceport(port);
  node_req.set_allocated_download(req);
  return node_req;
}

NodeRequest generate_add_file_request(digest_t hash, const string & hostname, const string & port) {
    NodeRequest node_req;
    node_req.set_type(2);
    AddFileRequest * req = new AddFileRequest();
    req->set_filenamehash(hash);
    req->set_sourcehostname(hostname);
    req->set_sourceport(port);
    node_req.set_allocated_addfile(req);
    return node_req;
}

NodeRequest generate_delete_file_request(digest_t hash, const string & hostname, const string & port) {
    NodeRequest node_req;
    node_req.set_type(4);
    DeleteFileRequest * req = new DeleteFileRequest();
    req->set_filenamehash(hash);
    req->set_sourcehostname(hostname);
    req->set_sourceport(port);
    node_req.set_allocated_deletefile(req);
    return node_req;
}

NodeRequest generate_join_request(digest_t hash, const string& hostname,
                                  const string& port) {
  NodeRequest node_req;
  node_req.set_type(6);
  JoinRequest* req = new JoinRequest();
  req->set_newhostnamehash(hash);
  req->set_newhostname(hostname);
  req->set_newport(port);
  node_req.set_allocated_join(req);
  return node_req;
}

NodeRequest generate_help_join_request(digest_t hash, const string& hostname,
                                       const string& port) {
  NodeRequest node_req;
  node_req.set_type(9);
  HelpJoinRequset* req = new HelpJoinRequset();
  req->set_newhostname(hostname);
  req->set_newport(port);
  req->set_newhostnamehash(hash);
  node_req.set_allocated_helpjoin(req);
  return node_req;
}

NodeResponse generate_help_join_response(digest_t hash, const string& hostname,
                                         const string& port) {
  NodeResponse node_resp;
  node_resp.set_type(6);
  HelpJoinResponse* resp = new HelpJoinResponse();
  resp->set_responsiblehostname(hostname);
  resp->set_responsibleport(port);
  resp->set_responhostnamehash(hash);
  node_resp.set_allocated_helpjoin(resp);
  return node_resp;
}

NodeResponse generate_routetable_init(RouteTableInit* finger_table) {
  NodeResponse node_resp;
  node_resp.set_type(4);
  node_resp.set_allocated_routeinit(finger_table);
  return node_resp;
}

NodeRequest generate_route_update_request(digest_t hash, const string& hostname,
                                          const string& port) {
  NodeRequest node_req;
  node_req.set_type(7);
  RouteUpdateRequest* req = new RouteUpdateRequest();
  req->set_newhostname(hostname);
  req->set_newport(port);
  req->set_newhostnamehash(hash);
  node_req.set_allocated_updateroute(req);
  return node_req;
}

NodeResponse generate_route_update_response(digest_t cur_hash,
                                            digest_t next_hash,
                                            const string& hostname,
                                            const string& port) {
  NodeResponse node_resp;
  node_resp.set_type(7);
  RouteUpdateResponse* resp = new RouteUpdateResponse();
  resp->set_nextupdatehostnamehash(next_hash);
  resp->set_nextupdatehostname(hostname);
  resp->set_nextupdateport(port);
  resp->set_curupdatehostnamehash(cur_hash);
  node_resp.set_allocated_updateroute(resp);
  return node_resp;
}

NodeResponse generate_filetable_response(FileTable* file_table) {
  NodeResponse node_resp;
  node_resp.set_type(5);
  node_resp.set_allocated_filetable(file_table);
  return node_resp;
}

NodeResponse generate_lookup_node_response(digest_t hash,
                                           const string& hostname,
                                           const string& port) {
  NodeResponse node_resp;
  node_resp.set_type(8);
  LookupNodeResponse* resp = new LookupNodeResponse();
  resp->set_hostname(hostname);
  resp->set_hostnamehash(hash);
  resp->set_port(port);
  node_resp.set_allocated_lookupnode(resp);
  return node_resp;
}
