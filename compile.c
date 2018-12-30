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
	char *sp = (char *)instring;
	char *ep = expbuf;
	char *lastep = 0;
	char bracket[NBRA];
	char *bracketp = &bracket[0];

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
			
defchar:
			default:
				*ep++ = CCHR;
				*ep++ = c;
		}	// end of switch
			
	} 	// end of while
	

error:
	return 0;
}