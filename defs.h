/*
 * Tokens used:
 */
#ifndef _DEFS_H
#define _DEFS_H
#include <stdbool.h>
#define ESIZE	512		// Buffer size used by malloc
#define NCBRA	9
 
extern bool bcf;		// Used to detect ^
 
enum { 
	CSTAR = 1,	// * matches 0 or more chars
	CBRA = 2,	// ( bracket
	CCHR = 4,	// Normal character
	CDOT = 8,	// dot matches any char
	CCL = 12,	// Character classes [a-b]
	NCCL = 16,	// Negated character classes [^a-b]
	CDOL = 20,	// $ matches end of line
	CEOF = 22,	// Used to signal end of compiled REXP
	CKET = 24,	// ) bracket
	CBACK = 36,	// / backslash
};
#endif

	