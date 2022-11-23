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
      // Spawn a new thread to handle the request
      // threadPool->assign_task(bind(<func>, <arg>));
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
    // 注意这里的所有函数都是spawn一个新thread来执行，并且要停等（thread.join()），执行完了terminal才继续运行
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
  }

  run_user_terminal_interface();
}