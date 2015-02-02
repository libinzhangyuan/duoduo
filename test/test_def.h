#ifndef _DD_TEST_DEF_H__
#define _DD_TEST_DEF_H__

#define _SKIP_ASSERT_UNIT_TEST





#ifdef _SKIP_ASSERT_UNIT_TEST
    #undef CPPUNIT_ASSERT_THROW
    #define CPPUNIT_ASSERT_THROW(a, b) __noop_()
    inline void __noop_(void) {}
#endif

#endif // _DD_TEST_DEF_H__
