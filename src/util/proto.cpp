#include "proto.h"

using namespace std;

ProtoStreamOut::~ProtoStreamOut() {
  close_proto_out();
}

proto_out* ProtoStreamOut::get_proto_out() {
  if (out != nullptr) {
    return out;
  } else {
    if (fd == 0) {
      fd = clientRequestConnection(hostname, port);
    }
    out = new proto_out(fd);
    return out;
  }
}

void ProtoStreamOut::close_proto_out() {
  if (out != nullptr) {
    delete out;
  }
  out = nullptr;
  if (fd != 0) {
    close(fd);
  }
  fd = 0;  
}

ProtoStreamIn::~ProtoStreamIn() {
  close_proto_in();
  close_listening_socket();
}

proto_in * ProtoStreamIn::get_proto_in() {
  if (in != nullptr) {
    return in;
  } else {
    if (server_fd == 0) {
      server_fd = initializeServer(port);
    }
    if (client_fd == 0) {
      string clientIP;
      client_fd = serverAcceptConnection(server_fd, clientIP);
    }
    in = new proto_in(client_fd);
    return in;
  }
}

void ProtoStreamIn::close_proto_in() {
  if (in != nullptr) {
    delete in;
  }
  in = nullptr;
  if (client_fd != 0) {
    close(client_fd);
  }
  client_fd = 0;
}

void ProtoStreamIn::close_listening_socket() {
  if (server_fd != 0) {
    close(server_fd);
  }
  server_fd = 0;
}
