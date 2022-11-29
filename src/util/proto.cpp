#include "proto.h"

using namespace std;

ProtoStreamOut::~ProtoStreamOut() {
  if (out != nullptr) {
    delete out;
  }
  if (fd != 0) {
    close(fd);
  }
}
proto_out* ProtoStreamOut::get_proto_out() {
  if (out != nullptr) {
    return out;
  } else {
    fd = clientRequestConnection(hostname, port);
    out = new proto_out(fd);
    return out;
  }
}

