/* chdb WASI shim: no process umask in WASI; report the conventional 022. */
#ifndef CHDB_WASI_COMPAT_SYS_STAT_H
#define CHDB_WASI_COMPAT_SYS_STAT_H

#include_next <sys/stat.h>

#ifdef __wasi__

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline mode_t umask(mode_t mask) { (void)mask; return 022; }
static inline int mkfifo(const char * path, mode_t mode) { (void)path; (void)mode; errno = ENOSYS; return -1; }
static inline int mknod(const char * path, mode_t mode, dev_t dev) { (void)path; (void)mode; (void)dev; errno = ENOSYS; return -1; }
static inline int lchmod(const char * path, mode_t mode) { (void)path; (void)mode; errno = ENOSYS; return -1; }

#ifdef __cplusplus
}
#endif

#endif /* __wasi__ */
#endif
