#pragma once
#include "../config.h"

/////////////////////////////////////////////////////////////////////////////
// ���ͷ�ļ���binary_streamϵͳ����Ҫ(��ʹ���߲�����Ҫ)�Ĺ��������ļ�
// ��Ҫ������ļ������κ�ͷ�ļ������������.
//

#include <exception>


BEGIN_ES_NAMESPACE

	class bin_stream_exception : public ::std::exception
	{
	public:
		bin_stream_exception(const char *const& _What) : ::std::exception(_What) {}
	};

END_ES_NAMESPACE
