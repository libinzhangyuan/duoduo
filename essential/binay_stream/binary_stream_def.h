#pragma once
#include "../config.h"

/////////////////////////////////////////////////////////////////////////////
// 这个头文件是binary_stream系统所需要(但使用者并不需要)的公共定义文件
// 不要将这个文件放入任何头文件里面进行引用.
//

#include <exception>


BEGIN_ES_NAMESPACE

	class bin_stream_exception : public ::std::exception
	{
	public:
		bin_stream_exception(const char *const& _What) : ::std::exception(_What) {}
	};

END_ES_NAMESPACE
