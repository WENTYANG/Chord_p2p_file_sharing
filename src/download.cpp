#include "node.h"
#include "util/proto_messages.h"

#include <fstream>

void Node::download_req_handle(const DownloadRequest& req) {
  digest_t hash = req.filenamehash();
  string source_host_name = req.sourcehostname();
  string source_port = req.sourceport();
  
  // file does not exist
  if (localFiles.find(hash) == localFiles.end()) {
    cerr << source_host_name << " is requesting a file that is not in my localFiles! file hash :" << hash << endl;
    // connect to source and close right away, need to catch the exception
    int fd = clientRequestConnection(source_host_name, source_port);
    close(fd);
    return;
  } else { // file exist in localFile
    string file_name = "./shared_file/" + localFiles[hash];
    ifstream ifs(file_name, ifstream::binary);
    if (!ifs.good()) {
      cerr << file_name << "exists in localFiles, but can not find it in ./shared_file\n";
      // connect to source and close right away, need to catch the exception
      ifs.close();
      return;
    }
    // send file to user
    char buffer[MAX_LENGTH];
    int length = 0;
    
    ifs.seekg(0, ifs.end);
    length = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    
    int fd = clientRequestConnection(source_host_name, source_port);
    socketSendMsg(fd, (void *)(&length), sizeof(int));
    cout << "length printed: " << length << endl;

    while (length > 0) {
      int len = min(MAX_LENGTH, length);
      ifs.read(buffer, len);

      socketSendMsg(fd, (void *)buffer, len);
      length -= len;
    }
    close(fd);
    ifs.close();
  }
}

void Node::download(const string & filename) {
  // check if already have the file
  string file_name = "./download_file/" + filename;
  ifstream ifs(file_name);
  if (ifs.good()) {
    cout << "\nLooks like you already have this file! Please check your ./download_file directory.\n\n";
    ifs.close();
    return;
  }
  ifs.close();

  // lookup if the file sharing system has the file
  digest_t hash = get_hash(filename);
  bool does_exist;
  contactInfo_t successor, owner;
  lookup(hash, my_config::user_interface_port_num, &does_exist, &successor, &owner);
  if (does_exist == false) {
    cout << "\nSorry! We do not have this file.\n\n";
    return;
  }
  
  // check if the file is on this node
  if (owner.first == my_hostname) {
    cout << "\nLooks like you already have this file! Please check your ./shared_file directory.\n\n";
    return;
  }

  // send out DownloadRequest
  ProtoStreamOut proto_stream_out(owner.first, owner.second);
  proto_out * out = proto_stream_out.get_proto_out();
  NodeRequest req = generate_download_request(hash, my_hostname, my_config::user_interface_port_num); 
  sendMesgTo<NodeRequest> (req, out);
  proto_stream_out.close_proto_out();

  // wait for socket connection coming in
  int server_fd = initializeServer(my_config::user_interface_port_num);
  string client_IP;
  int client_fd = serverAcceptConnection(server_fd, client_IP);
  
  int length;
  
  // check if we could successfully receive the file from another node
  try {

    int len = recv(client_fd, (void*)(&length), sizeof(int), 0);
    cout << "len: " << len << " length:" << length << endl;
  } catch (const std::exception& e) {
    close(client_fd);
    cout << "\nAn error occured when downloading the file. Please try later!\n\n";
    cout << e.what() << endl;
    return;
  }
  ofstream ofs(file_name, ofstream::binary);
  while (length > 0) {
      string received = socketRecvMsg(client_fd);
      ofs.write(received.data(), received.size());
      length -= received.size();
  }
  close(client_fd);
  ofs.close();
  cout << "\nDownload Complete! Please check your ./download_file directory.\n\n";
}
