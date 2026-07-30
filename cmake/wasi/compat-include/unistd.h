/* chdb WASI shim: pipes and fd duplication are compiled out of wasi-libc
 * (no such syscalls in WASI p1). Fail cleanly at runtime. */
#ifndef CHDB_WASI_COMPAT_UNISTD_H
#define CHDB_WASI_COMPAT_UNISTD_H

#include_next <unistd.h>

#ifdef __wasi__

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline int pipe(int fds[2]) { (void)fds; errno = ENOSYS; return -1; }
static inline int pipe2(int fds[2], int flags) { (void)fds; (void)flags; errno = ENOSYS; return -1; }
static inline int dup(int fd) { (void)fd; errno = ENOSYS; return -1; }
static inline int dup2(int oldfd, int newfd) { (void)oldfd; (void)newfd; errno = ENOSYS; return -1; }
static inline pid_t fork(void) { errno = ENOSYS; return (pid_t)-1; }
static inline pid_t setsid(void) { errno = ENOSYS; return (pid_t)-1; }
static inline int setuid(uid_t uid) { (void)uid; errno = ENOSYS; return -1; }
static inline int setgid(gid_t gid) { (void)gid; errno = ENOSYS; return -1; }
static inline uid_t getuid(void) { return 0; }
static inline uid_t geteuid(void) { return 0; }
static inline gid_t getgid(void) { return 0; }
static inline gid_t getegid(void) { return 0; }
static inline int execv(const char * path, char * const argv[]) { (void)path; (void)argv; errno = ENOSYS; return -1; }
static inline int execve(const char * path, char * const argv[], char * const envp[]) { (void)path; (void)argv; (void)envp; errno = ENOSYS; return -1; }
static inline int execvp(const char * file, char * const argv[]) { (void)file; (void)argv; errno = ENOSYS; return -1; }
static inline int fchdir(int fd) { (void)fd; errno = ENOSYS; return -1; }
static inline pid_t vfork(void) { errno = ENOSYS; return (pid_t)-1; }
static inline int lockf(int fd, int cmd, off_t len) { (void)fd; (void)cmd; (void)len; return 0; /* single process: locks trivially succeed */ }
static inline int fchown(int fd, uid_t owner, gid_t group) { (void)fd; (void)owner; (void)group; errno = ENOSYS; return -1; }
static inline int lchown(const char * path, uid_t owner, gid_t group) { (void)path; (void)owner; (void)group; errno = ENOSYS; return -1; }
static inline void sync(void) {}
#ifndef F_LOCK
#define F_ULOCK 0
#define F_LOCK 1
#define F_TLOCK 2
#define F_TEST 3
#endif

#ifdef __cplusplus
}
#endif

#endif /* __wasi__ */
#endif
