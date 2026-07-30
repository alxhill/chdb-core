/* chdb WASI stub: no SysV IPC in WASI. */
#ifndef CHDB_WASI_COMPAT_SYS_IPC_H
#define CHDB_WASI_COMPAT_SYS_IPC_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IPC_CREAT 01000
#define IPC_EXCL 02000
#define IPC_NOWAIT 04000
#define IPC_RMID 0
#define IPC_SET 1
#define IPC_STAT 2
#define IPC_PRIVATE ((key_t)0)

static inline key_t ftok(const char * path, int id) { (void)path; (void)id; return (key_t)-1; }

#ifdef __cplusplus
}
#endif

#endif
