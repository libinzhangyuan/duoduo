#ifndef UTIL_H_
#define UTIL_H_

// 获取程序位数（被编译为多少位的代码）
int GetProgramBits(void);

long long current_timestamp(void);

void error_exit(const char* error_text);

#define UNUSED_PARAM(p)    ((void)p)

#ifdef _UTEST_
    #define assert_check(check, expression) dd_exception_assert(bool(check), (const char*)(expression))

#else
    #define assert_check(check, expression) assert(check)
#endif

#include <string>
class AssertException
{
};
void dd_exception_assert(bool check, const char* expression);
#define error_log printf
#define info_log printf

#endif
