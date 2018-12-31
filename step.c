/**
 * Function: step:
 *
 * Description:
 *
 * Match the argument string against the compiled REGEXP
 * 
 * Parameters:
 * string is the one in which we scan for a match
 * expbuf is the Regular expresion we want to match
 *
 * Return:
 * On success returns non-zero value
 * If fails to find a match returns 0
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "regexpr.h"
#include "defs.h"

#ifdef __cplusplus
extern "C" {
#endif

char *loc1;
char *braslist[NCBRA];
char *braelist[NCBRA];

int
step(const char *string, const char *expbuf) {
	char c;
	char *p1 = (char *)string;
	char *p2 = (char *)expbuf;
	loc1 = 0;
	
	memset(braslist, 0, sizeof(braslist));
	memset(braelist, 0, sizeof(braelist));
	
	if (bcf) {
		//printf("executing...\n");
		loc1 = p1;
		return advance(p1, p2);
	}
	/*
	 * fast check for first character:
	 */
	if ( *p2 == CCHR ) {
		c = p2[1];
		do {
			if ( *p1 != c ) continue;
			if ( advance(p1, p2) ) {
				loc1 = p1;
				return 1;
			}
		} while ( *p1++ );
		return 0;
	}

	/*
	 * Regular algorithm
	 */
	do {
		if ( advance(p1, p2) ) {
			loc1 = p1;
			return 1;
		}
	} while ( *p1++ );
	return 0;
}

#ifdef __cplusplus
}
#endif
