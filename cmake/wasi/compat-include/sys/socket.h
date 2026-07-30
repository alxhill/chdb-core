/* chdb WASI shim: WASI p1 only supports accept/send/recv/shutdown on
 * preopened sockets; wasi-libc compiles out the rest of the BSD socket API.
 * Stub the missing calls to fail with ENOSYS — the embedded engine never
 * listens or dials, these paths exist only in never-taken branches. */
#ifndef CHDB_WASI_COMPAT_SYS_SOCKET_H
#define CHDB_WASI_COMPAT_SYS_SOCKET_H

#include <__chdb_in6_compat.h>
#include_next <sys/socket.h>

#ifdef __wasi__

#include <errno.h>

#ifndef AF_UNIX
#define AF_UNIX 1
#endif
#ifndef AF_PACKET
#define AF_PACKET 17
#endif
#ifndef PF_UNIX
#define PF_UNIX AF_UNIX
#endif
#ifndef PF_INET
#define PF_INET AF_INET
#endif
#ifndef PF_INET6
#define PF_INET6 AF_INET6
#endif

/* Socket-option and message-flag constants wasi-libc compiles out (values as
 * in musl/linux; the calls below all fail with ENOSYS anyway). */
#ifndef SOL_SOCKET
#define SOL_SOCKET 1
#endif
#ifndef SO_REUSEADDR
#define SO_REUSEADDR 2
#endif
#ifndef SO_ERROR
#define SO_ERROR 4
#endif
#ifndef SO_BROADCAST
#define SO_BROADCAST 6
#endif
#ifndef SO_SNDBUF
#define SO_SNDBUF 7
#endif
#ifndef SO_RCVBUF
#define SO_RCVBUF 8
#endif
#ifndef SO_KEEPALIVE
#define SO_KEEPALIVE 9
#endif
#ifndef SO_OOBINLINE
#define SO_OOBINLINE 10
#endif
#ifndef SO_LINGER
/* NB: wasi-libc defines struct linger unconditionally; only the macro is gated. */
#define SO_LINGER 13
#endif
#ifndef SO_REUSEPORT
#define SO_REUSEPORT 15
#endif
#ifndef SO_RCVTIMEO
#define SO_RCVTIMEO 20
#endif
#ifndef SO_SNDTIMEO
#define SO_SNDTIMEO 21
#endif
#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT 0x40
#endif
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0x4000
#endif
#ifndef MSG_OOB
#define MSG_OOB 1
#endif
#ifndef MSG_WAITALL
#define MSG_WAITALL 0x100
#endif
#ifndef SOCK_RAW
#define SOCK_RAW 3
#endif
#ifndef SOL_IP
#define SOL_IP 0
#endif
#ifndef SOL_TCP
#define SOL_TCP 6
#endif
#ifndef SHUT_RD
#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2
#endif

#ifdef __cplusplus
extern "C" {
#endif

static inline int socket(int domain, int type, int protocol)
{
    (void)domain; (void)type; (void)protocol;
    errno = ENOSYS;
    return -1;
}
static inline int socketpair(int domain, int type, int protocol, int fds[2])
{
    (void)domain; (void)type; (void)protocol; (void)fds;
    errno = ENOSYS;
    return -1;
}
static inline int bind(int fd, const struct sockaddr * addr, socklen_t len)
{
    (void)fd; (void)addr; (void)len;
    errno = ENOSYS;
    return -1;
}
static inline int connect(int fd, const struct sockaddr * addr, socklen_t len)
{
    (void)fd; (void)addr; (void)len;
    errno = ENOSYS;
    return -1;
}
static inline int listen(int fd, int backlog)
{
    (void)fd; (void)backlog;
    errno = ENOSYS;
    return -1;
}
static inline int getsockname(int fd, struct sockaddr * addr, socklen_t * len)
{
    (void)fd; (void)addr; (void)len;
    errno = ENOSYS;
    return -1;
}
static inline int getpeername(int fd, struct sockaddr * addr, socklen_t * len)
{
    (void)fd; (void)addr; (void)len;
    errno = ENOSYS;
    return -1;
}
static inline int setsockopt(int fd, int level, int optname, const void * optval, socklen_t optlen)
{
    (void)fd; (void)level; (void)optname; (void)optval; (void)optlen;
    errno = ENOSYS;
    return -1;
}
static inline ssize_t sendto(int fd, const void * buf, size_t n, int flags, const struct sockaddr * addr, socklen_t len)
{
    (void)fd; (void)buf; (void)n; (void)flags; (void)addr; (void)len;
    errno = ENOSYS;
    return -1;
}
static inline ssize_t recvfrom(int fd, void * buf, size_t n, int flags, struct sockaddr * addr, socklen_t * len)
{
    (void)fd; (void)buf; (void)n; (void)flags; (void)addr; (void)len;
    errno = ENOSYS;
    return -1;
}
static inline ssize_t sendmsg(int fd, const struct msghdr * msg, int flags)
{
    (void)fd; (void)msg; (void)flags;
    errno = ENOSYS;
    return -1;
}
static inline ssize_t recvmsg(int fd, struct msghdr * msg, int flags)
{
    (void)fd; (void)msg; (void)flags;
    errno = ENOSYS;
    return -1;
}

#ifdef __cplusplus
}
#endif

#endif /* __wasi__ */
#endif
