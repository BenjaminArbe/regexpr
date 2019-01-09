#include <regexpr.h>
#include <string.h>
#include "minunit.h"

char *
step_cbra() {
	char *p = compile("1*\\(abc\\)", 0, 0);
	mu_assert(p != 0, "Error compiling RE");
	char s[] = "1abchjk2abczx";
	int r = step(s, p);
	mu_assert(r == 1, "No match found");
	mu_assert(*braslist[0] == 'a', "Incorrect starting match");
	mu_assert(*braelist[0] == 'h', "Incorrrect ending match");
	if (p) free(p);
	return 0;
}

char *
step_cbra_multiple() {
	char *p = compile("AA*\\(012\\)BB*\\(3456\\)CC*\\(7890\\)DD*", 0, 0);
	mu_assert(p != 0, "Error compiling RE");
	char s[] = "qwertA012BB3456CCC7890DDDDwertYU";
	int r = step(s, p);
	mu_assert(r == 1, "No match found");
	mu_assert(*braslist[0] == '0', "Incorrect starting match substring 0");
	mu_assert(*braelist[0] == 'B', "Incorrect ending match substring 0");
	mu_assert(*braslist[1] == '3', "Incorrect starting match substring 1");
	mu_assert(*braelist[1] == 'C', "Incorrect ending match substring 1");
	mu_assert(*braslist[2] == '7', "Incorrect starting match substring 2");
	mu_assert(*braelist[2] == 'D', "Incorrect ending match substring 2");
	mu_assert(*loc1 == 'A', "Incorrect starting match");
	mu_assert(*loc2 == 'w', "Incorrect ending match");
	if (p) free(p);
	return 0;
}

char *
step_cbra_star_as_1st_char() {
	char *p = compile("\\(*abc\\)", 0, 0);
	mu_assert(p != 0, "Failure compling RE");
	char s[] = "qwert*abcxyz";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(*braslist[0] == '*', "wrong starting substring");
	mu_assert(*braelist[0] == 'x', "wrong ending substring");
	
	if (p) free(p);
	return 0;
}

char *
step_cket_followed_by_star() {
	char *p = compile("\\(abc\\)*", 0, 0);
	mu_assert(p != 0, "Failure compling RE");
	char s[] = "qwert*abc*xyz";
	int r = step(s, p);
	mu_assert(r == 1, "Failure matching RE");
	mu_assert(*braslist[0] == 'a', "wrong starting substring");
	mu_assert(*braelist[0] == '*', "wrong ending substring");
	
	if (p) free(p);
	return 0;
}


char *
all_tests() {
	mu_suite_start();
	
	mu_run_test(step_cbra);
	mu_run_test(step_cbra_multiple);
	mu_run_test(step_cbra_star_as_1st_char);
	mu_run_test(step_cket_followed_by_star);
	
	return 0;
}

RUN_TESTS(all_tests);
