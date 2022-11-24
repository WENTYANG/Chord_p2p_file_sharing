CC=g++ 
CFLAG = -g -Wall
UTIL_DIR = ./util/
EXECUTABLE_FILE = p2p_file_sharing
INCLUDE = -I ./util/ -I ./
LIBS = -pthread	-lssl -lcrypto

all: $(EXECUTABLE_FILE)

SOURCE=$(wildcard ./*.cpp $(UTIL_DIR)*.cpp)	#Match all the .cpp source files in current dir and util dir

OBJECTS=$(patsubst %.cpp,%.o,$(SOURCE))	#Find all the .cpp in SOURCE and replace with .o

$(EXECUTABLE_FILE): $(OBJECTS)
	$(CC) $(CFLAG) $(INCLUDE) $^ -o $@ $(LIBS)

$(OBJECTS): %.o: %.cpp
	$(CC) $(CFLAG) $(INCLUDE) -c $< -o $@ $(LIBS)

.PHONY:
	clean
clean:
	rm -rf $(OBJECTS) $(EXECUTABLE_FILE)