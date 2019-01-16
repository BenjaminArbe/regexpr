#include <regexpr.h>
#include "minunit.h"

char *
rpt_cclass_one_parameter() {
	char *p = compile("[a-d]\\{3\\}XX*",0 ,0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "012adgXabcXXddddX";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"abcXX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'd', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"dddX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == '\0', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 0, "Failure matching RE");
	
	if (p) free(p);
	return 0;
}

char *
rpt_ncclass_one_parameter() {
	char *p = compile("[^a-d]\\{3\\}XX*",0 ,0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "012adgXfghXXiiiiiX";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"fghXX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'i', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"iiiX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == '\0', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 0, "Failure matching RE");
	
	if (p) free(p);
	return 0;
}

char *
rpt_cclass_one_comma_parameter() {
	char *p = compile("[a-d]\\{3,\\}XX*",0 ,0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "012adgXabcXXddddXaabbccddccbbaaXXX";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"abcXX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'd', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"ddddX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'a', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"aabbccddccbbaaXXX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == '\0', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 0, "Failure matching RE");
	
	if (p) free(p);
	return 0;
}

char *
rpt_ncclass_one_comma_parameter() {
	char *p = compile("[^a-d]\\{3,\\}XX*",0 ,0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "012adgX012XXddddXaabbccddccbbaaYXXX";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"gX012XX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'd', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"YXXX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == '\0', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 0, "Failure matching RE");
	
	if (p) free(p);
	return 0;
}

char *
rpt_cclass_comma_one_parameter() {
	char *p = compile("[a-d]\\{,3\\}XX*",0 ,0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "012adgXabcXXddddXaabbccddccbbaaXXX";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"X",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'a', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"abcXX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'd', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"dddX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'a', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"baaXXX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == '\0', "loc2 wrong value");

	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 0, "Failure matching RE");
	
	if (p) free(p);
	return 0;
}

char *
rpt_ncclass_comma_one_parameter() {
	char *p = compile("[^a-d]\\{,3\\}XX*",0 ,0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "012adgXabcXXddddXaabbccddccbbaaXXX";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"gX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'a', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"XX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'd', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"X",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'a', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"XXX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == '\0', "loc2 wrong value");

	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 0, "Failure matching RE");
	
	if (p) free(p);
	return 0;
}

char *
rpt_cclass_two_parameters() {
	char *p = compile("[a-d]\\{3,5\\}XX*",0 ,0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "012adXabcXXddddXaabbccddccbbaaXXX";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"abcXX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'd', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"ddddX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'a', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"cbbaaXXX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == '\0', "loc2 wrong value");

	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 0, "Failure matching RE");
	
	if (p) free(p);
	return 0;
}

char *
rpt_ncclass_two_parameters() {
	char *p = compile("[^a-d]\\{3,5\\}XX*",0 ,0);
	mu_assert(p != 0, "Failure compiling RE");
	char s[] = "01234XajkXXddddXaabbccddccYYYYYYYXXX";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"01234X",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'a', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"jkXX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == 'd', "loc2 wrong value");
	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(strncmp(loc1,"YYYYYXXX",loc2-loc1)==0, "wrong match");
	mu_assert(*loc2 == '\0', "loc2 wrong value");

	locs = loc2;
	r = step(loc2, p);
	mu_assert(r == 0, "Failure matching RE");
	
	if (p) free(p);
	return 0;
}


char *
all_tests() {
	mu_suite_start();
	
	mu_run_test(rpt_cclass_one_parameter);
	mu_run_test(rpt_ncclass_one_parameter);
	mu_run_test(rpt_cclass_one_comma_parameter);
	mu_run_test(rpt_ncclass_one_comma_parameter);
	mu_run_test(rpt_cclass_comma_one_parameter);
	mu_run_test(rpt_ncclass_comma_one_parameter);
	mu_run_test(rpt_cclass_two_parameters);
	mu_run_test(rpt_ncclass_two_parameters);
	
	return 0;
}

RUN_TESTS(all_tests);