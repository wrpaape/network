#ifndef NETWORK_NETWORK_NETWORK_UTILS_H_
#define NETWORK_NETWORK_NETWORK_UTILS_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "system/exit_utils.h"	/* exit|file|system|string_utils */
#include "arpa/inet.h"		/* inet_* API */


/* helper macros
 * ────────────────────────────────────────────────────────────────────────── */
/* correct network byte order */
#if IS_LITTLE_ENDIAN
#	define NETWORK_SHORT(SHORT) __builtin_bswap16(SHORT)
#	define NETWORK_LONG(LONG)   __builtin_bswap32(LONG)
#else
#	define NETWORK_SHORT(SHORT) SHORT
#	define NETWORK_LONG(LONG)   LONG
#endif /* if IS_LITTLE_ENDIAN */
#endif /* ifndef NETWORK_NETWORK_NETWORK_UTILS_H_ */

