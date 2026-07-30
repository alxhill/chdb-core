/* chdb WASI shim: wasi-libc's single-threaded pthread.h lacks the
 * scheduling-parameter calls. No-op them (there is one thread). */
#ifndef CHDB_WASI_COMPAT_PTHREAD_H
#define CHDB_WASI_COMPAT_PTHREAD_H

#include_next <pthread.h>

#ifdef __wasi__

#include <sched.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline int pthread_setschedparam(pthread_t t, int policy, const struct sched_param * p)
{
    (void)t; (void)policy; (void)p;
    return 0;
}
static inline int pthread_getschedparam(pthread_t t, int * policy, struct sched_param * p)
{
    (void)t;
    if (policy) *policy = 0;
    if (p) p->sched_priority = 0;
    return 0;
}
static inline int pthread_kill(pthread_t t, int sig) { (void)t; (void)sig; return 0; /* no signals on wasm */ }
_Noreturn static inline void pthread_exit(void * retval)
{
    (void)retval;
    __builtin_trap(); /* single-threaded: exiting the only thread is fatal */
}

#ifdef __cplusplus
}
#endif

#endif /* __wasi__ */
#endif
