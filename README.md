# p2p_file_sharing
Implementation of Chord P2P Distributed File Sharing System
Google Doc: https://docs.google.com/document/d/1jgwUZPEkZfJI1MtdhHUCUf4GK_gOesnj7hNxs2x2Hqc/edit

## Usage:
    ./p2p_file_sharing <Contact Node Hostname> <My Hostname>
contact point为node加入circle时第一个联系的random node，若contact point hostname = “0.0.0.0” 则意味是第一个加入circle的node

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

# Protobuf Compiler:
如果要安装protobuf compiler,可以把protobuf目录下的zip文件解压到$HOME/.local/文件夹下  

    unzip protoc-3.20.1-linux-x86_64.zip -d $HOME/.local/
    export PATH="$PATH:$HOME/.local/bin"
执行`protoc --version`应看到version为libprotoc 3.20.1

运行前需要执行 export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./protobuf/lib 

* 这里对不同message的修改可能容易出现conflict，要改的话可能要互相同步一下？
* protobuf教程：https://blog.csdn.net/K346K346/article/details/51754431

## SHA-1:
* 需要手动安装openssl  

        sudo apt-get install openssl
* 以及安装libssl-dev  

        sudo apt-get install libssl-dev

## TODO: 
* 在node.cpp中标明了一些TODO，是listen thread和user terminal interface需要调用函数的地方，以后需要加上函数调用

## Lookup:
调用lookup_successor()可以获得hashvalue的successor，其他部分应该用到这个函数

## Download:
localFiles 保存在 ./shared_file 路径下，下载的文件保存在 ./download_file 路径下
