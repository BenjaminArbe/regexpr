#include <regexpr.h>
#include "minunit.h"

char *
step_cc_normal() {
	char *p = compile("01[abc]345",0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "xyz01a345mnp";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(*loc1 == '0', "loc1 didn't match");
	mu_assert(*loc2 == 'm', "loc2 didn't match");
	mu_assert(strncmp(loc1, "01a345", loc2-loc1) == 0, "wrong match");
	char s1[] = "xyz01b345mnp";
	r = step(s1, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "01b345", loc2-loc1) == 0, "wrong match");
	char s2[] = "xyz01c345mnp";
	r = step(s2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "01c345", loc2-loc1) == 0, "wrong match");
	char s3[] = "xyz01@345mnp";
	r = step(s3, p);
	mu_assert(r == 0, "Failure matching RE");
	
	if(p) free(p);
	return 0;
}

char *
step_cc_neg() {
	char *p = compile("01[^abc]345",0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "xyz01a345mnp";
	int r = step(s, p);
	mu_assert(r == 0, "Failure matching RE");
	char s1[] = "xyz01b345mnp";
	r = step(s1, p);
	mu_assert(r == 0, "Failure matching RE");
	char s2[] = "xyz01c345mnp";
	r = step(s2, p);
	mu_assert(r == 0, "Failure matching RE");
	char s3[] = "xyz01@345mnp";
	r = step(s3, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "01@345", loc2-loc1) == 0, "wrong match");
	
	if(p) free(p);
	return 0;
}

char *
step_cc_range() {
	char *p = compile("ab[0-3]xy", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "!@#ab0xyz";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "ab0xy",loc2-loc1) == 0, "wrong match");
	char s1[] = "!@ab1xyz";
	r = step(s1, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "ab1xy",loc2-loc1) == 0, "wrong match");
	char s2[] = "!@ab2xy";
	r = step(s2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "ab2xy",loc2-loc1) == 0, "wrong match");
	char s3[] = "!@ab3xy";
	r = step(s3, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "ab3xy",loc2-loc1) == 0, "wrong match");
	char s4[] = "!@ab4xy";
	r = step(s4, p);
	mu_assert(r == 0, "Failure matching RE");
	
	if (p) free(p);
	return 0;
}

char *
step_cc_range_neg() {
	char *p = compile("ab[^0-3]xy", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "!@#ab0xyz";
	int r = step(s, p);
	mu_assert(r == 0, "Failure matching RE");
	char s1[] = "!@ab1xyz";
	r = step(s1, p);
	mu_assert(r == 0, "Failure matching RE");
	char s2[] = "!@ab2xy";
	r = step(s2, p);
	mu_assert(r == 0, "Failure matching RE");
	char s3[] = "!@ab3xy";
	r = step(s3, p);
	mu_assert(r == 0, "Failure matching RE");
	char s4[] = "!@ab4xy";
	r = step(s4, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"ab4xy",loc2-loc1)==0, "wrong match");
	
	if (p) free(p);
	return 0;
}

char *
step_cc_multiple() {
	char *p = compile("[0-1][abc]$", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abc0bdefghi0b";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "0b", loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == '\0', "wrong place for loc2");
	
	if (p) free(p);
	return 0;
}

char *
all_tests() {
	mu_suite_start();
	
	mu_run_test(step_cc_normal);
	mu_run_test(step_cc_neg);
	mu_run_test(step_cc_range);
	mu_run_test(step_cc_range_neg);
	mu_run_test(step_cc_multiple);
	
	return 0;
}

RUN_TESTS(all_tests);
