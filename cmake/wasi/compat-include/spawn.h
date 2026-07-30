/* chdb WASI stub: no processes in WASI; posix_spawn fails with ENOSYS. */
#ifndef CHDB_WASI_COMPAT_SPAWN_H
#define CHDB_WASI_COMPAT_SPAWN_H

#include <sys/types.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct { int __flags; } posix_spawnattr_t;
typedef struct { int __pad; } posix_spawn_file_actions_t;

static inline int posix_spawn(pid_t * pid, const char * path, const posix_spawn_file_actions_t * fa, const posix_spawnattr_t * attr, char * const argv[], char * const envp[])
{
    (void)pid; (void)path; (void)fa; (void)attr; (void)argv; (void)envp;
    return ENOSYS;
}
static inline int posix_spawnp(pid_t * pid, const char * file, const posix_spawn_file_actions_t * fa, const posix_spawnattr_t * attr, char * const argv[], char * const envp[])
{
    (void)pid; (void)file; (void)fa; (void)attr; (void)argv; (void)envp;
    return ENOSYS;
}
static inline int posix_spawn_file_actions_init(posix_spawn_file_actions_t * fa) { (void)fa; return 0; }
static inline int posix_spawn_file_actions_destroy(posix_spawn_file_actions_t * fa) { (void)fa; return 0; }
static inline int posix_spawnattr_init(posix_spawnattr_t * attr) { (void)attr; return 0; }
static inline int posix_spawnattr_destroy(posix_spawnattr_t * attr) { (void)attr; return 0; }

#ifdef __cplusplus
}
#endif

#endif
