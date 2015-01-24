#pragma once
#include <string.h>
#include "../es_config.h"

/////////////////////////////////////////////////////////////////////////////
// ���ͷ�ļ���binary_streamϵͳ����Ҫ(��ʹ���߲�����Ҫ)�Ĺ��������ļ�
// ��Ҫ������ļ������κ�ͷ�ļ������������.
//

#include <exception>


BEGIN_ES_NAMESPACE

	class bin_stream_exception : public ::std::exception
	{
	public:
		bin_stream_exception(const char *const& _What)
        {
            strlcpy(m_What, _What, sizeof(m_What));
        }
        ~bin_stream_exception() _NOEXCEPT {}

    private:
        char m_What[1024];
	};

END_ES_NAMESPACE
