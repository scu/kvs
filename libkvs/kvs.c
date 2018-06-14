/* kvs.c
 * kvs main library entry points
 * 12.18.2013
 */

#include <kvs.h>

/* kvs_init
 * Initialize a kvs
 */
bool kvs_init (const char *kvs_filename)
{
	const char *filename = kvs_filename ? kvs_filename : DEFAULT_KVS_FILENAME;


	return true;

} /* kvs_init */
