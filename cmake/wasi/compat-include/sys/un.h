/* chdb WASI replacement: wasi-libc's sockaddr_un deliberately omits sun_path
 * (no UNIX-domain sockets in WASI p1). Provide the full POSIX layout so the
 * (never-taken at runtime) socket code paths compile. Defines wasi-libc's own
 * guards so its gutted definitions never load on top. */
#ifndef _SYS_UN_H
#define _SYS_UN_H
#define __wasilibc___struct_sockaddr_un_h

#include <features.h>
#include <string.h> /* strlen, with wasi-libc's own attributes (for SUN_LEN) */

#define __NEED_sa_family_t
#define __NEED_size_t
#include <bits/alltypes.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sockaddr_un
{
    sa_family_t sun_family;
    char sun_path[108];
};

#define SUN_LEN(s) (2 + strlen((s)->sun_path))

#ifdef __cplusplus
}
#endif

#endif
