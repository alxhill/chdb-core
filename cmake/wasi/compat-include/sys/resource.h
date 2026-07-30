/* chdb WASI shim: wasi-libc has getrusage(RUSAGE_SELF); add the Linux-only
 * per-thread constant (single-threaded: the thread is the process). */
#ifndef CHDB_WASI_COMPAT_SYS_RESOURCE_H
#define CHDB_WASI_COMPAT_SYS_RESOURCE_H

#include_next <sys/resource.h>

#ifdef __wasi__

#ifndef RUSAGE_THREAD
#define RUSAGE_THREAD RUSAGE_SELF
#endif

/* No resource limits in WASI; report "unlimited" and accept sets. */
#ifndef RLIMIT_NOFILE

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long long rlim_t;
#define RLIM_INFINITY (~0ULL)

struct rlimit
{
    rlim_t rlim_cur;
    rlim_t rlim_max;
};

#define RLIMIT_CPU 0
#define RLIMIT_FSIZE 1
#define RLIMIT_DATA 2
#define RLIMIT_STACK 3
#define RLIMIT_CORE 4
#define RLIMIT_RSS 5
#define RLIMIT_NPROC 6
#define RLIMIT_NOFILE 7
#define RLIMIT_AS 9

static inline int getrlimit(int resource, struct rlimit * rlim)
{
    (void)resource;
    if (rlim)
    {
        rlim->rlim_cur = RLIM_INFINITY;
        rlim->rlim_max = RLIM_INFINITY;
    }
    return 0;
}
static inline int setrlimit(int resource, const struct rlimit * rlim) { (void)resource; (void)rlim; return 0; }

#ifdef __cplusplus
}
#endif

#endif /* RLIMIT_NOFILE */

#endif /* __wasi__ */
#endif
