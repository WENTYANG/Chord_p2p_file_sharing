#include "proto.h"

using namespace std;

proto_in* get_proto_in(string& hostname, string&) {
  proto_in* tmp =  new proto_in(0);
  return tmp;
}

proto_out* get_proto_out(string& hostname, string&) {
  proto_out* tmp =  new proto_out(0);
  return tmp;
}

