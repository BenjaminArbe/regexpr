#include <regexpr.h>
#include "minunit.h"

char *
step_backslash() {
	char *p = compile("\\(uv\\)0123\\1", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abcduv0123uvzyu";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(*loc1 == 'u', "loc1 didn't match");
	mu_assert(*loc2 == 'z', "loc2 didn't match");
	mu_assert(*braslist[0] == 'u', "starting of substring 0 didn't match");
	mu_assert(*braelist[0] == '0', "ending of substring didn't match");
	if (p) free(p);
	return 0;
}

char *
step_backslash_multiple() {
	char *p = compile("\\(uv\\)\\(012\\).*\\1\\2", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abcuv012vvvfguv012mnopq";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(*loc1 == 'u', "loc1 didn't match");
	mu_assert(*loc2 == 'm', "loc2 didn't match");
	mu_assert(*braslist[0] == 'u', "starting of substring 0 didn't match");
	mu_assert(*braelist[0] == '0', "ending of substring 0 didn't match");
	mu_assert(*braslist[1] == '0', "starting of substring 1 didn't match");
	mu_assert(*braelist[1] == 'v', "ending of substring 1 didn't match");
	
	if (p) free(p);
	return 0;
}

char *
step_backslash_nested() {
	char *p = compile("\\(uv\\(012\\)xy\\).*\\1\\2", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abcuv012xyhhhggjuv012xy012mnpq";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(*loc1 == 'u', "loc1 didn't match");
	mu_assert(*loc2 == 'm', "loc2 didn't match");
	mu_assert(*braslist[0] == 'u', "starting of substring 0 didn't match");
	mu_assert(*braelist[0] == 'h', "ending of substring 0 didn't match");
	mu_assert(*braslist[1] == '0', "starting of substring 1 didn't match");
	mu_assert(*braelist[1] == 'x', "ending of substring 1 didn't match");
	
	if (p) free(p);
	return 0;
}

char *
step_backslash_nested_1() {
	char *p = compile("\\(uv\\(012\\)xy\\).*\\1\\2???\\2\\1", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abcuv012xyhhhggjuv012xy012???012uv012xymnpq";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(*loc1 == 'u', "loc1 didn't match");
	mu_assert(*loc2 == 'm', "loc2 didn't match");
	mu_assert(*braslist[0] == 'u', "starting of substring 0 didn't match");
	mu_assert(*braelist[0] == 'h', "ending of substring 0 didn't match");
	mu_assert(*braslist[1] == '0', "starting of substring 1 didn't match");
	mu_assert(*braelist[1] == 'x', "ending of substring 1 didn't match");
	
	if (p) free(p);
	return 0;
}

char *
all_tests() {
	mu_suite_start();
	
	mu_run_test(step_backslash);
	mu_run_test(step_backslash_multiple);
	mu_run_test(step_backslash_nested);
	mu_run_test(step_backslash_nested_1);
	
	return 0;
}

RUN_TESTS(all_tests);