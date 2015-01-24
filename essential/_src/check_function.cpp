#include "../check_function.h"

namespace Essential
{

    ::std::string make_check_message(const char* const expression, const char* const fileName, int line, const char* const message)
    {
        char moudleName[1024] = ("");
        //::GetModuleFileNameA(NULL, moudleName, sizeof(moudleName) / sizeof(char));


        char showMessage[1024 * 4] = ("");
        snprintf(showMessage, sizeof(showMessage) / sizeof(char),
                "                                                         \r\n"
                "Moudle Name: %s\r\n"
                "File:        %s\r\n"
                "Line:        [%d]\r\n"
                "\r\n"
                "Expression:  %s\r\n"
                "Message:     %s",
                moudleName, fileName, line, expression, message);

        return ::std::string(showMessage);
    }


    void do_assert(const char* const expression, const char* const fileName, int line, const char* const message)
    {
        // log
        // throw exception
        throw AssertException(make_check_message(expression, fileName, line, message).c_str());
    }

    void _assert_check( bool condition, const char* const expression, const char* const fileName, int line, const char* const message )
    {
        if (!condition)
        {
            do_assert(expression, fileName, line, message);
        }
    }

}
