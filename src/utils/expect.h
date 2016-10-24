#ifndef NETWORK_UTILS_EXPECT_H_
#define NETWORK_UTILS_EXPECT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdbool.h>

/* helper macros
 * ────────────────────────────────────────────────────────────────────────── */
#define LIKELY(BOOL)   __builtin_expect(BOOL, true)
#define UNLIKELY(BOOL) __builtin_expect(BOOL, false)

#endif /* ifndef NETWORK_UTILS_EXPECT_H_ */
