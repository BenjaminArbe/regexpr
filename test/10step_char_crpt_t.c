#include <regexpr.h>
#include "minunit.h"

char *
rpt_char_one_parameter() {
	char *p = compile("a\\{3\\}bc",0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abcxaabcyaaabczaaaabcu";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "aaabc", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 = 'z', "wrong loc2 value");
	
	if (p) free(p);
	return 0;
}


char *
all_tests() {
	mu_suite_start();
	
	mu_run_test(rpt_char_one_parameter);
	
	return 0;
}

RUN_TESTS(all_tests);
