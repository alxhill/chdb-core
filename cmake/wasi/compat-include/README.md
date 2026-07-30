# WASI libc compatibility shims for the chdb WASI port

wasi-libc is much narrower than Emscripten's linux-flavored musl: whole POSIX
areas (signals beyond `signal()`, scheduling, terminals, users/groups, name
resolution, contexts) are either absent or compiled out behind
`__wasilibc_unmodified_upstream`. ClickHouse's OS_WASM code paths were written
against Emscripten's headers and expect these declarations to exist even where
the functionality is a stub at runtime.

This directory is prepended to the include path (`-isystem`, see the WASI
branch in cmake/target.cmake). Headers that exist in wasi-libc are *wrapped*
(`#include_next` + the missing pieces); headers wasi-libc lacks entirely are
minimal stubs whose functions fail cleanly (return -1 / NULL / no-op).

These shims intentionally mirror what Emscripten does under the hood: its
signal/sched/termios support is equally fake — the difference is only that
Emscripten bakes the stubs into libc while WASI makes the absence explicit.
