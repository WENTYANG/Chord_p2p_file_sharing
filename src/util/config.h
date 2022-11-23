#ifndef CONFIG_H
#define CONFIG_H

// 这个namespace包含所有的全局常量，引用这个头文件以后用my_config::<constant>来引用相应常量

namespace my_config {
const string listening_port_num = "8888";
const int finger_table_length = 48;
}  // namespace my_config

#endif