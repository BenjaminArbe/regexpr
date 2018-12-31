#include <regexpr.h>
#include <string.h>

#include <defs.h>
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
all_tests() {
	mu_suite_start();
	
	mu_run_test(step_cbra);
	
	return 0;
}

RUN_TESTS(all_tests);
