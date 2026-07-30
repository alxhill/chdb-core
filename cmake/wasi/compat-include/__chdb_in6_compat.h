/* chdb WASI shim: wasi-libc's struct in6_addr is a bare byte array with no
 * 16/32-bit word accessors (musl's union layout sits in a compiled-out
 * __wasilibc_unmodified_upstream block). Provide the musl-style union —
 * layout-identical (16 bytes, 4-byte alignment) — before wasi-libc's
 * __struct_in6_addr.h can load, so s6_addr16/s6_addr32 work.
 * Included from the netinet/in.h and arpa/inet.h wrappers. */
#ifndef CHDB_WASI_COMPAT_IN6_PRELUDE_H
#define CHDB_WASI_COMPAT_IN6_PRELUDE_H

#include <stdint.h>

#ifndef __wasilibc___struct_in6_addr_h
#define __wasilibc___struct_in6_addr_h

struct in6_addr
{
    union
    {
        uint8_t __s6_addr[16];
        uint16_t __s6_addr16[8];
        uint32_t __s6_addr32[4];
    } __in6_union;
};
#define s6_addr __in6_union.__s6_addr
#define s6_addr16 __in6_union.__s6_addr16
#define s6_addr32 __in6_union.__s6_addr32

#endif
#endif
