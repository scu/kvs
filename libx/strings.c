/* strings.c
 * 12.13.2013 sju
 * libx ordinary string functions
 * NOTE: see bstrlib.txt for details on bstring
 */

#define LIBX_INTERNAL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libx.h"


/* xstrdup
 * Duplicate s.
 * @returns pointer to duplicate string
 */
char * xstrdup (const char * s)
{
	if (! s) {
		return (char *)NULL;
	}

	return (char *)strdup(s);

} /* xstrdup */
