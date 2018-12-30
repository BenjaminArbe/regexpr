/*
 * Tokens used:
 */
#ifndef _DEFS_H
#define _DEFS_H
#include <stdbool.h>
#define ESIZE	512		// Buffer size used by malloc
#define NBRA	9
 
extern bool bcf;		// Used to detect ^
 
enum { 
	CSTAR = 1,	// * matches 0 or more chars
	CBRA = 2,	
	CCHR = 4,	// Normal character
	CDOT = 8,	// dot matches any char
	CCL = 12,
	NCCL = 16,
	CDOL = 20,	// $ matches end of line
	CEOF = 22,	// Used to signal end of compiled REXP
	CKET = 24,
	CBACK = 36,
};
#endif

	