/* chdb WASI shim: alias the Linux-only clock ids to the closest WASI clock
 * and stub the timezone globals (the embedded engine uses DateLUT, not the
 * libc timezone machinery; Emscripten similarly hardcodes UTC). */
#ifndef CHDB_WASI_COMPAT_TIME_H
#define CHDB_WASI_COMPAT_TIME_H

#include_next <time.h>

#ifdef __wasi__

#ifndef CLOCK_MONOTONIC_COARSE
#define CLOCK_MONOTONIC_COARSE CLOCK_MONOTONIC
#endif
#ifndef CLOCK_REALTIME_COARSE
#define CLOCK_REALTIME_COARSE CLOCK_REALTIME
#endif
#ifndef CLOCK_THREAD_CPUTIME_ID
#if defined(CLOCK_PROCESS_CPUTIME_ID)
/* single-threaded: the thread is the process */
#define CLOCK_THREAD_CPUTIME_ID CLOCK_PROCESS_CPUTIME_ID
#else
#define CLOCK_THREAD_CPUTIME_ID CLOCK_MONOTONIC
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

static inline void tzset(void) {}

/* Per-TU copies are fine for stubs: everything is UTC. NB: `timezone` must
 * NOT be an object-like macro — it would mangle wasi-libc's `struct timezone`
 * (sys/time.h). The rare `timezone`/`daylight` users are leaf fixes instead. */
static char * chdb_wasi_tzname[2] = {(char *)"UTC", (char *)"UTC"};
#define tzname chdb_wasi_tzname

#ifdef __cplusplus
}
#endif

#endif /* __wasi__ */
#endif
