#include <regexpr.h>
#include <string.h>
#include <defs.h>
#include "minunit.h"

int nbra, regerrno, reglength;
char *loc1, *loc2, *locs;

char *
step_bcf() {
	char regexp[] = "^.*$";
	char *cp = compile (regexp, 0, 0);
	char s[] = "0123456789";
	int ret = step(s, cp);
	if ( ret ) {
		//printf("%s\n", loc1 );
		mu_assert(*loc2 == '\0', "loc2 didn't match end of string");
		mu_assert(strcmp(s, loc1) == 0, "loc1 didn't match start of string");
	}
	if ( cp ) free(cp);
	return 0;
}

char *
step_bcf1()  {
	char *cp = compile("^123", 0, 0);
	char s[] = "abc123trs";
	int ret = step(s, cp);
	mu_assert( ret == 0, "incorrect match");
	char s1[] = "12bctrs";
	ret = step(s1, cp);
	mu_assert( ret == 0, "incorrect match");
	char s2[] = "123abc123trs123";
	ret = step(s2, cp);
	mu_assert( ret == 1, "incorrect unmatch");
	mu_assert( strncmp(loc1, s2, loc2-loc1) == 0, "loc1 pointing wrong match");
	
	return 0;
}

char *
step_char_match() {
	char regexp[] = "abc";
	char *cp = compile(regexp, 0, 0);
	char s[] = "0123qweabczxc4";
	int ret = step(s, cp);
	if ( ret ) {
		mu_assert(*loc2 == 'z', "loc2 wrong match.");
		mu_assert(strncmp(loc1, "abc", loc2-loc1) == 0,
			"loc1 points wrong match");
	}
	if ( cp ) free(cp);
	return 0;
}

char *
step_dot_match() {
	char regexp[] = "a.b";
	char *cp = compile(regexp, 0, 0);
	char s[] = "0123xb74aca09!azbkmnsaxb";
	int ret = step(s, cp);
	if ( ret ) {
		mu_assert(*loc2 == 'k', "loc2 wrong match");
		mu_assert(strncmp(loc1, "azb", loc2-loc1) == 0,
			"loc1 points wrong match");
	}
	if ( cp ) free(cp);
	return 0;
}

char *
step_dollar() {
	char regexp[] = "a$vd$";
	char *cp = compile(regexp, 0, 0);
	//printf("%s\n", cp);
	char s[] = "wlda$vd";
	int ret = step(s, cp);
	if ( ret ) {
		mu_assert(*loc2 == '\0', "loc2 wrong match");
		mu_assert(strcmp(loc1, "a$vd") == 0, "loc1 didn't match");
	}
	if ( cp ) free(cp);
	return 0;
}

char *
step_multiple_star() {
	char regexp[] = "a*12b*34*c";
	char *cp = compile(regexp, 0, 0);
	static char s[] = "xbghfaaahdggaaaaa12bb012aaa1234dgd";
	int ret = step(s, cp);
	if ( ret ) {
		mu_assert(strncmp(loc1, "aaa1234",loc2-loc1) == 0, "Incorrect match");
		mu_assert(*loc2 == 'd', "Incorrectly loc2 pointing");
	}
	static char s1[] = "xbghfaaahdggaaaaa12bb012aaa1234cgd";
	ret = step(s1, cp);
	if ( ret ) {
		mu_assert(strncmp(loc1, "aaa1234c",loc2-loc1) == 0, "Incorrect match");
		mu_assert(*loc2 == 'g', "Incorrectly loc2 pointing");
	}
	static char s2[] = "xbghfaaahdgg12bb3c012aaa1234cgd";
	ret = step(s2, cp);
	if ( ret ) {
		mu_assert(strncmp(loc1, "12bb3c",loc2-loc1) == 0, "Incorrect match");
		mu_assert(*loc2 == '0', "Incorrectly loc2 pointing");
	}
	static char s3[] = "xbgaaa12bb3c012xxvvkkkaaa12bb3c3";
	ret = step(s3, cp);
	if ( ret ) {
		mu_assert(strncmp(loc1, "aaa12bb3c",loc2-loc1) == 0, "Incorrect match");
		mu_assert(*loc2 == '0', "Incorrectly loc2 pointing");
	}
	if ( cp ) free(cp);
	return 0;
}

char *
step_successive_matches() {
	char regexp[] = "abc";
	char *cp = compile(regexp, 0, 0);
	static char s[] = "xyabc111abc2222abc";
	step(s, cp);
	mu_assert(*loc2 == '1', "incorrect first match");
	step(loc2, cp);
	mu_assert(*loc2 == '2', "incorrect second match");
	step(loc2, cp);
	mu_assert(*loc2 == '\0', "incorrect third match");
	if ( cp ) free(cp);
	return 0;
}

char *
step_successive_matches_star() {
	char *cp = compile("cc*:", 0, 0);
	static char s[] = "00c:111cc:222ccccc:333";
	step(s, cp);
	mu_assert(*loc2 == '1', "incorrect first match");
	step(loc2, cp);
	mu_assert(*loc2 == '2', "incorrect second match");
	step(loc2, cp);
	mu_assert(*loc2 == '3', "incorrect third match");
	if ( cp ) free(cp);
	return 0;
}

char *
all_tests() {
	mu_suite_start();
	
	mu_run_test(step_bcf);
	mu_run_test(step_bcf1);
	mu_run_test(step_char_match);
	mu_run_test(step_dot_match);
	mu_run_test(step_dollar);
	mu_run_test(step_multiple_star);
	mu_run_test(step_successive_matches);
	mu_run_test(step_successive_matches_star);
	
	return 0;
}


RUN_TESTS(all_tests);
