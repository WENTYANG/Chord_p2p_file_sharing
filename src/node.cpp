#include "node.h"

Node::Node() {
  fingerTable = vector<pair<contactInfo_t, digest_t> >(
      my_config::finger_table_length);  // Construct an empty finger table
                                        // with a length (specified in
                                        // config.h)
}

/*
    Initialize the node server and listen on the port to accept connections from
   other nodes
*/
void Node::run_server() {
  int server_fd = initializeServer(my_config::listening_port_num);
  cout << "Start listening on port" << my_config::listening_port_num << endl;
  while (1) {
    string clientIP;
    try {
      int client_fd = serverAcceptConnection(server_fd, clientIP);
      cout << "Accepted a connection from " << clientIP << endl;
      string request = socketRecvMsg(client_fd);
      cout << "Received a request " << request << endl;
      close(client_fd);
      // TODO: Spawn a new thread to handle the request
    } catch (const std::exception& e) {
      std::cerr << e.what() << endl;
      continue;
    }
  }
}

/*
    User terminal interface
*/
void Node::run_user_terminal_interface() {
  cout << "Welcome to the file sharing system! What can we help with you "
          "today?"
       << endl;
  while (1) {
    // Print the options
    cout << "Please choose a number from the following options:" << endl;

    cout << "1 Upload a file" << endl;
    cout << "2 Delete a file" << endl;
    cout << "3 Search if a file exists" << endl;
    cout << "4 Download a file" << endl;
    cout << "5 Node exit" << endl;

    // Receive command from user
    int option;
    cin >> option;

    // Process command
    switch (option) {
      case 1:
        // TODO: fm128 这里调用add file函数
        break;
      case 2:
        // TODO: fm128 这里调用delete file函数
        break;
      case 3:
        // TODO: ky99 这里调用lookup file函数
        break;
      case 4:
        // TODO: ky99 这里调用lookup file, 然后downlod file
        break;
      case 5:
        // TODO: jz399 这里调用节点退出函数
        exit(EXIT_SUCCESS);
      default:
        cout << "Invalid input, please select a number from the provided "
                "options!"
             << endl;
        continue;  // Start over, print the options again and let user choose
    }

    cout << "Your request has been completed! What else can we do?" << endl;
  }
}

/*
    Set up the node, spawn the listen thread => join the circle => run user
   terminal interface
*/
void Node::main() {
  // Spawn a thread to init server and listen
  thread listenThread(&Node::run_server, this);

  // Join the circle
  if (entryNode.first != "0.0.0.0") {
    // Node is not the first one in the circle
    // TODO: jz399
    // 这里调用新节点加入的函数，第一个联系的随机节点信息存在entryNode这个pair中，first是hostname，second是port
    // please also update local_start and local_end 
  }

  run_user_terminal_interface();
}

/*
  Get the SHA-1 hash for a given key
  Each node will be assigned a unique ID (within 2^m (m is 48 in this case)) 
  by hashing key which will be hostname of that node by SHA-1 Algorithm
  => a maximum of 2^48 nodes can join the circle, the finger table length is log2(2^m)=48
*/
digest_t Node::get_hash(string key){
  unsigned char obuf[41];
    char finalHash[41];
    string keyHash = "";
    long unsigned int i;
    long unsigned int m = my_config::finger_table_length;
    digest_t mod = pow(2, m);

    // convert string to an unsigned char array because SHA1 takes unsigned char array as parameter
    unsigned char unsigned_key[key.length()+1];
    for(i=0;i<key.length();i++){
        unsigned_key[i] = key[i];
    }
    unsigned_key[i] = '\0';


    SHA1(unsigned_key,sizeof(unsigned_key),obuf);
    for (i = 0; i < m/8; i++) {
        sprintf(finalHash,"%d",obuf[i]);
        keyHash += finalHash;
    }

    digest_t hash = stoll(keyHash) % mod;

    return hash;
}
