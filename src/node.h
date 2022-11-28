#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "threadpool.h"
#include "util/config.h"
#include "util/socket.h"
#include "util/util.h"
#include "protobuf/P2P_Protocol.pb.h"

#include <openssl/sha.h>
#include <cmath>

using namespace std;
using namespace p2pfilesharing;

typedef long long int digest_t;                       // The type for the hash digest
typedef string IP_addr_t;
typedef pair<IP_addr_t, string> contactInfo_t;  // The pair of (hostname, port)

class Node {
 private:
  vector<pair<contactInfo_t, digest_t> >
      fingerTable;                             //((hostname, port), node hash)
  unordered_map<digest_t, contactInfo_t> DHT;  // file hash --> (hostname, port)
  unordered_map<digest_t, string> localFiles;  // file hash --> file name

 public:
  contactInfo_t entryNode;

 private:
  Node();

 public:
  ~Node(){};
  Node(const Node&) = delete;
  Node& operator=(const Node) = delete;
  static Node& get_instance() {
    static Node instance;
    return instance;
  }
  void main();        // set up the node
  void run_server();  // initialize the server and listen on port
  void run_user_terminal_interface();  // Start interacting with user on terminal
  digest_t get_hash(string key);

  /***** Lookup Related *****/
  private:
    digest_t local_start; // file hash for the first file that the node is responsible to 
    digest_t local_end; // file hash for the last file that the node is responseible to
    IP_addr_t this_IP;
    
    void lookup_handle(LookupFileRequest& req); // 转发或处理（向sourcehost 发送 response）收到的LookupFileRequest 
    void send_lookup_req(digest_t hash_key, string src_port);
    bool wait_for_lookup_rsp(string src_port);

  /***** Download Related *****/
  private:
    void download_handle(DownloadRequest& req);
    void download(string file_name);
};
  

#endif
