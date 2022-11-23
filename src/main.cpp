#include <iostream>
#include <string>
#include "node.h"
#include "util/config.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr << "Usage: <Contact Node Hostname>\n";
    exit(EXIT_FAILURE);
  }

  string contact_host = string(argv[1]);
  Node& node = Node::get_instance();
  node.entryNode = make_pair(
      argv[1],
      my_config::listening_port_num);  // node加入circle时联系的random
                                       // node，random node会给它发送负责的node
  node.main();
  return EXIT_SUCCESS;
}