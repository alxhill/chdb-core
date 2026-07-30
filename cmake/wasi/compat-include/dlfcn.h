/* chdb WASI shim: wasi-libc's dlfcn.h has the dlopen family but not the
 * GNU dladdr extension (no symbol tables at runtime in wasm). */
#ifndef CHDB_WASI_COMPAT_DLFCN_H
#define CHDB_WASI_COMPAT_DLFCN_H

#include_next <dlfcn.h>

#ifdef __wasi__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RTLD_DEFAULT
#define RTLD_DEFAULT ((void *)0)
#endif

typedef struct chdb_wasi_dl_info
{
    const char * dli_fname;
    void * dli_fbase;
    const char * dli_sname;
    void * dli_saddr;
} Dl_info;

static inline int dladdr(const void * addr, Dl_info * info)
{
    (void)addr;
    if (info)
    {
        info->dli_fname = 0;
        info->dli_fbase = 0;
        info->dli_sname = 0;
        info->dli_saddr = 0;
    }
    return 0; /* 0 = lookup failed, per the dladdr contract */
}

#ifdef __cplusplus
}
#endif

#endif /* __wasi__ */
#endif
