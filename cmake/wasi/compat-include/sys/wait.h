/* chdb WASI stub: no processes in WASI; wait* fails with -1. */
#ifndef CHDB_WASI_COMPAT_SYS_WAIT_H
#define CHDB_WASI_COMPAT_SYS_WAIT_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WNOHANG 1
#define WUNTRACED 2
#define WCONTINUED 8
#define WIFCONTINUED(s) ((s) == 0xffff)

#define WIFEXITED(s) (((s) & 0x7f) == 0)
#define WEXITSTATUS(s) (((s) & 0xff00) >> 8)
#define WIFSIGNALED(s) (((signed char)(((s) & 0x7f) + 1) >> 1) > 0)
#define WTERMSIG(s) ((s) & 0x7f)
#define WIFSTOPPED(s) (((s) & 0xff) == 0x7f)
#define WSTOPSIG(s) WEXITSTATUS(s)
#define WCOREDUMP(s) ((s) & 0x80)

static inline pid_t wait(int * status) { (void)status; return (pid_t)-1; }
static inline pid_t waitpid(pid_t pid, int * status, int options) { (void)pid; (void)status; (void)options; return (pid_t)-1; }

#ifdef __cplusplus
}
#endif

#endif
