/* chdb WASI stub: no name resolution in WASI. Declarations let Poco::Net and
 * the resolver-adjacent code compile; every lookup fails cleanly at runtime.
 * (Networked features are compiled out of the wasm engine anyway.) */
#ifndef CHDB_WASI_COMPAT_NETDB_H
#define CHDB_WASI_COMPAT_NETDB_H

#include <sys/socket.h>
#include <stddef.h>
#include <__chdb_inet_ntoa.h>

#ifdef __cplusplus
extern "C" {
#endif

struct hostent
{
    char * h_name;
    char ** h_aliases;
    int h_addrtype;
    int h_length;
    char ** h_addr_list;
};
#define h_addr h_addr_list[0]

struct servent
{
    char * s_name;
    char ** s_aliases;
    int s_port;
    char * s_proto;
};

struct protoent
{
    char * p_name;
    char ** p_aliases;
    int p_proto;
};

struct addrinfo
{
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    socklen_t ai_addrlen;
    struct sockaddr * ai_addr;
    char * ai_canonname;
    struct addrinfo * ai_next;
};

#define AI_PASSIVE 0x01
#define AI_CANONNAME 0x02
#define AI_NUMERICHOST 0x04
#define AI_V4MAPPED 0x08
#define AI_ALL 0x10
#define AI_ADDRCONFIG 0x20
#define AI_NUMERICSERV 0x400

#define NI_NUMERICHOST 0x01
#define NI_NUMERICSERV 0x02
#define NI_NOFQDN 0x04
#define NI_NAMEREQD 0x08
#define NI_DGRAM 0x10
#define NI_MAXHOST 255
#define NI_MAXSERV 32

#define EAI_BADFLAGS (-1)
#define EAI_NONAME (-2)
#define EAI_AGAIN (-3)
#define EAI_FAIL (-4)
#define EAI_NODATA (-5)
#define EAI_FAMILY (-6)
#define EAI_SOCKTYPE (-7)
#define EAI_SERVICE (-8)
#define EAI_MEMORY (-10)
#define EAI_SYSTEM (-11)
#define EAI_OVERFLOW (-12)

#define HOST_NOT_FOUND 1
#define TRY_AGAIN 2
#define NO_RECOVERY 3
#define NO_DATA 4
#define NO_ADDRESS NO_DATA

static inline int getaddrinfo(const char * node, const char * service, const struct addrinfo * hints, struct addrinfo ** res)
{
    (void)node; (void)service; (void)hints; (void)res;
    return EAI_FAIL;
}
static inline void freeaddrinfo(struct addrinfo * res) { (void)res; }
static inline const char * gai_strerror(int code) { (void)code; return "name resolution unavailable on WASI"; }
static inline int getnameinfo(const struct sockaddr * sa, socklen_t salen, char * host, socklen_t hostlen, char * serv, socklen_t servlen, int flags)
{
    (void)sa; (void)salen; (void)host; (void)hostlen; (void)serv; (void)servlen; (void)flags;
    return EAI_FAIL;
}
static inline struct hostent * gethostbyname(const char * name) { (void)name; return (struct hostent *)0; }
static inline struct hostent * gethostbyaddr(const void * addr, socklen_t len, int type) { (void)addr; (void)len; (void)type; return (struct hostent *)0; }
static inline struct servent * getservbyname(const char * name, const char * proto) { (void)name; (void)proto; return (struct servent *)0; }
static inline struct servent * getservbyport(int port, const char * proto) { (void)port; (void)proto; return (struct servent *)0; }
static inline int getservbyname_r(const char * name, const char * proto, struct servent * se, char * buf, size_t buflen, struct servent ** result)
{
    (void)name; (void)proto; (void)se; (void)buf; (void)buflen;
    if (result) *result = (struct servent *)0;
    return 2; /* ENOENT-ish per the _r contract */
}
static inline int getservbyport_r(int port, const char * proto, struct servent * se, char * buf, size_t buflen, struct servent ** result)
{
    (void)port; (void)proto; (void)se; (void)buf; (void)buflen;
    if (result) *result = (struct servent *)0;
    return 2;
}
static inline struct protoent * getprotobyname(const char * name) { (void)name; return (struct protoent *)0; }

static int chdb_wasi_h_errno_storage = HOST_NOT_FOUND;
#define h_errno chdb_wasi_h_errno_storage
static inline void herror(const char * s) { (void)s; }
static inline const char * hstrerror(int err) { (void)err; return "name resolution unavailable on WASI"; }

#ifdef __cplusplus
}
#endif

#endif
