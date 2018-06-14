/* messages.c
 * 12.13.2013 sju
 * libx messaging
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <execinfo.h>
#include <unistd.h>
#include <time.h>

#ifdef TARGET_LINUX
#include <sys/syscall.h>
#endif

#include "libx.h"

#define MAXBUFSZ 1024
#define MESSAGE_START_SEQ ">>"

// Static data
static FILE *msg_fd = NULL;						// Process-specific message file descriptor
static const char *process_name = NULL;			// Process name
static unsigned int process_id;					// Process id
static bool MESSAGING_INIT = false;				// Messaging initialized flag
static bool DISPLAY_MSGS = false;				// Display messages to stdout
static bool MESSAGING_ON = true;				// Messaging flag
static int TRACE_LEVEL = 0;						// Trace level
static int PERMA_MODE = 0;						// Mode applied to all messages
static unsigned long long MESSAGE_COUNT = 0L;	// Message counter
static char *msg_file_name = NULL;				// Name of the message file

// Static function prototypes
static void write_to_streams (bool display_on, char *fmt, ...);
static void flush_streams (void);
static pid_t xmsg_getpid (void);


/*******************
 * CODE STARTS HERE
 *******************/

/*
 * xmsg_getpid
 * Returns the thread ID or process ID.
 */
static pid_t xmsg_getpid (void)
{
#ifdef TARGET_LINUX
	return (pid_t)syscall(SYS_gettid);
#else
	return getpid();
#endif

} /* xmsg_getpid */

/*
 * print_backtrace
 * Print backtrace.
 */
void print_backtrace(void)
{
    int j, nptrs;
    void *buffer[MAXBUFSZ];
    char **strings;

    if (! DISPLAY_MSGS)
    	return;

    nptrs = backtrace(buffer, MAXBUFSZ);

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        xmsg(mError, "Error: cannot obtain backtrace symbols");
        return;
    }

    for (j = 0; j < nptrs; j++)
        fprintf(stderr, "%s\r\n", strings[j]);

    free(strings);

} /* print_backtrace */

/*
 * xmsg_set_display
 * Setting to "true" means messages will be displayed.
 * @arg bool Display messages? (true/false)
 */
void xmsg_set_display (bool display_msgs)
{
	DISPLAY_MSGS = display_msgs;

} /* xmsg_set_display */

/*
 * xmsg_set_mode
 * Applies "mode" to all subsequent messages.
 * @arg int Mode flag
 */
void xmsg_set_mode (int mode)
{
	PERMA_MODE = PERMA_MODE ^ mode;

} /* xmsg_set_mode */

/*
 * xmsg_set_messaging_flag
 * Sets messaging on/off (default is "on").
 * @arg bool Flag indicates messaging is on/off
 */
void xmsg_set_messaging_flag (bool messaging_on)
{
	MESSAGING_ON = messaging_on;

} /* xmsg_set_messaging_flag */

/*
 * xmsg_set_trace_level
 * Set to TraceLevel 1 through 3.
 * @arg int Trace level
 */
void xmsg_set_trace_level (int trace_level)
{
	if (trace_level < 0 || trace_level > 3) {
		fprintf(stderr, "tsmsg_set_trace_level: trace_level must be between 0 and 3\n");
		return;
	}

	TRACE_LEVEL = trace_level;

} /* xmsg_set_trace_level */

/*
 * xmsg_zero
 * Truncate the message log (but not the aggregate log) to zero.
 */
bool xmsg_zero (void)
{
	if (! msg_file_name) {
		fprintf(stderr, "Error: msg_file_name unknown.\n");
		return false;
	}

	if (! msg_fd) {
		fprintf(stderr, "Error: msg_file_fd unopened.\n");
		return false;
	}

	fclose(msg_fd);

	// Open aggregated msg file name in mode
	msg_fd = fopen(msg_file_name, "w+");
	if (! msg_fd) {
		perror("fopen");
		fprintf(stderr, "failed to open [%s]", msg_file_name);
		return false;
	}

	fclose(msg_fd);

	// Re-open in append mode
	msg_fd = fopen(msg_file_name, "a+");
	if (! msg_fd) {
		perror("fopen");
		fprintf(stderr, "failed to open [%s]", msg_file_name);
		return false;
	}

	return true;

} /* xmsg_zero */

/*
 * xmsg_init
 * Initialize messaging for process.
 * Call only once per PROCESS.  Do not call from each thread.
 * @arg char String representing the name of the process
 * @returns true/false on success/fail
 */
bool xmsg_init (const char *name)
{
	char *ETRACE = NULL;

	// Set globals
	process_name = name;
	process_id = (unsigned int)getpid();

	// Check for ETRACE variable.
	ETRACE = getenv("ETRACE");
	if (ETRACE) {
		if (! strcmp(ETRACE, "Trace1"))
			xmsg_set_trace_level(1);
		else
		if (! strcmp(ETRACE, "Trace2"))
			xmsg_set_trace_level(2);
		else
		if (! strcmp(ETRACE, "Trace3"))
			xmsg_set_trace_level(3);
	}

	// Open the aggregated message file
	if (! msg_fd) {
		// Build aggregated msg file name
		msg_file_name = (char *)malloc(strlen(process_name) + 6);
		if (! msg_file_name) {
			perror("malloc");
			return false;
		}

		sprintf(msg_file_name, "%s.log", process_name);

		// Open aggregated msg file name in append mode
		msg_fd = fopen(msg_file_name, "a+");
		if (! msg_fd) {
			perror("fopen");
			fprintf(stderr, "failed to open [%s]", msg_file_name);
			free(msg_file_name);
			return false;
		}
	}

	MESSAGING_INIT = true;

	// Default to dtstamp
	xmsg_set_mode(mDtstamp);

	xmsg_t(mDtstamp, "%s started", process_name);

	return true;

} /* xmsg_init */

/*
 * xmsg_close
 * Close all log files
 */
void xmsg_close (void)
{
	if (! msg_fd)
		fprintf(stderr, "xmsg_close: unable to close msg file: not open.\n");
	else
		fclose(msg_fd);

} /* xmsg_close */

/*
 * write_to_streams
 * Write to each fd, and display if necessary.
 * @arg bool Flag to indicate whether msgs are to be written to stderr
 * @arg char Format string
 * @arg vararg Variable argument list
 */
static void write_to_streams (bool display_on, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);

	// If display is on, write to that
	if (display_on) {
		vfprintf(stderr, fmt, ap);
	}

	// Write to the process message file
	vfprintf(msg_fd, fmt, ap);

	va_end(ap);

} /* write_to_streams */

/*
 * flush_streams
 * Flushes output streams for message files.
 */
static void flush_streams (void)
{
	fflush(msg_fd);
	fflush(stdout);

} /* flush_streams */

/*
 * xmsg_t
 * Message caller.
 * @arg int Mode flag
 * @arg char Format string
 * @arg vararg Variable argument list
 */
void xmsg_t (int mode, char *fmt, ...)
{
	va_list ap;
	char dtstamp[48];
	char source_name[MAXBUFSZ];
	char *vbuf = NULL;
	bool display_on = false;
	time_t t;

	// Do nothing if messaging is off
	if (! MESSAGING_ON)
		return;

		// Check if messaging has been initialized
	if (! MESSAGING_INIT) {
		fprintf(stderr, "Error: calling xmsg_t before xmsg_init.\n");
		return;
	}

	// Apply the default mode if there is one
	if (PERMA_MODE)
		mode = mode ^ PERMA_MODE;

	// Check trace level
	if ((mode & mTrace1) && (TRACE_LEVEL < 1)) {
		return;
	} else
	if ((mode & mTrace2) && (TRACE_LEVEL < 2)) {
		return;
	} else
	if ((mode & mTrace3) && (TRACE_LEVEL < 3)) {
		return;
	}

	// Build datetime stamp
	if (mode & mDtstamp) {
		time(&t);
		if (mode & mReaddate)
			strftime(dtstamp, 48, "%D %r", localtime(&t));
		else
			strftime(dtstamp, 48, "%Y%m%d%H%M%S", localtime(&t));
	}

	// Build source name
	strcpy(source_name, process_name);

	// Increment the message counter: threadsafely
	MESSAGE_COUNT++;

	display_on = ((mode & mDisplay) || DISPLAY_MSGS) ? true : false;

	//================================
	// START of message
	//================================

	// Write basic details unless in "plain mode"
	if (! (mode & mPlain))
		write_to_streams(display_on,
				"%s|%s|%u|%s|%lld|"
				,MESSAGE_START_SEQ	// _? indicates the beginning of a message line
				,source_name
				,xmsg_getpid()
				,(mode & mDtstamp) ? dtstamp : ""
			,MESSAGE_COUNT
			);

	// Prepend error alert
	if ((mode & mError) || (mode & mCritical)) {
		write_to_streams(display_on, "ERROR: ");
	}

	// Write message itself
	va_start(ap, fmt);
	vasprintf(&vbuf, fmt, ap);
	if (mode & mPlain) {
		char *ctok = strchr(vbuf, '|');
		write_to_streams(display_on, ++ctok);
	} else
		write_to_streams(display_on, vbuf);
	free(vbuf);
	va_end(ap);

	// Write system message
	if (mode & mSystem)
		write_to_streams(display_on, " <sys:%s>", strerror(errno));

	// Write CR/LF
	write_to_streams(display_on, "\r\n");

	flush_streams();

	//================================
	// END of message
	//================================

} /* xmsg_t */
