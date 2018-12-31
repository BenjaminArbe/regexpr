#include <regexpr.h>
#include <string.h>
#include <defs.h>
#include "minunit.h"

int regerrno, reglength, nbra;

typedef struct {
	char tok;
	char ch;
} cchr_t;

char *
compile_back_normal() {
	char re[] = "abc\d";
	char *p = compile(re, 0, 0);
	mu_assert( p != 0, "Failure while matching");
	cchr_t *cp = (cchr_t *)p;
	int i;
	for (i = 0;cp->tok != CEOF; cp++,i++) {
		mu_assert(cp->tok == CCHR, "Incorrect token match");
		mu_assert(cp->ch == re[i], "Incorrect char match");
	}
	
	if ( p ) free (p);
	return 0;
}

char *
compile_back_dot() {
	char re[] = "abc\.def";
	char *p = compile(re, 0, 0);
	mu_assert( p != 0, "Failure while matching");
	printf("%s\n", re);
	printf("%s\n", p);
	cchr_t *cp = (cchr_t *)p;
	int i;
	for (i = 0;cp->tok != CEOF; cp++,i++) {
		mu_assert(cp->tok == CCHR, "Incorrect token match");
		mu_assert(cp->ch == re[i], "Incorrect char match");
	}
	
	if ( p ) free (p);
	return 0;
}

char *
all_tests() {
	mu_suite_start();
	
	mu_run_test(compile_back_normal);
	mu_run_test(compile_back_dot);
	
	return 0;
}

RUN_TESTS(all_tests);