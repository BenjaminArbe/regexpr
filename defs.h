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
	CCHR = 1,	// Normal character
	CDOT = 2,	// dot matches any char
	CCL  = 4,	// Character classes [a-b]
	NCCL = 8,	// Negated character classes [^a-b]
	CBACK = 10,	// / backslash

	CSTAR = 32,	// * matches 0 or more chars
	CEOF = 48,	// Used to signal end of compiled REXP
	CRPT = 64,	// \{m,n\}
	CKET = 80,	// ) bracket
	CDOL = 96, // $ matches end of line
	CBRA = 112,	// ( bracket
};
#endif

	