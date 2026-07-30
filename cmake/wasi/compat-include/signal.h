/* chdb WASI shim: wasi-libc's emulated signal.h provides signal()/raise() and
 * sigset_t, but compiles out sigaction, sigprocmask and the sigset ops
 * (guarded by __wasilibc_unmodified_upstream). Declare them as no-op stubs,
 * matching Emscripten's behavior (signals never fire in wasm anyway). */
#ifndef CHDB_WASI_COMPAT_SIGNAL_H
#define CHDB_WASI_COMPAT_SIGNAL_H

#include_next <signal.h>

#ifdef __wasi__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SIG_BLOCK
#define SIG_BLOCK 0
#define SIG_UNBLOCK 1
#define SIG_SETMASK 2
#endif

#ifndef SA_NOCLDSTOP
#define SA_NOCLDSTOP 1
#define SA_NOCLDWAIT 2
#define SA_SIGINFO 4
#define SA_ONSTACK 0x08000000
#define SA_RESTART 0x10000000
#define SA_NODEFER 0x40000000
#define SA_RESETHAND 0x80000000
#endif

#ifndef SS_ONSTACK
#define SS_ONSTACK 1
#define SS_DISABLE 2
#endif

/* siginfo si_code values (musl), for the signal-describing code in
 * StackTrace/BaseDaemon — no signal ever carries them on wasm. */
#ifndef ILL_ILLOPC
#define ILL_ILLOPC 1
#define ILL_ILLOPN 2
#define ILL_ILLADR 3
#define ILL_ILLTRP 4
#define ILL_PRVOPC 5
#define ILL_PRVREG 6
#define ILL_COPROC 7
#define ILL_BADSTK 8
#endif
#ifndef FPE_INTDIV
#define FPE_INTDIV 1
#define FPE_INTOVF 2
#define FPE_FLTDIV 3
#define FPE_FLTOVF 4
#define FPE_FLTUND 5
#define FPE_FLTRES 6
#define FPE_FLTINV 7
#define FPE_FLTSUB 8
#endif
#ifndef SEGV_MAPERR
#define SEGV_MAPERR 1
#define SEGV_ACCERR 2
#define SEGV_BNDERR 3
#define SEGV_PKUERR 4
#endif
#ifndef BUS_ADRALN
#define BUS_ADRALN 1
#define BUS_ADRERR 2
#define BUS_OBJERR 3
#define BUS_MCEERR_AR 4
#define BUS_MCEERR_AO 5
#endif
#ifndef CLD_EXITED
#define CLD_EXITED 1
#define CLD_KILLED 2
#define CLD_DUMPED 3
#define CLD_TRAPPED 4
#define CLD_STOPPED 5
#define CLD_CONTINUED 6
#endif
#ifndef SI_USER
#define SI_USER 0
#define SI_KERNEL 128
#define SI_QUEUE (-1)
#define SI_TIMER (-2)
#define SI_MESGQ (-3)
#define SI_ASYNCIO (-4)
#define SI_TKILL (-6)
#endif

/* wasi-libc's signal.h never defines sigset_t (its definition sits in a
 * compiled-out __wasilibc_unmodified_upstream block); request it through
 * musl's alltypes mechanism, which is idempotent and conflict-free. */
#define __NEED_sigset_t
#include <bits/alltypes.h>

#ifndef MINSIGSTKSZ
#define MINSIGSTKSZ 2048
#define SIGSTKSZ 8192
#endif

typedef struct chdb_wasi_stack
{
    void * ss_sp;
    int ss_flags;
    size_t ss_size;
} stack_t;

typedef struct chdb_wasi_siginfo
{
    int si_signo;
    int si_errno;
    int si_code;
    int si_pid;
    int si_uid;
    void * si_addr;
    int si_status;
    long si_band;
    union { int sival_int; void * sival_ptr; } si_value;
} siginfo_t;

struct sigaction
{
    union
    {
        void (*sa_handler)(int);
        void (*sa_sigaction)(int, siginfo_t *, void *);
    };
    sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer)(void);
};

static inline int sigemptyset(sigset_t * set) { (void)set; return 0; }
static inline int sigfillset(sigset_t * set) { (void)set; return 0; }
static inline int sigaddset(sigset_t * set, int sig) { (void)set; (void)sig; return 0; }
static inline int sigdelset(sigset_t * set, int sig) { (void)set; (void)sig; return 0; }
static inline int sigismember(const sigset_t * set, int sig) { (void)set; (void)sig; return 0; }
static inline int sigprocmask(int how, const sigset_t * set, sigset_t * old) { (void)how; (void)set; (void)old; return 0; }
static inline int pthread_sigmask(int how, const sigset_t * set, sigset_t * old) { (void)how; (void)set; (void)old; return 0; }
static inline int sigwait(const sigset_t * set, int * sig) { (void)set; (void)sig; return -1; }
static inline int sigpending(sigset_t * set) { (void)set; return 0; }
static inline int sigsuspend(const sigset_t * set) { (void)set; return -1; }
static inline int sigaltstack(const stack_t * ss, stack_t * old) { (void)ss; (void)old; return 0; }
static inline int sigaction(int sig, const struct sigaction * act, struct sigaction * old)
{
    (void)sig; (void)act; (void)old;
    return 0; /* pretend installed; wasm delivers no signals */
}

struct timespec;
static inline int sigtimedwait(const sigset_t * set, siginfo_t * info, const struct timespec * timeout)
{
    (void)set; (void)info; (void)timeout;
    return -1; /* no signals ever arrive */
}
static inline int sigwaitinfo(const sigset_t * set, siginfo_t * info) { (void)set; (void)info; return -1; }
static inline int kill(int pid, int sig) { (void)pid; (void)sig; return -1; /* no processes */ }
static inline int killpg(int pgrp, int sig) { (void)pgrp; (void)sig; return -1; }

#ifdef __cplusplus
}
#endif

#endif /* __wasi__ */
#endif
