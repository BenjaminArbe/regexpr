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
static char *lastep;
static char *sp;
static char cbracket[NCBRA];
static char *cbracketp;


static int compile_groups(int);
static int compile_char_class(char *);
static int compile_rpt();

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
	lastep = 0;
	ncbra = 0;
	cbracketp = &cbracket[0];
	regerrno = reglength = 0;

	if ( *sp == '\0' ) { regerrno = 41; goto error; }
	
	if ( *sp == '^' ) { bcf = true; sp++; }
	
	while (true) {
		// sucessful terminating condition:
		if ( (c = *sp++) == '\0' ) {
			// Before we quit, check cbracket balance
			// This block also takes care of the left unbalance
			if ( cbracketp != cbracket ) {
				regerrno = 42; goto error; }
			// Now the setup to quit
			*ep++ = CEOF;
			*ep = 0;
			reglength = ep - expbuf;
			if ( inUseMalloc )
				return expbuf;
			else
				return ep;
		}
		
		// Buffer overflow check:
		if ( ep >= endbuf ) { regerrno = 50; goto error; }
		
		// Here is where we remember 'char tokens' and separate
		// from repetition operators:
		if ( c != '*' &&
				( c != '\\' || *sp != '{' ) ) 
			lastep = ep;
		
		switch (c) {
			case '.':	*ep++ = CDOT; continue;
			
			case '*':	
				// If * is 1st char in REGEXP then it is a normal char
				// * after ( or ) is considered as normal char
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
			case '\\':	{
				c = *sp++;
				int ret = compile_groups(c);
				if ( ret == 1 ) continue;
				else if ( ret == -1 ) goto error;
				
				if ( c == '{' ) {
					ret = compile_rpt();
					if ( ret == 1 ) continue;
					else if (ret == -1 ) goto error;
				}
				*ep++ = CCHR;
				*ep++ = c;
				continue;
			}
			
			case '[': {
				int ret = compile_char_class(endbuf);
				if ( ret == 1 ) continue;
				else if ( ret == -1 ) goto error;
			}
			break;
			
defchar:
			default:
				*ep++ = CCHR;
				*ep++ = c;
		}	// end of switch
			
	} 	// end of while
	
// Note: Before to get here, regerrno should be set.
error:
	reglength = 0;
	if ( inUseMalloc && expbuf )
		free(expbuf);
	return 0;
}

/* Function: compile_char_class
 * 	Do processing of character class groups
 *
 * returns:
 * 	1 means successfully compiled so ready to continue 
 *			compilation
 * 	-1 fail to compile due a syntax error or buffer overrun
 */
static int
compile_char_class(char *ebuf) {
	*ep++ = CCL;
	*ep++ = 0;	
	int count = 0;
	char c = *sp++;
	if ( c == '^' ) { c = *sp++; ep[-2] = NCCL; }
	do {
		if ( c == '\0' ) { // Missing ]
			regerrno = 49; return -1;
		}

		// If '-' is not the first char in the class:
		if ( c == '-' && ep[-1] != 0 ) {
			// If ] comes right after '-' then consider '-'
			// as normal char too;
			if ( (c = *sp++) == ']' ) {
				*ep++ = '-';
				count++;
				break;
			}
			// So we're dealing with a range:
			// copy all chars in the range
			while ( ep[-1] < c - 1 ) {
				*ep = ep[-1] + 1;
				ep++; count++;
				// Check for buffer overflow:
				if ( ep >= ebuf ) {
					regerrno = 11; // Range endpoint too large
					return -1;
				}
			}
		}
		// Here is the normal char processing or 
		// If '-' is the 1st char in the class consider it normal char
		*ep++ = c;
		count++;
		// Check for buffer overflow:
		if ( ep >= ebuf ) {
			regerrno = 11;	 // Range endpoint too large
			return -1;
		}
		
	} while ( (c = *sp++) != ']');
	lastep[1] = count;
	return 1;
}

/* Function: compile_rpt
 *   Do processing for REGEXP repetition operator
 *	
 * input: current processing char
 * returns:
 *		1 found a valid rep op \\{m, n\\}, \\{m,\\} or \\{m\}}
 *		0 allow normal char processing no errors
 *		-1 found an error, specified by regerrno 
 */
static int
compile_rpt() {
	bool bc = false;	// comma indicator
	// If { is 1st char in RE consider normal char
	if ( lastep == NULL ) return 0;
	*lastep |= CRPT;
	int m = 0, n = MAXRPT;
	char c = *sp++;
	while ( c != '\\' ) {
		switch ( c ) {
			case ',':
				if ( bc ) {
					regerrno = 44; // More than two numbers given
					return -1;
				} else {
					bc = true;
					c = *sp++;
				}
				break;
				
			default: {
				int i = 0;
				do {
					if ( c >= '0' && c <= '9' )
						i = 10 * i + c - '0';
					else {
						regerrno = 16; // Bad number
						return -1;
					}
				} while ( (c = *sp++ ) != '\\' && c != ',' );
				if ( i > MAXRPT ) {
					regerrno = 11; 	// Range end point too large
					return -1;
				}
				if ( !bc )  m = i;
				else n = i;
			}
		} // end of switch
	} // end while
	if ( *sp++ != '}' ) {
		regerrno = 45;	/* } expected after \ */
		return -1;
	}
	if ( !bc ) // In case of one number //{m//} write n = m
		n = m;
	if ( n < m ) {
		regerrno = 46;	// 1st exceeds the 2nd #
		return -1;
	}
	*ep++ = m;
	*ep++ = n;
	return 1;
} // end of compile_rpt()


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
				return -1;
			}
			*ep++ = CKET;
			*ep++ = *--cbracketp;
			return 1;
		
		case '0':
			{ regerrno = 25; // \digit out of range
				return -1;
			}
			
		case '1': case '2' : case '3' : case '4' :
		case '5': case '6': case '7': case '8': case '9':
			*ep++ = CBACK;
			*ep++ = c - '1';
			return 1;
		
		default:
			return 0;
	}
}
