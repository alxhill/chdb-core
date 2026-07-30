/* chdb WASI shim: temp files, PTYs and GNU qsort_r, which wasi-libc gates
 * out or lacks. PTY functions fail cleanly (no terminals); qsort_r is
 * implemented over qsort with a global context — safe single-threaded. */
#ifndef CHDB_WASI_COMPAT_STDLIB_H
#define CHDB_WASI_COMPAT_STDLIB_H

#include_next <stdlib.h>

#ifdef __wasi__

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline int mkstemp(char * tmpl) { (void)tmpl; errno = ENOSYS; return -1; }
static inline int mkstemps(char * tmpl, int suffixlen) { (void)tmpl; (void)suffixlen; errno = ENOSYS; return -1; }
static inline char * mkdtemp(char * tmpl) { (void)tmpl; errno = ENOSYS; return (char *)0; }

static inline int posix_openpt(int flags) { (void)flags; errno = ENOSYS; return -1; }
static inline int grantpt(int fd) { (void)fd; errno = ENOSYS; return -1; }
static inline int unlockpt(int fd) { (void)fd; errno = ENOSYS; return -1; }
static inline char * ptsname(int fd) { (void)fd; errno = ENOSYS; return (char *)0; }

/* GNU-style qsort_r. Single-threaded engine: a global trampoline is safe. */
static int (*chdb_wasi_qsort_r_compar)(const void *, const void *, void *);
static void * chdb_wasi_qsort_r_arg;
__attribute__((unused)) static int chdb_wasi_qsort_r_trampoline(const void * a, const void * b)
{
    return chdb_wasi_qsort_r_compar(a, b, chdb_wasi_qsort_r_arg);
}
__attribute__((unused)) static void
qsort_r(void * base, size_t nmemb, size_t size, int (*compar)(const void *, const void *, void *), void * arg)
{
    chdb_wasi_qsort_r_compar = compar;
    chdb_wasi_qsort_r_arg = arg;
    qsort(base, nmemb, size, chdb_wasi_qsort_r_trampoline);
}

#ifdef __cplusplus
}
#endif

#endif /* __wasi__ */
#endif
