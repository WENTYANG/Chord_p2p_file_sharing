#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include "threadpool.h"
#include "util/config.h"
#include "util/socket.h"
#include <openssl/sha.h>
#include <cmath>

using namespace std;
typedef long long int digest_t;                       // The type for the hash digest
typedef pair<string, string> contactInfo_t;  // The pair of (hostname, port)

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
};

#endif