/*
 * dbutil.c
 * kvs command-line utility
 * KVS - Key Value Store
 * 12.13.2013 - sju
 */

#include <stdio.h>
#include <stdlib.h>

#include <kvs.h>

#include "test_kvs_node.h"
#include "test_kvs.h"

// Static data
static bool IS_VERBOSE = false;

// Static function prototypes
static void cleanup();
static void exit_program(bool exstat);
static bool run_tests();
static bool run_init();
static void usage();

/* cleanup
 * Performs program cleanup before exit.
 */
static void cleanup()
{
	fflush(stdin);
	fflush(stdout);
	fflush(stderr);

} /* cleanup */


/* exit_program
 * Exit the program, cleanup along the way.
 * @arg bool Exit status (true/false)
 */
static void exit_program(bool exstat)
{
	cleanup();

	exit(exstat ? EXIT_SUCCESS : EXIT_FAILURE);

} /* exit_program */


/* usage
 * Print program usage to stderr.
 */
static void usage()
{
	fprintf(stderr, "\nUtility for KVS: dbutil [-v?] action\n"
		"  Action    Usage                            Description\n"
		"  tests     tests                            Run API tests\n"
		"  init      init                             Initialize a kvs database"
		"\n"
		"  Options:\n"
		"  -v        Print verbose messages\n"
		"  -?        Print this message\n"
		"\n"
		);

} /* usage */


/* run_tests
 * Run API tests
 */
static bool run_tests()
{
	bool ALL_TESTS_OK = true;

	// Always set display on
	xmsg_set_display(true);

	// Plain output for tests
	xmsg_set_mode(mPlain);

	xmsg(0, "Begin all tests.");

	if (! test_kvs_node())
		ALL_TESTS_OK = false;

	if (! test_kvs())
			ALL_TESTS_OK = false;

	if (! ALL_TESTS_OK)
			xmsg(mError, "Some API tests FAILED!");

	return ALL_TESTS_OK;

} /* run_tests */


/* run_init
 * Run kvs database initialization
 */
static bool run_init()
{
	// Always set display on
	xmsg_set_display(true);

	return true;

} /* run_init */


/* main
 * Main entry point to program.
 */
int main(int argc, char **argv)
{
	int c;
	extern int optind;
	extern char *optarg;
	char *action;
	bool exstat = true;

	// Parse options on command line
	while ((c = getopt(argc, argv, "v?")) != -1) {
		switch (c) {
		case 'v':
			IS_VERBOSE = true;
			break;

		case '?':
			usage();
			exit_program(true);

		default:
			usage();
			exit_program(false);
		}
	}

	if (argc <= optind) {
		usage();
		exit_program(false);
	}

	xmsg_init("dbutil");
	xmsg_set_display(IS_VERBOSE);

	action = xstrdup(argv[optind]);

	if (! strcmp(action, "tests")) {
		exstat = run_tests();
		goto cleanup;
	}

	if (! strcmp(action, "init")) {
		exstat = run_init();
		goto cleanup;
	}

	xmsg(mError|mDisplay, "[%s] is not a valid action", action);
	exstat = false;

cleanup:
	xfree(action);
	exit_program(exstat);

}
