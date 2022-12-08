#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <thread>

#include "threadpool.h"
#include "util/config.h"
#include "util/socket.h"
#include "util/util.h"
#include "util/proto.h"
#include "protobuf/P2P_Protocol.pb.h"
#include "types.h"

#include <openssl/sha.h>
#include <cmath>

using namespace std;
using namespace p2pfilesharing;

class Node {
 private:
  vector<pair<contactInfo_t, digest_t> >
      fingerTable;                             // ((hostname, port), node hash)
  unordered_map<digest_t, contactInfo_t> DHT;  // file hash --> (hostname, port). Files that are responsible by current node.
  unordered_map<digest_t, string> localFiles;  // file hash --> file name
  digest_t local_start; // file hash for the first file that the node is responsible to, this should equals to its predecessor's hash + 1
  digest_t my_hash; // the hash of my hostname

 public:
  contactInfo_t entryNode; // the first node to contact
  hostname_t my_hostname;

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

    void add_file(string filename, const string & port);
    int delete_file(string filename, const string & port);
    void add_file_req_handle(const AddFileRequest & request);
    void del_file_req_handle(const DeleteFileRequest & request);

    /***** Lookup Related *****/
  private:
    // port for the user interface thread, default should be my_config::user_interface_port_num 
    // When this function called by user_terminal, arg port should be my_config::user_interface_port_num.
    // Because user_terminal can only execute one command at one time. 
    // In other cases, port should be a random number.
    pair<bool, contactInfo_t> lookup_successor(digest_t hash, const string& port); 
    
    void lookup_req_handle(const LookupFileRequest& req); // 转发或处理（向sourcehost 发送 response）收到的LookupFileRequest 
    void lookup(digest_t hash, const string & port, bool * does_exist, contactInfo_t * successor, contactInfo_t * owner); 
    
    bool is_responsible_to(digest_t file_hash);
    contactInfo_t get_next_hop_info(digest_t hash);
  /***** Download Related *****/
  private:
    void download_req_handle(const DownloadRequest& req);
    void download(const string& file_name);
  /***** Join Related *****/ 
  private:
    void initial_chord();
    void join_chord(); 
    void join_req_handle(const JoinRequest& join_req);
    void help_join_req_handle(const HelpJoinRequset& help_join_req);
    void handle_update_route_req(const RouteUpdateRequest& update_route_req);
};
  

#endif
