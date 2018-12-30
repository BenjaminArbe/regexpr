#ifndef __REGEXPR_H
#define __REGEXPR_H

#ifdef __cplusplus
extern "C" {
#endif
/*
 * External variables:
 * See man page for usage
 */
extern int nbra, regerrno, reglength;
extern char *loc1, *loc2, *locs;

int step(const char *string, const char *expbuf);
int advance(const char *string, const char *expbuf);
char *compile(const char *instring, char *expbuf, char *endbuf);
#ifdef __cplusplus
}
#endif

#endif
