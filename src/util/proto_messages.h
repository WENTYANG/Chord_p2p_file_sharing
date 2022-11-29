#ifndef _PROTO_MESSAGES_H
#define _PROTO_MESSAGES_H

#include "protobuf/P2P_Protocol.pb.h"

using namespace std;
using namespace p2pfilesharing;

NodeResponse generate_lookup_response(bool if_exist, string owner_host_name, string owner_port);

NodeRequest generate_lookup_request(const LookupFileRequest& req);


#endif
