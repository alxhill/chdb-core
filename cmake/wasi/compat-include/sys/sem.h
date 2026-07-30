/* chdb WASI stub: no SysV semaphores in WASI. */
#ifndef CHDB_WASI_COMPAT_SYS_SEM_H
#define CHDB_WASI_COMPAT_SYS_SEM_H

#include <sys/ipc.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SEM_UNDO 0x1000
#define GETVAL 12
#define SETVAL 16

struct sembuf
{
    unsigned short sem_num;
    short sem_op;
    short sem_flg;
};

static inline int semget(key_t key, int nsems, int semflg) { (void)key; (void)nsems; (void)semflg; return -1; }
static inline int semop(int semid, struct sembuf * sops, size_t nsops) { (void)semid; (void)sops; (void)nsops; return -1; }
static inline int semctl(int semid, int semnum, int cmd, ...) { (void)semid; (void)semnum; (void)cmd; return -1; }

#ifdef __cplusplus
}
#endif

#endif
