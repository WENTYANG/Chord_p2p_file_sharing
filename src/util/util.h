#ifndef _UTIL_H
#define _UTIL_H

#include <string>
#include "../types.h"
#include "config.h"


using namespace std;

int get_random_port();
digest_t get_distance(digest_t base, digest_t target, digest_t N = my_config::finger_table_length);
digest_t hash_value_add(digest_t hash, digest_t num);
void print_contactInfo(contactInfo_t t);
#endif
