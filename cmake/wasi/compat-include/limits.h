/* chdb WASI shim: pipe-related limits (no pipes in WASI, but the constant is
 * referenced in compile-time expressions). */
#ifndef CHDB_WASI_COMPAT_LIMITS_H
#define CHDB_WASI_COMPAT_LIMITS_H

#include_next <limits.h>

#ifdef __wasi__

#ifndef PIPE_BUF
#define PIPE_BUF 4096
#endif

#endif /* __wasi__ */
#endif
