#include "node.h"
#include "util/proto_messages.h"

using namespace std;
using namespace p2pfilesharing;

bool Node::is_responsible_to(digest_t file_hash) {
  return true;
}

void Node::lookup_handle(LookupFileRequest& res) {
  digest_t filenamehash = res.filenamehash();
  if (is_responsible_to(filenamehash)) {
    string host_name = res.sourcehostname();
    string port = res.sourceport();
    proto_out* out = get_proto_out(host_name, port);
    
    LookupFileResponse rsp = generate_lookup_response(true, true, true);
    sendMesgTo(rsp, out);    
  } else{
  
  }
}
