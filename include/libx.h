/*
 * libx.h
 * Main header for libx
 */

#ifndef _LIBX_H
#define _LIBX_H

#define _GNU_SOURCE

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
extern "C" {
#endif // C

// Provide bool type for C
#if !defined(__cplusplus) && !defined(__CPLUSPLUS__)
#ifdef bool
#undef bool
#undef false
#undef true
#endif //bool
typedef enum bool { err = (-1), false = 0, true } bool;
#endif //__cplusplus

#include <bstrlib.h>
#include <libx/messages.h>
#include <libx/system.h>
#include <libx/strings.h>

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
}
#endif // C

#endif //_LIBX_H
