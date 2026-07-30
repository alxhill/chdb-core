/* chdb WASI stub: WebAssembly has no stack switching (the stack is not
 * addressable memory), so ucontext cannot be implemented. The fiber-based
 * code paths (remote/async IO) must stay disabled at runtime — these stubs
 * abort if reached, they exist only to let the translation units compile. */
#ifndef CHDB_WASI_COMPAT_UCONTEXT_H
#define CHDB_WASI_COMPAT_UCONTEXT_H

#include <signal.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct chdb_wasi_mcontext
{
    unsigned long long __reserved[8];
} mcontext_t;

typedef struct chdb_wasi_ucontext
{
    struct chdb_wasi_ucontext * uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    sigset_t uc_sigmask;
} ucontext_t;

static inline int getcontext(ucontext_t * ucp) { (void)ucp; return -1; }
static inline int setcontext(const ucontext_t * ucp) { (void)ucp; abort(); }
static inline int swapcontext(ucontext_t * oucp, const ucontext_t * ucp) { (void)oucp; (void)ucp; abort(); }
static inline void makecontext(ucontext_t * ucp, void (*func)(void), int argc, ...) { (void)ucp; (void)func; (void)argc; abort(); }

#ifdef __cplusplus
}
#endif

#endif
