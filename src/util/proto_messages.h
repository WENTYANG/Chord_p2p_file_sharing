#ifndef _PROTO_MESSAGES_H
#define _PROTO_MESSAGES_H

#include "protobuf/P2P_Protocol.pb.h"
#include "types.h"

using namespace std;
using namespace p2pfilesharing;

NodeResponse generate_lookup_response(bool if_exist, const string& successor_host_name, const string& successor_port, const string& owner_host_name, const string& owner_port);
NodeRequest generate_lookup_request(const LookupFileRequest& req);
NodeRequest generate_lookup_request(digest_t hash, const string & hostname, const string & port);
NodeRequest generate_download_request(digest_t hash, const string & hostname, const string & port);


NodeRequest generate_add_file_request(digest_t hash, const string & hostname, const string & port);
NodeRequest generate_delete_file_request(digest_t hash, const string & hostname, const string & port);


NodeRequest generate_join_request(digest_t hash, const string& hostname, const string& port);
NodeRequest generate_help_join_request(digest_t hash, const string& hostname, const string& port);
NodeResponse generate_help_join_response(digest_t hash, const string& hostname, const string& port);
NodeResponse generate_routetable_init(RouteTableInit* finger_table);
NodeRequest generate_route_update_request(digest_t hash, const string& hostname, const string& port);
NodeResponse generate_route_update_response(digest_t cur_hash, digest_t next_hash, const string& hostname, const string& port);
NodeResponse generate_filetable_response(FileTable* file_table);
NodeResponse generate_lookup_node_response(digest_t hash, const string& hostname, const string& port);

#endif
