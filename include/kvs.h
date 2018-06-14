/*
 * kvs.h
 * Main header for kvs library
 */

#ifndef __KVS_H
#define __KVS_H

#define _GNU_SOURCE

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
extern "C" {
#endif // C

#define DEFAULT_KVS_FILENAME "fb.db"

// libkvs requires libx
#include <libx.h>
#include <libkvs/node.h>

/* DbUnit
 * Holds status, information and stats about a repository
 */
typedef struct DbUnit_t {
	const char *name;
	bool is_initialized;
	bool is_open;
} DbUnit;

/* kvs_init
 * Initialize a kvs
 */
bool kvs_unit_init (const char *dbunit_filename);

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
}
#endif // C

#endif //__KVS_H
