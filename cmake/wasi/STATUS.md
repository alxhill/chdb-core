# chdb → WASI port: status and recipe

**State: experimental — WORKING for read queries under stock wasmtime.**
`buildwasi64/programs/wasi/chdb.wasm` (wasm64-wasip1, ~251 MB) boots the full
embedded ClickHouse engine and executes real queries under plain
`wasmtime run`:

```bash
# one-time: AOT-compile (~12 min; or let wasmtime JIT on first run)
wasmtime compile chdb.wasm -o chdb.cwasm

mkdir -p root/chdb/data
wasmtime run --allow-precompiled --dir=root::/ chdb.cwasm \
  "SELECT number % 5 AS k, count(), sum(number) FROM numbers(1000000) GROUP BY k ORDER BY k"
wasmtime run --allow-precompiled --dir=root::/ chdb.cwasm \
  "SELECT city, count(), avg(fare) FROM file('/chdb/data/user_files/trips.csv', 'CSVWithNames') GROUP BY city" 
```

Extra CLI args after `<sql> <format>` are forwarded to the embedded server
(`--log-level=trace --logger.console=1` for boot logs; env
`CHDB_BOOT_TRACE=1` prints boot-phase breadcrumbs).

Verified: `SELECT` pipelines (aggregation over `numbers()`, 1M rows), CSV via
`file()` through the preopened filesystem, `CREATE TABLE` (Log family).
Known single-threaded limitations (same class as the Emscripten `-st` build):
`ENGINE = MergeTree` creation needs the background executors, and *reloading*
persisted tables at boot goes through `AsyncLoader`, which is strictly
worker-thread-based — so cross-process table persistence needs either a
threads story or an AsyncLoader inline-execution mode.

**Primary target: wasm64-wasip1 over the STANDARD wasip1 ABI** (Memory64
required — matches the Emscripten port's choice and the codebase's 64-bit
`size_t` assumptions). The key trick making stock runtimes work: a Memory64
module may import the standard (i32-pointer) wasip1 functions and truncate
pointers at the syscall boundary, as long as linear memory stays below 4 GiB
— and wasmtime's WASI host reads a 64-bit guest memory just fine (verified).
The guest-side adapter lives in the wasi-libc fork's
`libc-bottom-half/sources/__wasilibc_real.c` (rewritten): standard import
names, i32 pointer/size params, translation for the ABI structs whose layout
differs (iovec arrays {u64,u64}→{u32,u32}, prestat, the argv/environ pointer
arrays, u32 out-params widened through temporaries). Result: `wasmtime run`
executes the module with no custom host. wasm32-wasip1 is kept as a
fully-official-toolchain fallback; its extra cost is the 32-bit fixes
catalogued below.

## Why WASI is now plausible at all (mid-2026)

Two long-standing blockers fell recently:

- **C++ exceptions**: wasi-sdk ≥ 33 ships exceptions-enabled sysroots
  (`lib/<target>/eh/` variants of libc++/libc++abi/libunwind,
  [wasi-sdk#606](https://github.com/WebAssembly/wasi-sdk/pull/606)).
  ClickHouse pervasively requires exception *catching*, so this was a hard
  prerequisite. Caveats: pass `-fwasm-exceptions` **and**
  `-mllvm -wasm-use-legacy-eh=false` (LLVM 22 still defaults to the legacy EH
  opcodes, which wasmtime does not implement — only standardized exnref), and
  link `-lunwind`.
- **Runtime support**: wasmtime enabled the exception-handling proposal by
  default in July 2026 (v47). Node ≥ 23 (V8) has supported both for longer.

Still missing upstream:

- **wasm64**: no official wasm64-wasi target exists and none is planned for
  the wasip1 line ([wasi-sdk#185](https://github.com/WebAssembly/wasi-sdk/issues/185)).
  ClickHouse assumes 64-bit `size_t` in places (see below), which is why the
  Emscripten port targets Memory64.
- **threads**: wasi-threads was withdrawn; wasmtime 47 removed its CLI
  support. A WASI build is single-threaded for now
  (`CHDB_WASM_SINGLE_THREADED`, same degradation as the Emscripten -st build).

## Reproduce the wasm32 build attempt

```bash
# wasi-sdk 33 for your host from https://github.com/WebAssembly/wasi-sdk/releases
export WASI_SDK_PATH=~/code/wasi-sdk-33.0-arm64-macos

mkdir buildwasi && cd buildwasi
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/wasi/toolchain.cmake \
  -DCMAKE_BUILD_TYPE=MinSizeRel -DWERROR=0 \
  -DENABLE_THINLTO=0 -DENABLE_TESTS=0 -DENABLE_XRAY=0 \
  -DENABLE_LIBRARIES=0 -DENABLE_PYTHON=0 \
  -DUSE_STATIC_LIBRARIES=1 -DSPLIT_SHARED_LIBRARIES=0 \
  -DENABLE_JEMALLOC=0 -DENABLE_ICU=0 \
  -DENABLE_EMBEDDED_COMPILER=0 -DENABLE_DWARF_PARSER=0 -DENABLE_RUST=0 \
  -DENABLE_UTILS=0 -DENABLE_EXAMPLES=0 -DENABLE_BENCHMARKS=0 \
  -DENABLE_FUZZING=OFF -DENABLE_BUZZHOUSE=OFF -DENABLE_FUZZER_TEST=OFF \
  -DCOMPILER_CACHE=disabled ..
ninja -k 0 clickhouse-local-lib   # catalog build; not linkable yet
```

CHDB_LITE is forced on (as on the Emscripten target).

## Reproduce the wasm64 build attempt (primary)

The wasm64 sysroot is assembled from three parts (all proven together by the
spike: C++ exceptions caught on wasm64 under a Node host):

1. **wasi-libc (fork)**: `github.com/trcrsired/wasi-libc`,
   `make TARGET_TRIPLE=wasm64-wasip1 CC=$WASI_SDK_PATH/bin/clang AR=... NM=...`
   (the final `check-symbols` step fails on clang-version macro drift — ignore;
   the sysroot is already built).
2. **compiler-rt builtins** for wasm64: built from `contrib/llvm-project/compiler-rt/lib/builtins`
   with `-DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON -DCMAKE_C_COMPILER_TARGET=wasm64-wasip1
   -DCOMPILER_RT_BAREMETAL_BUILD=ON`, installed as
   `$WASI_SDK_PATH/lib/clang/<ver>/lib/wasm64-unknown-wasip1/libclang_rt.builtins.a`.
3. **libunwind + libc++abi + libc++** from `contrib/llvm-project/runtimes` with
   `-DCMAKE_SYSTEM_NAME=WASI -DUNIX=ON`, target/sysroot as above, EH flags
   (`-fwasm-exceptions -mllvm -wasm-use-legacy-eh=false -fdeclspec`), threads
   off, exceptions on, `LIBCXX_HAS_MUSL_LIBC=OFF` (its copy_file_range path
   doesn't exist in wasi-libc). Requires two contrib tweaks (in-tree):
   llvm#185770 backport in `libunwind/src/Unwind-wasm.c` (defines the
   `__cpp_exception` tag) and restoring `__locale_dir/locale_base_api/musl.h`
   (pruned from the fork; taken from upstream release/20.x) plus its
   CMakeLists entry.
   Install into the sysroot wasi-sdk-style: headers at
   `include/wasm64-wasip1/c++/v1`, an **empty** `include/c++/v1` (clang's
   version-detection probe), libs into `lib/wasm64-wasip1/`.

Then:

```bash
mkdir buildwasi64 && cd buildwasi64
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/wasi/toolchain.cmake \
  -DWASI_TARGET_TRIPLE=wasm64-wasip1 \
  -DWASI_SYSROOT=<path-to-wasm64-sysroot> \
  ... same flags as the wasm32 attempt ...
ninja -k 0 clickhouse-local-lib
```

## What the port consists of so far

- `cmake/wasi/toolchain.cmake` + `Platform/WASI.cmake` — toolchain integration;
  `cmake/target.cmake` gained a WASI branch that reuses the whole `OS_WASM`
  feature trim and additionally defines `OS_WASI`.
- `cmake/wasi/compat-include/` — POSIX surface wasi-libc lacks (see its
  README). Mirrors what Emscripten's linux-flavored libc provides as built-in
  stubs: signals, sched, termios, pwd/grp, netdb, sockets-that-ENOSYS,
  ucontext-that-aborts, etc.
- A small set of genuine 32-bit fixes in `src/` (64-bit literals in 32-bit
  `size_t`/`long`, `constexpr std::string` exceeding 32-bit SSO capacity,
  `std::variant` integral ambiguity, `ProfileEvents::Count` width,
  `IAST_fwd.h` `stored_size` option, `rusage` field availability).

## Compile-error burn-down (lite set, ~5,440 TUs)

wasm32-wasip1 (official toolchain):

| iteration | failing TUs | change |
|---|---|---|
| 1 (raw) | 2,701 | baseline: no shims, no fixes |
| 2 | 470 | compat headers + 5 header-level 64-bit fixes |
| 3 | 67 | sigset/sockaddr_un/rlimit shims, constexpr-SSO fixes |
| 4–6 | 36 | socket-API stubs, rusage/variant fixes |

wasm64-wasip1 (community sysroot, primary target): **32 failing TUs** on the
first full build with the same shims — the 32-bit-only fixes are inert and the
residue matches the wasm32 tail almost exactly.

The dominant error classes were never "thousands of scattered 64-bitisms":
each iteration collapsed into a handful of header-level root causes, and on
wasm64 the 64-bit question disappears entirely. The remaining ~32 TUs are the
OS-integration tail — terminal (replxx, openpty), raw sockets / interface
enumeration (Poco RawSocket/NetworkInterface, c-ares), signal/timer machinery
(QueryProfiler, BaseDaemon, StackTrace, ThreadFuzzer, CPULeaseAllocation),
libarchive's POSIX disk backends, and the two Emscripten-glue TUs
(ReadBufferFromJSFile, WasmHTTPBridge) — all of which want `OS_WASI` compile-out
guards or cmake exclusion, not shims.

## Remaining work to a running engine (est. order)

1. Finish the compile catalog to zero (mostly guarding Poco Net/socket use
   and the remaining OS-query leaf sites with `OS_WASI`).
2. Entry point: `programs/wasi/` — a WASI *reactor* exporting the same flat C
   API as `programs/wasm/chdb_wasm.cpp` (which wraps `programs/local/chdb.h`);
   no ccall layer needed — hosts call exports directly. The cancel-flag /
   progress-struct address trick carries over unchanged (linear memory).
3. Link: expect missing-symbol whack-a-mole (fibers/ucontext consumers,
   emscripten-glue TUs — `ReadBufferFromJSFile`, `WasmHTTPBridge` — must be
   excluded for `OS_WASI`).
4. Runtime bring-up under wasmtime (dirs preopened for the DB path) and/or
   Node's WASI. Memory ceiling is 4 GiB (wasm32).
5. Decide the 64-bit question with data: whether the remaining size_t
   assumptions are all compile-time visible (this catalog suggests most are)
   or whether runtime issues (>4 GiB datasets, hash-table sizing) demand
   wasm64 — which today means a community wasi-libc fork, a custom libc++/
   libunwind build (recipe proven in the wasm64 spike), a custom host shim
   (`*_wasm64` import ABI — no standard runtime accepts it), and one
   libunwind backport (llvm#185770, already applied in contrib).
