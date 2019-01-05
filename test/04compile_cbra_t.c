#include <regexpr.h>
#include <string.h>
#include <defs.h>
#include "minunit.h"


typedef struct {
	char tok;
	char ch;
} cchar_t;

char *
compile_cbra_balanced() {
	char re[] = "abc\\(xyz123\\)HJS";
	char *p = compile(re, 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	mu_assert(ncbra == 1, "Incorrect number of subexpressions");
	//printf("%s\n", re);
	cchar_t *cp = (cchar_t *)p;
	int i;
	for (i = 0;cp->tok != CEOF; cp++,i++) {
		//printf("%c", cp->ch);
		switch ( cp->tok ) {
			case CCHR:
				mu_assert(cp->ch == re[i], "incorrect compiled CCHR");
				break;
			case CBRA:
			case CKET:
				mu_assert(cp->ch == 0, "incorrect bracket number");
				i++;
				break;
		}
	}
	//printf("\n");
	if ( p ) free(p);
	return 0;
}

char *
compile_cbra_many_balanced() {
	char re[] = "abc\\(xyz\\)123\\(HJS\\)vfa";
	char *p = compile(re, 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	mu_assert(ncbra == 2, "Incorrect number of subexpressions");
	//printf("%s\n", re);
	cchar_t *cp = (cchar_t *)p;
	int i, j;
	for (i = 0,j = 0;cp->tok != CEOF; cp++,i++) {
		//printf("%c", cp->ch);
		switch ( cp->tok ) {
			case CCHR:
				mu_assert(cp->ch == re[i], "incorrect compiled CCHR");
				break;
			case CBRA:
				mu_assert(cp->ch == j, "incorrect bracket number");
				i++;
				break;
			case CKET:
				mu_assert(cp->ch == j, "incorrect bracket number");
				i++;j++;
				break;
		}
	}
	//printf("\n");
	if ( p ) free(p);
	return 0;
}

char *
compile_cbra_left_unbalanced() {
	char re[] = "abc\\(xyz123HJS";
	char *p = compile(re, 0, 0);
	mu_assert(p == 0, "Failure compiling RE");
	mu_assert(regerrno == 42, "incorrect regerrno value");
	
	if (p) free(p);
	return 0;
}

char *
compile_cbra_right_unbalanced() {
	char re[] = "abc\\(xyz\\)123\\)HJS";
	char *p = compile(re, 0, 0);
	mu_assert(p == 0, "Failure compiling RE");
	mu_assert(regerrno == 42, "incorrect regerrno value");
	if (p) free(p);
	return 0;
}

char *
compile_cbra_too_many() {
	char re[] = "\\(1\\)X\\(2\\)Y\\(3\\)Z\\(4\\)W\\(5\\)U\\(6\\)J\\(7\\)P\\(8\\)Q\\(9\\)N\\(10\\)";
	char *p = compile(re, 0, 0);
	mu_assert(p == 0, "Failure compiling RE");
	mu_assert(regerrno == 43, "incorrect regerrno value");
	if (p) free(p);
	return 0;
}

char *
compile_bslash_number() {
	char re[] = "\\1\\2\\3\\4\\5\\6\\7\\8\\9";
	char *p = compile(re, 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	cchar_t *cp = (cchar_t *)p;
	int i;
	for (i = 0 ;cp->tok != CEOF; cp++,i++) {
		mu_assert(cp->tok == CBACK, "Incorrect token found");
		mu_assert(cp->ch == i, "incorrect digit found");
	}
	
	if (p) free(p);
	return 0;
}

char *
compile_bslash_number_0 () {
	char re[] = "\\0";
	char *p = compile(re, 0, 0);
	mu_assert(p == 0, "Failure compiling RE");
	mu_assert(regerrno == 25, "Digit out of range");
	if (p) free(p);
	return 0;
}

char *
all_tests() {
	mu_suite_start();
	
	mu_run_test(compile_cbra_balanced);
	mu_run_test(compile_cbra_many_balanced);
	mu_run_test(compile_cbra_left_unbalanced);
	mu_run_test(compile_cbra_right_unbalanced);
	mu_run_test(compile_cbra_too_many);
	mu_run_test(compile_bslash_number);
	mu_run_test(compile_bslash_number_0);
	
	return 0;
}

RUN_TESTS(all_tests);
