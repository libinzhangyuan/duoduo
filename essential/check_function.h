#ifndef _CHECK_FUNCTION_H_
#define _CHECK_FUNCTION_H_
#include <exception>
#include <string>
#include "es_config.h"

BEGIN_ES_NAMESPACE

#ifdef _UTEST_
    #define assert_check(check, expression) exception_assert(bool(check), (const char*)(expression))
#else
    #define assert_check(condition, message) Essential::_assert_check((condition), (#condition), (__FILE__), __LINE__, (message))
#endif

class AssertException : public ::std::exception
{
public:
	AssertException(const char *const& _What) : m_What(_What) {}
    ~AssertException() _NOEXCEPT {}
private:
    std::string m_What;
};

#define exception_assert(condition, message) Essential::_excption_check<Essential::AssertException>((condition), (#condition), (__FILE__), __LINE__, (message))

#define do_check(checkFunction, condition, message) checkFunction((condition), (#condition), (__FILE__), __LINE__, (message))



// *****************************************************************
//	下面是实现:
// *****************************************************************
//
	typedef void (*check_fuction)(  bool				/* condition */, 
									const char* const	/* expression */,
									const char* const	/* fileName */,
									int					/* line */,
									const char* const	/* message */);



	void _assert_check( bool condition, const char* const expression, const char* const fileName, int line, const char* const message );


	::std::string make_check_message(const char* const expression, const char* const fileName, int line, const char* const message);

	template <class exception_type>
	void _excption_check(bool condition, const char* const expression, const char* const fileName, int line, const char* const message) {
		if (!condition)
		{
			::std::string msg = make_check_message(expression, fileName, line, message);
            printf("%s", msg.c_str());
			throw exception_type(msg.c_str());
		}
	}




END_ES_NAMESPACE


#endif
