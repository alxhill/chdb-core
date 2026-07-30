/* chdb WASI shim: inet_ntoa is compiled out of wasi-libc. One static
 * definition, shared by the arpa/inet.h and netdb.h wrappers (both may be
 * included in one TU; the guard keeps it single). */
#ifndef CHDB_WASI_COMPAT_INET_NTOA_H
#define CHDB_WASI_COMPAT_INET_NTOA_H

#include <netinet/in.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((unused)) static char * inet_ntoa(struct in_addr in)
{
    static char chdb_wasi_ntoa_buf[16];
    const unsigned char * b = (const unsigned char *)&in;
    snprintf(chdb_wasi_ntoa_buf, sizeof(chdb_wasi_ntoa_buf), "%u.%u.%u.%u", b[0], b[1], b[2], b[3]);
    return chdb_wasi_ntoa_buf;
}

#ifdef __cplusplus
}
#endif

#endif
