#ifndef _PROTO_H__
#define _PROTO_H__
#include <string>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "socket.h"

using namespace std;
typedef google::protobuf::io::FileOutputStream proto_out;
typedef google::protobuf::io::FileInputStream proto_in;


// try to send a protobuf to another host
class ProtoStreamOut {
public: 
  ProtoStreamOut(const string & hostname, const string & port) : hostname(hostname), port(port), fd(0), out(nullptr) {}
  ~ProtoStreamOut();
  proto_out* get_proto_out();
  void close_proto_out();

private:
  string hostname;
  string port;
  int fd;
  proto_out * out;
};

// wait to get a protobuf message on a port
class ProtoStreamIn {
public: 
  ProtoStreamIn(const string &port) : port(port), server_fd(0), client_fd(0), in(nullptr) {}
  ~ProtoStreamIn();
  // this might block to wait for connection to come in
  proto_in * get_proto_in();
  void close_proto_in();
  void close_listening_socket();
private:
  string port;
  int server_fd;
  int client_fd;
  proto_in * in;
};

template <typename T>
bool sendMesgTo(const T& message, google::protobuf::io::FileOutputStream* out) {
    {  // extra scope: make output go away before out->Flush()
        // We create a new coded stream for each message.
        // Don’t worry, this is fast.
        google::protobuf::io::CodedOutputStream output(out);
        // Write the size.
        const int size = message.ByteSize();
        output.WriteVarint32(size);
        uint8_t* buffer = output.GetDirectBufferForNBytesAndAdvance(size);
        if (buffer != NULL) {
            // Optimization:  The message fits in one buffer, so use the faster
            // direct-to-array serialization path.
            message.SerializeWithCachedSizesToArray(buffer);
        } else {
            // Slightly-slower path when the message is multiple buffers.
            message.SerializeWithCachedSizes(&output);
            if (output.HadError()) {
                return false;
            }
        }
    }
    out->Flush();
    return true;
}

template <typename T>
bool recvMesgFrom(T& message, google::protobuf::io::FileInputStream* in) {
    google::protobuf::io::CodedInputStream input(in);
    uint32_t size;
    if (!input.ReadVarint32(&size)) {
        cerr << "ReadVarint32 fail" << endl;
        return false;
    }
    // Tell the stream not to read beyond that size.
    google::protobuf::io::CodedInputStream::Limit limit = input.PushLimit(size);
    // Parse the message.
    if (!message.MergeFromCodedStream(&input)) {
        cerr << "Fail to parse mesg" << endl;
        return false;
    }
    if (!input.ConsumedEntireMessage()) {
        cerr << "ConsumedEntireMessage fail" << endl;
        return false;
    }
    // Release the limit.
    input.PopLimit(limit);
    return true;
}

#endif
