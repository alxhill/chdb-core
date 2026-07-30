/* chdb WASI stub: no statfs in WASI; callers fall back to statvfs/defaults. */
#ifndef CHDB_WASI_COMPAT_SYS_STATFS_H
#define CHDB_WASI_COMPAT_SYS_STATFS_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct { int __val[2]; } chdb_wasi_fsid_t;

struct statfs
{
    unsigned long f_type;
    unsigned long f_bsize;
    unsigned long long f_blocks;
    unsigned long long f_bfree;
    unsigned long long f_bavail;
    unsigned long long f_files;
    unsigned long long f_ffree;
    chdb_wasi_fsid_t f_fsid;
    unsigned long f_namelen;
    unsigned long f_frsize;
    unsigned long f_flags;
    unsigned long f_spare[4];
};

static inline int statfs(const char * path, struct statfs * buf) { (void)path; (void)buf; return -1; }
static inline int fstatfs(int fd, struct statfs * buf) { (void)fd; (void)buf; return -1; }

#ifdef __cplusplus
}
#endif

#endif
