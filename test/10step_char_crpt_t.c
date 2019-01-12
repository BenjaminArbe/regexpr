#include <regexpr.h>
#include "minunit.h"

char *
rpt_curlybraces_as_1st_char() {
	char *p = compile("\\{abc", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "{abcsyx";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"{abc",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 's', "wrong loc2 value");

	p = compile("{abc", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s1[] = "{abcsyx";
	r = step(s1, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"{abc",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 's', "wrong loc2 value");
	
	if (p) free(p);
	return 0;
}

char *
rpt_char_one_parameter() {
	char *p = compile("a\\{3\\}bc",0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abcxaabcyaaabczaaaabcu";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "aaabc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'z', "wrong loc2 value");
	
	if (p) free(p);
	return 0;
}

char *
rpt_char_one_comma_parameter() {
	char *p = compile("a\\{3,\\}bc",0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abcxaabcyaaabczaaaabcuaaaaabc";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "aaabc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'z', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "aaaabc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'u', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "aaaaabc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == '\0', "wrong loc2 value");
	
	if (p) free(p);
	return 0;
}

char *
rpt_char_comma_one_parameter() {
	char *p = compile("a\\{,3\\}bc",0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "bcxabcyaabczaaabcuaaaabc";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "bc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'x', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "abc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'y', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "aabc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'z', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "aaabc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'u', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "aaabc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == '\0', "wrong loc2 value");
	
	if (p) free(p);
	return 0;
}

char *
rpt_char_two_parameter() {
	char *p = compile("a\\{3,5\\}bc",0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abcxaabcyaaabczaaaabcuaaaaabcvaaaaaabcw";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "aaabc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'z', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "aaaabc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'u', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "aaaaabc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'v', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "aaaaabc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'w', "wrong loc2 value");
	
	if (p) free(p);
	return 0;
}

char *
rpt_char_limits() {
	char *p = compile("a\\{127\\}bc", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	p = compile("a\\{128\\}bc", 0, 0);
	mu_assert(p == 0, "Failure compiling RE");
	p = compile("a\\{1,127\\}bc", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	p = compile("a\\{1,128\\}bc", 0, 0);
	mu_assert(p == 0, "Failure compiling RE");
	
	if (p) free(p);
	return 0;
}

char *
all_tests() {
	mu_suite_start();
	
	mu_run_test(rpt_curlybraces_as_1st_char);
	mu_run_test(rpt_char_one_parameter);
	mu_run_test(rpt_char_one_comma_parameter);
	mu_run_test(rpt_char_comma_one_parameter);
	mu_run_test(rpt_char_two_parameter);
	mu_run_test(rpt_char_limits);
	
	return 0;
}

RUN_TESTS(all_tests);
