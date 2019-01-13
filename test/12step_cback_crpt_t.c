#include <regexpr.h>
#include "minunit.h"

char *
rpt_back_one_parameter() {
	char *p = compile("\\(ab\\).*\\1\\{3\\}",0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "abxyzabababucyaabccczaabccccuababdc";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, "abxyzababab", loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == 'u', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 0, "Failure matching RE");
	
	char s1[] = "abxyzabababucyaabccczaabccccuababab";
	r = step(s1, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1, s1, loc2-loc1) == 0,"wrong match");
	mu_assert(*loc2 == '\0', "wrong loc2 value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 0, "Failure matching RE");
	
	if (p) free(p);
	return 0;
}

char *
all_tests() {
	mu_suite_start();
	
	mu_run_test(rpt_back_one_parameter);
	//mu_run_test(rpt_back_one_comma_parameter);
	//mu_run_test(rpt_back_comma_one_parameter);
	//mu_run_test(rpt_back_two_parameter);
	
	return 0;
}

RUN_TESTS(all_tests);