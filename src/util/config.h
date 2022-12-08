#ifndef CONFIG_H
#define CONFIG_H

#include <cmath>
#include "../types.h"
// 这个namespace包含所有的全局常量，引用这个头文件以后用my_config::<constant>来引用相应常量

namespace my_config {
const string listening_port_num = "8888";
const string user_interface_port_num = "8889";
const int finger_table_length = 16;
const digest_t total_hash_points = pow(2, finger_table_length);
}  // namespace my_config

#endif
