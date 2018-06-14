/*
 * tests.h
 * Header for tests
 */

#ifndef _TESTS_H
#define _TESTS_H

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
extern "C" {
#endif // C

#define WS "                 "
#define UL "______"

#define TEST_UNIT bool
#define TEST(a) static bool a(const char *__tname)

#define BEGIN_TEST_UNIT(a)						\
	const char *__tuname = a;					\
	unsigned long testcnt = 0L;					\
	bool ALL_TESTS_OK = true;					\
	xmsg(0, "BEGIN UNIT >-----[%s].", a);

#define END_TEST_UNIT()							\
	if (! ALL_TESTS_OK)							\
		xmsg(0, WS UL "SOME TESTS IN "			\
			"UNIT [%s] FAILED" UL, __tuname );	\
	xmsg(0, "END UNIT >-------[%s] "			\
		"%ld tests completed.\n",				\
		__tuname, testcnt);						\
	return ALL_TESTS_OK;

#define TEST_PASS	 							\
	do {										\
		xmsg(0, WS "[%s] PASSED.", __tname);	\
		return true;							\
	} while(0)

#define TEST_FAIL(a) 							\
	do {										\
		xmsg(0, WS "[%s] " UL "FAILED" UL		\
			" (%s)!",							\
			__tname, a);						\
		return false;							\
	} while(0)

#define RUN_TEST(a)								\
	do {										\
		testcnt++;								\
		xmsg(0, "BEGIN TEST       [" #a "]"		\
			" (#%ld)", testcnt);				\
		if (! a(#a)) ALL_TESTS_OK = false;		\
		xmsg(0, "END TEST         [" #a "]");	\
	} while(0)

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
}
#endif // C

#endif //_TESTS_H
