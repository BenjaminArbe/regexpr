#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include "dbg.h"
#include "regexpr.h"
#include "defs.h"

#ifdef __cplusplus
extern "C" {
#endif

bool bcf;
int ncbra;
int reglength, regerrno;

static char *ep;
static char *sp;
static char cbracket[NCBRA];
static char *cbracketp;


static int compile_groups(int c);

char *
compile ( const char *instring, char *expbuf, char *endbuf) {
	check ( instring, "instring is invalid (NULL)");
	bcf = false;
	
	bool inUseMalloc = false;
	if ( !expbuf ) {
		expbuf = (char *)malloc(ESIZE);
		check (expbuf, "malloc: could'n allocate space");
		inUseMalloc = true;
		endbuf = expbuf + ESIZE;
		memset(expbuf, 0, ESIZE);
	}

	int c;
	sp = (char *)instring;
	ep = expbuf;
	char *lastep = 0;
	ncbra = 0;
	cbracketp = &cbracket[0];
	regerrno = reglength = 0;

	if ( *sp == '\0' ) {
		if ( inUseMalloc && expbuf )
			free (expbuf);
		regerrno = 41;
		reglength = 0;
		goto error;
	}
	
	if ( *sp == '^' ) { bcf = true; sp++; }
	
	while (true) {
		// sucessful terminating condition:
		if ( (c = *sp++) == '\0' ) {
			// Before we quit, check cbracket balance
			if ( cbracketp != cbracket ) {
				regerrno = 42;
				reglength = 0;
				return 0;
			}
			// Now the setup to quit
			*ep++ = CEOF;
			*ep = 0;
			reglength = ep - expbuf;
			//return (inUseMalloc ? expbuf : ep);
			if ( inUseMalloc )
				return expbuf;
			else
				return ep;
		}
		
		// Buffer overflow check:
		if ( ep >= endbuf ) {
			regerrno = 50;
			if ( inUseMalloc && expbuf ) 
				free (expbuf);
			goto error;
		}
		
		// Remember ep if not a RE *
		if ( c != '*' ) lastep = ep;
		
		switch (c) {
			case '.':	*ep++ = CDOT; continue;
			
			case '*':	
				if ( lastep == 0 || *lastep == CBRA || *lastep == CKET )
					goto defchar;
				*lastep |= CSTAR;
				continue;
			// $ is considered normal char if is in the middle of REGEXP
			// $ is CDOL only before the terminating '\0' 
			case '$':
				if ( *sp != '\0' ) goto defchar;
				*ep++ = CDOL;	
				continue;
			
			// '\' allows the use of especial chars[*.$]
			// as normal chars. Exceptions are ( ) and \1,
			// \2, ... (groups of RegExps as substrings)
			case '\\':	
				c = *sp++;
				int ret = compile_groups(c);
				if ( ret == 1 ) continue;
				else if ( ret == -1 ) goto error;
				*ep++ = CCHR;
				*ep++ = c;
				continue;
			
defchar:
			default:
				*ep++ = CCHR;
				*ep++ = c;
		}	// end of switch
			
	} 	// end of while
	

error:
	return 0;
}

/* Function: compile_groups
 *   Do processing for REGEXP groups
 *	
 * input: processing char one past the '\'
 * returns:
 *		1 found a (, ), or 1, 2, ... NBRA
 *		0 allow normal char processing no errors
 *		-1 found an error, specified by regerrno 
 */
static int
compile_groups(int c) {
	switch (c) {
		case '(':
			if ( ncbra >= NCBRA ) {
				regerrno = 43;	// Too many \(
				reglength = 0;
				return -1;
			}
			*cbracketp++ = ncbra;
			*ep++ = CBRA;
			*ep++ = ncbra++;
			return 1;

		case ')':
			// This block takes care of right unbalance
			if ( cbracketp <= cbracket ) {
				regerrno = 42;	// Unmatched \)
				reglength = 0;
				return -1;
			}
			*ep++ = CKET;
			*ep++ = *--cbracketp;
			return 1;
		
		case '1': case '2' : case '3' : case '4' :
		case '5': case '6': case '7': case '8': case '9':
			*ep++ = CBACK;
			*ep++ = c - '1';
			return 1;
		
		default:
			return 0;
	}
}
