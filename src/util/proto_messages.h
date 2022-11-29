#ifndef _PROTO_MESSAGES_H
#define _PROTO_MESSAGES_H

#include "protobuf/P2P_Protocol.pb.h"

using namespace std;
using namespace p2pfilesharing;

LookupFileResponse generate_lookup_response(bool if_exist, bool owner_host_name, bool owner_port);

#endif
