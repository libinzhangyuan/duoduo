#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "Util.h"

// 获取程序位数（被编译为多少位的代码）
int GetProgramBits(void)
{
    return sizeof(int*) * 8;
}


long long current_timestamp(void)
{
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    return milliseconds;
}

void error_exit(const char* error_text)
{
    printf("\nError exit because %s\n        -- errno: %s\n", error_text, strerror(errno));
    exit(-1);
}

//void dd_exception_assert(bool check, char* expression)
void dd_exception_assert(bool check, const char* expression)
{
    if (check == false)
        throw AssertException();
}
