#include "check_function.h"

void dd_exception_assert(bool check, const char* expression)
{
    if (check == false)
        throw AssertException();
}
