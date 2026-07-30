/* chdb WASI shim: see __chdb_in6_compat.h — supplies the musl union layout of
 * in6_addr (with s6_addr16/s6_addr32 accessors) that wasi-libc compiles out. */
#ifndef CHDB_WASI_COMPAT_NETINET_IN_H
#define CHDB_WASI_COMPAT_NETINET_IN_H

#include <__chdb_in6_compat.h>
#include_next <netinet/in.h>

#endif
