#ifndef UTIL_H_
#define UTIL_H_

// 获取程序位数（被编译为多少位的代码）
int GetProgramBits(void);

long long current_timestamp(void);

void error_exit(const char* error_text);

#define UNUSED_PARAM(p)    ((void)p)

#define error_log printf
#define info_log printf

#endif
