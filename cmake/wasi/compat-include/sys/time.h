/* chdb WASI shim: symlink-time updates are unsupported (callers fall back). */
#ifndef CHDB_WASI_COMPAT_SYS_TIME_H
#define CHDB_WASI_COMPAT_SYS_TIME_H

#include_next <sys/time.h>

#ifdef __wasi__

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline int lutimes(const char * path, const struct timeval times[2])
{
    (void)path; (void)times;
    errno = ENOSYS;
    return -1;
}

/* Interval timers need signal delivery; neither exists on wasm. */
#ifndef ITIMER_REAL
#define ITIMER_REAL 0
#define ITIMER_VIRTUAL 1
#define ITIMER_PROF 2
struct itimerval
{
    struct timeval it_interval;
    struct timeval it_value;
};
static inline int getitimer(int which, struct itimerval * curr) { (void)which; (void)curr; errno = ENOSYS; return -1; }
static inline int setitimer(int which, const struct itimerval * val, struct itimerval * old)
{
    (void)which; (void)val; (void)old;
    errno = ENOSYS;
    return -1;
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* __wasi__ */
#endif
