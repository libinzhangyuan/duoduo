#pragma once

#define  ES_PUBLISH_VERSION  1.0	// µ±Ç°°æ±¾ºÅ

#ifdef    ES_EXTERN_INCLUDE
#include  ES_EXTERN_INCLUDE
#endif

#ifdef   IGNORE_ES_NAMESPACE
#define  BEGIN_ES_NAMESPACE
#define  END_ES_NAMESPACE
#else
#define  BEGIN_ES_NAMESPACE  namespace  Essential {
#define  END_ES_NAMESPACE    }
#endif

#define  BEGIN_ES_DETAIL_NAMESPACE  namespace  Es_Implement_Detail {
#define  END_ES_DETAIL_NAMESPACE    }

#ifndef WINVER
#define WINVER 0x0501
#endif 

#ifndef _BOT_NAME_
#define _BOT_NAME_	"SROKing"
#endif

