#include <regexpr.h>
#include "minunit.h"

char *
rpt_dot_one_parameter() {
	char *p = compile("ab.\\{3\\}c",0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abcxabccyaabccczaabccccuababdc";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "abcccc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'u', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "ababdc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == '\0', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 0, "Failure matching RE");
	
	if (p) free(p);
	return 0;
}

char *
rpt_dot_one_comma_parameter() {
	char *p = compile("ab.\\{3,\\}c",0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abcxaccyaabccczaabccccuababdc";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "abcxaccyaabccczaabccccuababdc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == '\0', "wrong loc2 value");
	char s1[] = "bcxabccyaabccczaabuababd";
	r = step(s1, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "abccyaabccc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'z', "wrong loc2 value");
	
	if (p) free(p);
	return 0;
}

char *
rpt_dot_comma_one_parameter() {
	char *p = compile("ab.\\{,3\\}c",0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abcxaccyaabccczaabccccuabacabc";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "abcxac", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'c', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "abccc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'z', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "abcccc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'u', "wrong loc2 value");
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "abac", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'a', "wrong loc2 value");
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "abc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == '\0', "wrong loc2 value");
	r = step(loc2, p);
	mu_assert(r == 0, "Failure matching RE");
	
	
	if (p) free(p);
	return 0;
}

char *
rpt_dot_comma_two_parameter() {
	char *p = compile("a.\\{3,5\\}bc",0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abcxaabcyaaabczaaaabcuaaaaabcvaaaaaaabcw";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "abcxaabc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'y', "wrong loc2 value");
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
	mu_assert(strncmp(loc1, "aaaaaabc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'w', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 0, "Failure matching RE");
	
	if (p) free(p);
	return 0;
}

char *
all_tests() {
	mu_suite_start();
	
	mu_run_test(rpt_dot_one_parameter);
	mu_run_test(rpt_dot_one_comma_parameter);
	mu_run_test(rpt_dot_comma_one_parameter);
	mu_run_test(rpt_dot_comma_two_parameter);
	
	return 0;
}

RUN_TESTS(all_tests);
