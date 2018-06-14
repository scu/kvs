/*
 * messages.h
 * Messaging header for libx messages
 */

#ifndef _LIBX_MESSAGES_H
#define _LIBX_MESSAGES_H

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
extern "C" {
#endif // C

#define xmsg(a, b, ...) xmsg_t(a, "%s:%d|" b, __FILE__, __LINE__, ##__VA_ARGS__)

// Message flags
typedef enum MsgFlag {
	// Message formats
	 mNormal	= 0x00		// Default message
	,mDisplay	= 0x01		// Force messages to be displayed to stderr
	,mDtstamp	= 0x02		// Add Date/time stamp
	,mPlain		= 0x04		// Plain output (no dtstamp, counters, etc.)
	,mReaddate	= 0x08		// Date/time in human-readable format (requires mDtstamp)
	// Message enhancements
	,mSystem	= 0x10		// Include system (strerror) messages
	,mSql		= 0x20		// Report last SQL (if supported)
	// Trace flags
	,mTrace1	= 0x40		// BASIC information (start/stop/etc.)
	,mTrace2	= 0x80		// NORMAL trace level
	,mTrace3	= 0x100		// HIGHLY VERBOSE (i.e. each rec. field is printed per rec.)
	// Severity flags
	,mWarning	= 0x200		// Message is a WARNING
	,mError		= 0x400		// Message is an ERROR
	,mCritical	= 0x800		// Message is a CRITICAL ERROR
} MsgFlag;

/* xmsg_set_display
 * Setting to "true" means messages will be displayed
 * @arg display_msgs
 */
void xmsg_set_display (bool display_msgs);

/* xmsg_zero
 * Truncate the message log (but not the aggregate log) to zero
 */
bool xmsg_zero (void);

/* xmsg_set_trace_level
 * Set to TraceLevel 1 through 3
 * @arg display_msgs
 */
void xmsg_set_trace_level (int trace_level);

/* xmsg_set_mode
 * Applies "mode" to all subsequent messages
 */
void xmsg_set_mode (int mode);

/* xmsg_set_messaging_flag
 * Sets messaging on/off (default is "on")
 * @arg messaging_on	Boolean flag indicates messaging is on/off
 */
void xmsg_set_messaging_flag (bool messaging_on);

/* xmsg_init
 * Initialize messaging for process
 * Call only once per PROCESS.  Do not call from each thread.
 *  @arg name			String representing the name of the process (i.e. "envoyd").
 *  @returns true/false on success/fail
 */
bool xmsg_init (const char *name);

/* xmsg_close
 * Close all log files
 */
void xmsg_close (void);

/* xmsg_t
 * Message caller
 * @arg msgflag		Message flag
 * @arg fmt			Format string
 * @arg ...			Variable argument list
 */
void xmsg_t (int msgflag, char *fmt, ...);

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
}
#endif // C

#endif //_LIBX_MESSAGES_H
