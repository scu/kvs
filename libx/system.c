/* system.c
 * 12.13.2013 sju
 * libx system functions
 */

#define LIBX_INTERNAL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libx.h"

/* xalloc
 * Allocate memory.
 * @arg size_t Size of memory.
 */
void * xalloc (size_t size)
{
	register void *value = malloc(size);
	memset(value, 0x00, size);

	if (value == 0)
    	xmsg(mError|mSystem, "failed to create virtual memory");

	return value;

} /* xalloc */

/* xfree
 * Free memory.
 * @arg void Pointer to value being released
 */
void xfree (void * value)
{
	if (! value) {
		return;
	}

	free(value);

	value = NULL;

} /* xfree */
