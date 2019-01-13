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
#include <string.h>
#include "regexpr.h"
#include "defs.h"

#ifdef __cplusplus
extern "C" {
#endif

char *loc2, *locs;
static int m, len;

static bool stringchr(char *s, char c);
static void getrange(const char *s);

int
advance(const char *string, const char *expbuf) {
	char *lp = (char *)string;
	char *lpmax = lp + strlen(lp);
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
				if ( lp > lpmax ) return 0;
				loc2 = lp;
				return 1;
			case CBRA:
				braslist[(int)*ep++] = lp;
				continue;
			case CKET:
				braelist[(int)*ep++] = lp;
				continue;
			case CBACK: {
				char *sp = braslist[(int)*ep];
				int count = braelist[(int)*ep++] - sp;
				if ( strncmp(sp, lp, count) == 0 ) {
					lp += count;
					continue;
				}
				return 0;
			}
			case CCL:
				if ( stringchr(ep, *lp++) ) { ep += *ep+1; continue; }
				else return 0;
			case NCCL:
				if ( ! stringchr(ep, *lp++) ) { ep += *ep+1; continue; }
				else return 0;
			
			case CCL|CSTAR:
				curlp = lp;
				while ( stringchr(ep, *lp++) ) ;
				ep += *ep+1;
				goto star;
			case NCCL|CSTAR:
				curlp = lp;
				while ( ! stringchr(ep, *lp++) ) ;
				ep += *ep+1;
				goto star;
			case CBACK|CSTAR: {
				char *sp = braslist[(int)*ep];
				int count = braelist[(int)*ep++] - sp;
				curlp = lp; 
				while ( strncmp(sp, lp, count) == 0 ) lp += count;
				while ( lp >= curlp ) { // back up and try to match again
					if ( advance(lp, ep) ) return 1;
					lp -= count;
				}
				return 0;
			}
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
			
			case CCHR|CRPT: {
				char c = *ep++;
				getrange(ep);
				while (m--)
					if ( *lp++ != c ) return 0;
				curlp = lp;
				while (len--)
					if ( *lp++ != c ) break;
				if ( len < 0 ) lp++;	
				ep += 2;	// skip m and n
				goto star;
			}
			case CDOT|CRPT:
				getrange(ep);
				while (m--)
					if ( *lp++ == '\0' ) return 0;
				curlp = lp;
				while (len--)
					if ( *lp++ == '\0' ) break;
				if ( len < 0 ) lp++;
				ep += 2;
				goto star;
			case CBACK|CRPT: {
				char *sp = braslist[(int)*ep];
				int count = braelist[(int)*ep++] - sp;
				getrange(ep);
				while (m--) {
					if ( strncmp(sp, lp, count) != 0 ) return 0;
					lp += count;
				}
				curlp = lp;
				while (len--) {
					if ( strncmp(sp, lp, count) != 0 ) break;
					lp += count;
				}
				if ( len < 0) lp++;
				ep += 2;
				goto star;
			}
				
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

static bool
stringchr(char *s, char c) {
	if ( c == '\0' ) return true;
	size_t len = (size_t)*s++;
	while (len--) 
		if ( *s++ == c ) return true;
	return false;
}

static void
getrange(const char *s) {
	m = *s++;
	len = *s - m;
}

#ifdef __cplusplus
}
#endif
