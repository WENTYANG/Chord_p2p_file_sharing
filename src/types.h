#ifndef _MY_TYPES_H_
#define _MY_TYPES_H_

#include <iostream>
#include <string>

using namespace std;

typedef long digest_t;                       // The type for the hash digest
typedef string hostname_t;
typedef pair<hostname_t, string> contactInfo_t;  // The pair of (hostname, port)

#endif
