/* chdb WASI shim: see __chdb_in6_compat.h — must run before wasi-libc's
 * __struct_in6_addr.h regardless of which network header a TU includes first. */
#ifndef CHDB_WASI_COMPAT_ARPA_INET_H
#define CHDB_WASI_COMPAT_ARPA_INET_H

#include <__chdb_in6_compat.h>
#include_next <arpa/inet.h>

#ifdef __wasi__
#include <__chdb_inet_ntoa.h>
#endif

#endif
