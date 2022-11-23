#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class SocketException : public exception {
 public:
  SocketException() : message("Error.") {}
  SocketException(string str) : message(str) {}
  ~SocketException() throw() {}

  virtual const char* what() const throw() { return message.c_str(); }

 private:
  string message;
};

#endif