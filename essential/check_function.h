#ifndef _CHECK_FUNCTION_H_
#define _CHECK_FUNCTION_H_

#ifdef _UTEST_
    #define assert_check(check, expression) dd_exception_assert(bool(check), (const char*)(expression))
#else
    #define assert_check(check, expression) assert(check)
#endif

class AssertException
{
};
void dd_exception_assert(bool check, const char* expression);




#endif
