/* chdb WASI shim: WebAssembly has a single rounding mode (nearest-even);
 * wasi-libc's fenv.h defines only FE_TONEAREST. Define the other C99 modes
 * with distinct values so mode-selection code compiles; fesetround() (already
 * declared) fails at runtime for anything but FE_TONEAREST, which callers
 * handle as "rounding mode not supported". */
#ifndef CHDB_WASI_COMPAT_FENV_H
#define CHDB_WASI_COMPAT_FENV_H

#include_next <fenv.h>

#ifdef __wasi__

#ifndef FE_DOWNWARD
#define FE_DOWNWARD 0x400
#endif
#ifndef FE_UPWARD
#define FE_UPWARD 0x800
#endif
#ifndef FE_TOWARDZERO
#define FE_TOWARDZERO 0xc00
#endif

#endif /* __wasi__ */
#endif
