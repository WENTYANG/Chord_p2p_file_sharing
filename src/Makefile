CC=g++ 
CFLAG = -g -Wall -std=c++11
UTIL_DIR = ./util/
PROTO_DIR = ./protobuf/
EXECUTABLE_FILE = p2p_file_sharing
INCLUDE = -I ./ -I ./util/ -I ./protobuf/lib/ -I ./protobuf/include/ -I ./protobuf/
LIBS_DIR = -L./protobuf/lib/ -lprotobuf
LIBS = -pthread	-lssl -lcrypto

all: $(EXECUTABLE_FILE)

SOURCE=$(wildcard ./*.cpp $(UTIL_DIR)*.cpp)	#Match all the .cpp source files in current dir and util dir, match all the .cc files in protobuf dir

OBJECTS=$(patsubst %.cpp,%.o,$(SOURCE))	#Find all the .cpp in SOURCE and replace with .o

PROTO_SOURCE=$(wildcard $(PROTO_DIR)*.cc)

$(EXECUTABLE_FILE): $(OBJECTS) $(PROTO_SOURCE)
	$(CC) $(CFLAG) $(INCLUDE)  $^ -o $@ $(LIBS_DIR) $(LIBS)

$(OBJECTS): %.o: %.cpp
	$(CC) $(CFLAG) $(INCLUDE) -c $< -o $@ $(LIBS)

.PHONY:
	clean
clean:
	rm -rf $(OBJECTS) $(EXECUTABLE_FILE)
