/**
 * Function: advance:
 *
 * Description:
 *
 * Try to match the next thing in the dfa:
 * The function advance has the same arguments and side
 * effects as step, but it always restricts matches to 
 * the beginning of the string.
 * 
 * Parameters:
 * string is the one in which we scan for a match
 * expbuf is the Regular expresion we want to match
 *
 * Return:
 * On success returns non-zero value
 * If fails to find a match returns 0
 */
#include <stdbool.h>
#include "regexpr.h"
#include "defs.h"

#ifdef __cplusplus
extern "C" {
#endif

char *loc2, *locs;
 
int
advance(const char *string, const char *expbuf) {
	char *lp = (char *)string;
	char *ep = (char *)expbuf;
	char *curlp;
	loc2 = 0; locs = 0;

	while (true)  {
		switch ( *ep++ ) {
			case CCHR:
				if ( *ep++ == *lp++ ) continue;
				return 0;
			case CDOT:
				if ( *lp++ ) continue;
				return 0;
			// CDOL must match terminating string char '\0'
			// in order to succeed.
			case CDOL:
				if ( *lp == '\0' ) continue;
				return 0;
			case CEOF:
				loc2 = lp;
				return 1;
			case CBRA:
				braslist[(int)*ep++] = lp;
				continue;
			case CKET:
				braelist[(int)*ep++] = lp;
				continue;
			
			case CDOT|CSTAR:
				curlp = lp;
				while ( *lp++ )
					;
				goto star;
			case CCHR|CSTAR:
				curlp = lp;
				while ( *lp++ == *ep )
					;
				ep++;
				goto star;
				
star:
		do {
			// By default locs is NULL string search ends
			// if the end of the string is met or locs is set
			// by using locs = loc2
			// locs is used by tools like sed, and ed
			// to avoid an endless loop s/*// too
			if ( --lp == locs ) break;
			// This recursive call allow us to match multiple *
			if ( advance(lp, ep) ) 
				return 1;
		} while (lp > curlp);
		return 0;
		
			default:
				return 0;
				
		} // end of switch
		
	} // end of while
	
} // end of advance

#ifdef __cplusplus
}
#endif
