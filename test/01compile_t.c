#include <regexpr.h>
#include <string.h>
#include <defs.h>
#include "minunit.h"

char instring[10];
char expbuf[20];

typedef struct {
	char tok;
	char ch;
} cchr_t;

char*
compile_expbuf_endbuf() {
	strcpy(instring, "abcdefgh");
	char *endbuf = expbuf+4;
	char *ret = compile(instring, expbuf, endbuf);
	mu_assert( ret == 0, "RE cannot fit in (endbuf - expbuf)");
	mu_assert( regerrno == 50, "regerrno not set or has wrong value");
	
	regerrno = 0;
	int i;
	for (i=20,endbuf = expbuf + 20; !regerrno; endbuf--,i--) {
		ret = compile(instring, expbuf, endbuf);
		if ( !regerrno ) {
			mu_assert( ret != 0, "RE fit in (endbuf - expbuf)");
			mu_assert( regerrno == 0, "regerrno improper value.");
		} else {
			mu_assert( ret == 0, "RE cannot fit in (endbuf - expbuf)");
			mu_assert( regerrno == 50, "regerrno not set or has wrong value");
		}
		//printf("%d (%p)", i,ret);
		//printf("%d ", i);
	}
	return 0;
}

char *
compile_malloc_use() {
	regerrno = 0;
	strcpy (instring, "abcdefgh");
	char *p = compile(instring, 0, 0);
	mu_assert( p != 0, "RE is larger than ESIZE=512");
	mu_assert( regerrno == 0, "improper value for regerrno");
	free (p);
	
	char *regstr = (char *) malloc(ESIZE+5);
	memset(regstr, 'a', ESIZE+5);
	p = compile(regstr, 0, 0);
	mu_assert( p == 0, "RE is smaller than ESIZE");
	mu_assert( regerrno == 50, "regerrno isn't properly set");
	return 0;
}

char *
compile_match_defchar() {
	strcpy(instring, "abcdefgh");
	char *p = compile(instring, 0, 0);
	mu_assert(reglength == (int)strlen(p), "reglength isn't right");
	mu_assert(p[reglength-1] == CEOF, "Missing terminating CEOF");
	
	int i;
	cchr_t *cp = (cchr_t *)p;
	for (i = 0 ; cp->tok != CEOF; cp++, i++) {
		mu_assert(cp->tok == CCHR, "token isn't CCHAR");
		mu_assert(cp->ch == instring[i], "char didn't match");
		//printf ("%c ", cp->ch);
	}
	//printf ("%d\n", reglength);
	//printf ("%s\n", p);
	if (p) free(p);
	return 0;
}

char *
compile_match_defchar_line() {
	char instring[] = "^!@#%&?<>,123  456 7$";
	char *p = compile(instring, 0 ,0);
	mu_assert(reglength == (int)strlen(p), "reglength isn't right");
	mu_assert(p[reglength-1] == CEOF, "Missing terminating CEOF");
	mu_assert(p[reglength-2] == CDOL, "Missing CDOL");
	int i;
	cchr_t *cp = (cchr_t *)p;
	
	for (i = 1 ; cp->tok != CDOL; cp++, i++) {
		mu_assert(cp->tok == CCHR, "token isn't CCHAR");
		mu_assert(cp->ch == instring[i], "char didn't match");
		//printf ("%c ", cp->ch);
	}
	
	//printf("%s\n", p);
	if (p) free(p);
	return 0;
}

char *
compile_match_cdot() {
	char instring[] = "abc.012.34.56";
	char *p = compile(instring, 0, 0);
	mu_assert(reglength == (int)strlen(p), "reglength isn't right");
	mu_assert(p[reglength-1] == CEOF, "Missing terminating CEOF");
	int i;
	cchr_t *cp = (cchr_t *)p;
	for (i = 0 ; cp->tok != CEOF; i++) {
		if ( cp->tok == CDOT ) {
			cp = (cchr_t *)&cp->ch;
			//printf("%c, %d ", cp->ch, i);
			continue;
		}
		mu_assert(cp->tok == CCHR, "token isn't CCHAR");
		mu_assert(cp->ch == instring[i], "char didn't match");
		//printf ("%c ", cp->ch);
		cp++;
	}
	
	//printf("%s\n", p);
	if (p) free(p);
	return 0;
}

char *
compile_match_cstar() {
	char instring[] = "ab.c*de*.*";
	char *p = compile(instring, 0, 0);
	mu_assert(reglength == (int)strlen(p), "reglength isn't right");
	mu_assert(p[reglength-1] == CEOF, "Missing terminating CEOF");
	int i;
	cchr_t *cp = (cchr_t *)p;
	for (i = 0 ; cp->tok != CEOF; i++) {
		if ( cp->tok == CCHR ) {
			mu_assert(cp->ch == instring[i], "char didn't match");
		} else if ( cp->tok == (CCHR|CSTAR) ) {
			mu_assert(cp->ch == instring[i++], "char didn't match");
		} else if ( cp->tok == CDOT || cp->tok == (CDOT|CSTAR) ) {
			cp = (cchr_t *)&cp->ch; continue;
		}
		//printf ("%c ", cp->ch);
		cp++;
	}
	
	//printf("%s\n", p);
	if (p) free(p);
	return 0;
}

char *
all_tests() {
	mu_suite_start();
	
	mu_run_test(compile_expbuf_endbuf);
	mu_run_test(compile_malloc_use);
	mu_run_test(compile_match_defchar);
	mu_run_test(compile_match_defchar_line);
	mu_run_test(compile_match_cdot);
	mu_run_test(compile_match_cstar);
	
	return 0;
}


RUN_TESTS(all_tests);