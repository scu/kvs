/*
 * db.c
 * Database access for kvs lib
 * 12.13.2013 sju
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <sqlite3.h>

static sqlite3 *db;

/*
 * kvs_init_database
 * Initialize a kvs database
 */
void kvs_init_database()
{
	printf("initializing database\n");

} /* kvs_init_database */
