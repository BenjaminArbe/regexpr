#include <regexpr.h>
#include <defs.h>
#include "minunit.h"

typedef struct {
	char tok;
	char ch;
} cchar_t;

char *
compile_incomplete_cc() {
	char *p = compile("abd[", 0, 0);
	mu_assert(p == 0, "Failure compiling RE");
	mu_assert(regerrno == 49, "Missing ]");
	p = compile("abd[^", 0, 0);
	mu_assert(p == 0, "Failure compiling RE");
	mu_assert(regerrno == 49, "Missing ]");
	
	if (p) free(p);
	return 0;
}

char *
compile_normal_cc() {
	char re[] = "abd[-*.abcjz]";
	char *p = compile(re, 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	cchar_t *pc = (cchar_t *)p; int i;
	for (i = 0; pc->tok == CCHR; pc++, i++) ;
	char *ptr = (char *)pc;
	mu_assert(*ptr++ == CCL, "Incorrect token found");
	mu_assert(*ptr++ == 8, "Incorrect number of chars in class");
	i++;
	mu_assert(strncmp(ptr, &re[i], 8) == 0, "class char didn't match");
	
	if (p) free(p);
	return 0;
}

char *
compile_normal_neg_cc() {
	char re[] = "abd[^-*.abcjz]";
	char *p = compile(re, 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	cchar_t *pc = (cchar_t *)p; int i;
	for (i = 0; pc->tok == CCHR; pc++, i++) ;
	char *ptr = (char *)pc;
	mu_assert(*ptr++ == NCCL, "Incorrect token found");
	mu_assert(*ptr++ == 8, "Incorrect number of chars in class");
	i++;i++;
	mu_assert(strncmp(ptr, &re[i], 8) == 0, "class char didn't match");
	
	if (p) free(p);
	return 0;
}

char *
compile_normal_exception() {
	char re[] = "[\a]";
	char *p = compile(re, 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	mu_assert(p[0] == CCL, "Incorrect token found");
	mu_assert(p[1] == 1, "Incorrect number of chars in class");
	mu_assert(p[2] == '\a', "Incorrect char in class");
	
	if (p) free(p);
	return 0;
}

char *
compile_range_cc() {
	char re[] = "[a-c]";
	char *p = compile(re, 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	mu_assert(p[0] == CCL, "Incorrect token found");
	mu_assert(p[1] == 3, "Incorrect number of chars in range");
	mu_assert(strncmp(&p[2], "abc", 3)==0, "Incorrect range matched");
	
	if (p) free(p);
	return 0;
}

char *
compile_range_neg_cc() {
	char re[] = "[^a-c]";
	char *p = compile(re, 0, 0);
	mu_assert(p != 0, "Failure compiling RE");
	mu_assert(p[0] == NCCL, "Incorrect token found");
	mu_assert(p[1] == 3, "Incorrect number of chars in range");
	mu_assert(strncmp(&p[2], "abc", 3)==0, "Incorrect range matched");
	
	if (p) free(p);
	return 0;
}

char *
all_tests() {
	mu_suite_start();
	
	mu_run_test(compile_incomplete_cc);
	mu_run_test(compile_normal_cc);
	mu_run_test(compile_normal_neg_cc);
	mu_run_test(compile_normal_exception);
	mu_run_test(compile_range_cc);
	mu_run_test(compile_range_neg_cc);
	
	return 0;
}

RUN_TESTS(all_tests);