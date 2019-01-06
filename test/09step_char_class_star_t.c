#include <regexpr.h>
#include "minunit.h"

char *
step_cc_normal_star() {
	char *p = compile("[abc]*345", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "xyz01abbccc345mnop";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"abbccc345",loc2-loc1)==0, "wrong match");
	char s1[] = "xyz01bbccc345mnop";
	r = step(s1, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"bbccc345",loc2-loc1)==0, "wrong match");
	char s2[] = "xyz01accc345mnop";
	r = step(s2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"accc345",loc2-loc1)==0, "wrong match");
	char s3[] = "xyz01a345mnop";
	r = step(s3, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"a345",loc2-loc1)==0, "wrong match");
	char s4[] = "xyz01345mnop";
	r = step(s4, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"345",loc2-loc1)==0, "wrong match");
	
	if (p) free(p);
	return 0;
}

char *
step_cc_normal_neg_star() {
	char *p = compile("[^abc]*345", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "xyz01abbccc345mnop";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"345",loc2-loc1)==0, "wrong match");
	p = compile("[^abc]*", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"xyz01",loc2-loc1)==0, "wrong match");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"345mnop",loc2-loc1)==0, "wrong match");
	
	if (p) free(p);
	return 0;
}

char *
step_cc_range_star() {
	char *p = compile("[0-3]*xy", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abcde00111222333xyz";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"00111222333xy",loc2-loc1)==0, "wrong match");
	char s1[] = "abcde111222xyz";
	r = step(s1, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"111222xy",loc2-loc1)==0, "wrong match");
	
	p = compile("[0-3][0-3]*", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s2[] = "abc00111444222vvvut1133@#";
	r = step(s2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"00111",loc2-loc1)==0, "wrong match");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"222",loc2-loc1)==0, "wrong match");
	locs = loc2;
	r = step(loc2,p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"1133",loc2-loc1)==0, "wrong match");
	
	if (p) free(p);
	return 0;
}

char *
step_cc_range_neg_star() {
	char *p = compile("[0-3][^0-3]*", 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abc0110444222vvvut14367@#";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"0",loc2-loc1)==0, "wrong match");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"1",loc2-loc1)==0, "wrong match");
	locs = loc2;
	r = step(loc2,p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"1",loc2-loc1)==0, "wrong match");
	locs = loc2;
	r = step(loc2,p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"0444",loc2-loc1)==0, "wrong match");
	locs = loc2;
	r = step(loc2,p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"2",loc2-loc1)==0, "wrong match");
	locs = loc2;
	r = step(loc2,p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"2",loc2-loc1)==0, "wrong match");
	locs = loc2;
	r = step(loc2,p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"2vvvut",loc2-loc1)==0, "wrong match");
	locs = loc2;
	r = step(loc2,p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"14",loc2-loc1)==0, "wrong match");
	locs = loc2;
	r = step(loc2,p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"367@#",loc2-loc1)==0, "wrong match");
	//locs = loc2;
	//printf("%c\n", *loc2);
	//r = step(loc2,p);
	//mu_assert(r == 1, "Failure matching RE");
	//printf("%c\n", *loc2);
	printf("%d, %d\n", strlen(s), loc2-s);
	printf("%c\n", s[strlen(s)]);
	printf("%c\n", *(loc2-1));
	if (p) free(p);
	return 0;
}

char *
all_tests() {
	mu_suite_start();
	
	mu_run_test(step_cc_normal_star);
	mu_run_test(step_cc_normal_neg_star);
	mu_run_test(step_cc_range_star);
	mu_run_test(step_cc_range_neg_star);
	
	return 0;
}

RUN_TESTS(all_tests);

