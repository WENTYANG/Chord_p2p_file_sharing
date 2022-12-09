sudo apt-get install openssl
sudo apt-get install libssl-dev
cd src
make clean
make
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./protobuf/lib

./p2p_file_sharing $1 $2

