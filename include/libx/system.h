/*
 * system.h
 * Header for libx system
 */

#ifndef _LIBX_SYSTEM_H
#define _LIBX_SYSTEM_H

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
extern "C" {
#endif // C

/* xalloc
 * Allocate memory.
 * @arg size_t Size of memory.
 */
void * xalloc (size_t size);

/* xfree
 * Free memory.
 * @arg void Pointer to value being released
 */
void xfree (void * value);

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
}
#endif // C

#endif //_LIBX_SYSTEM_H
