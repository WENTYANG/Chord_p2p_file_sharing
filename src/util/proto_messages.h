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


#endif
