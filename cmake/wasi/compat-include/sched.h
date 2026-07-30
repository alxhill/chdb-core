/* chdb WASI shim: wasi-libc compiles out the scheduling-policy surface
 * (no CPU scheduling in WASI). struct sched_param exists; the policy
 * constants and priority queries do not. */
#ifndef CHDB_WASI_COMPAT_SCHED_H
#define CHDB_WASI_COMPAT_SCHED_H

#include_next <sched.h>

#ifdef __wasi__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SCHED_OTHER
#define SCHED_OTHER 0
#define SCHED_FIFO 1
#define SCHED_RR 2
#define SCHED_BATCH 3
#define SCHED_IDLE 5
#endif

static inline int sched_get_priority_max(int policy) { (void)policy; return 0; }
static inline int sched_get_priority_min(int policy) { (void)policy; return 0; }

#ifdef __cplusplus
}
#endif

#endif /* __wasi__ */
#endif
