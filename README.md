# p2p_file_sharing
Implementation of Chord P2P Distributed File Sharing System

## Usage:
    ./p2p_file_sharing <contact point hostname>
contact point为node加入circle时第一个联系的random node，若contact point hostname = “0.0.0.0” 则意味是第一个加入circle的node
**Hostname should be an IP address, we use IP addresses as keys in the unit circle**

## Structure:  
    [main.cpp]  main(): construct node (Node类为单例，只能construct一次)；设置node的entry_node field  
    [node.cpp]     |__ Node::main(): Spawn listen thread; join circle; run user terminal interface  
    [node.cpp]                  |__ Node::run_server(): Listen on port 8888; Receive connection and handle request  
    [node.cpp]                  |__ Node::run_user_terminal_interface(): Run user terminal interface; receive user command and handle  


## Listen Port:
* 目前暂定每个节点默认在port 8888上listen
* 关于port的设定写在config.h中，include config.h以后用my_config::listening_port_num可以得到port

## Protobuf:
暂时定义了14种消息类型，可以在`P2P_Protocol.proto`中找到。  
如果需要更改，可以更改后用  `protoc P2P_Protocol.proto --cpp_out=./ `  来重新编译生成新的.cc和.h文件

* 这里对不同message的修改可能容易出现conflict，要改的话可能要互相同步一下？
* protobuf教程：https://blog.csdn.net/K346K346/article/details/51754431

## SHA-1:
* 需要手动安装openssl  

        sudo apt-get install openssl
* 以及安装libssl-dev  

        sudo apt-get install libssl-dev

## TODO: 
* 在node.cpp中标明了一些TODO，是listen thread和user terminal interface需要调用函数的地方，以后需要加上函数调用
