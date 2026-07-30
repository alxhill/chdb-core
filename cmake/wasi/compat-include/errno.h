/* chdb WASI shim: BSD/Linux errno values wasi-libc's WASI-derived errno set
 * lacks. Values are outside the WASI range (< 77) so they never collide;
 * they are only stored/compared/stringified, never returned by the host. */
#ifndef CHDB_WASI_COMPAT_ERRNO_H
#define CHDB_WASI_COMPAT_ERRNO_H

#include_next <errno.h>

#ifdef __wasi__

#ifndef EPFNOSUPPORT
#define EPFNOSUPPORT 200
#endif
#ifndef ESOCKTNOSUPPORT
#define ESOCKTNOSUPPORT 201
#endif
#ifndef ESHUTDOWN
#define ESHUTDOWN 202
#endif
#ifndef EHOSTDOWN
#define EHOSTDOWN 203
#endif
#ifndef ETOOMANYREFS
#define ETOOMANYREFS 204
#endif
#ifndef EREMOTE
#define EREMOTE 205
#endif
#ifndef EUSERS
#define EUSERS 206
#endif

#endif /* __wasi__ */
#endif
