# Toolchain file for the WASI (non-Emscripten) WebAssembly target.
#
# Usage:
#   WASI_SDK_PATH=~/code/wasi-sdk-33.0-arm64-macos \
#   cmake -DCMAKE_TOOLCHAIN_FILE=cmake/wasi/toolchain.cmake ...
#
# Differences from the Emscripten target (cmake/target.cmake OS_WASM block):
# no JS glue is generated — the output is a plain .wasm importing the wasip1
# syscall surface, runnable under wasmtime / Node's WASI. wasi-libc replaces
# Emscripten's musl+POSIX-emulation, so the OS surface is narrower.
#
# WASI_TARGET_TRIPLE selects the ABI. wasm32-wasip1 is the official wasi-sdk
# target. wasm64-wasi requires a community sysroot (set WASI_SYSROOT) — see
# WebAssembly/wasi-sdk#185: no official wasm64 target exists.

cmake_minimum_required(VERSION 3.20)

# Provides Platform/WASI.cmake (CMAKE_SYSTEM_NAME=WASI is not known to CMake).
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

# Forward our configuration into try_compile sub-projects (which re-evaluate
# this file with a fresh cache).
set(CMAKE_TRY_COMPILE_PLATFORM_VARIABLES WASI_SDK_PATH WASI_TARGET_TRIPLE WASI_SYSROOT)

set(CMAKE_SYSTEM_NAME WASI)
set(CMAKE_SYSTEM_VERSION 1)

# Cached so build-time cmake regeneration (which does not inherit the
# original environment) keeps working after the first configure.
set(WASI_SDK_PATH "$ENV{WASI_SDK_PATH}" CACHE PATH "wasi-sdk install root")
if(NOT WASI_SDK_PATH)
    message(FATAL_ERROR "Set WASI_SDK_PATH (env or -D) to a wasi-sdk >= 33 install (needs the eh/ sysroot variants).")
endif()

set(WASI_TARGET_TRIPLE "wasm32-wasip1" CACHE STRING "WASI target triple (wasm32-wasip1 | wasm64-wasi)")
if(WASI_TARGET_TRIPLE MATCHES "^wasm64")
    set(CMAKE_SYSTEM_PROCESSOR wasm64)
else()
    set(CMAKE_SYSTEM_PROCESSOR wasm32)
endif()

set(CMAKE_C_COMPILER "${WASI_SDK_PATH}/bin/clang")
set(CMAKE_CXX_COMPILER "${WASI_SDK_PATH}/bin/clang++")
set(CMAKE_ASM_COMPILER "${WASI_SDK_PATH}/bin/clang")
set(CMAKE_AR "${WASI_SDK_PATH}/bin/llvm-ar")
set(CMAKE_RANLIB "${WASI_SDK_PATH}/bin/llvm-ranlib")
set(CMAKE_C_COMPILER_TARGET "${WASI_TARGET_TRIPLE}")
set(CMAKE_CXX_COMPILER_TARGET "${WASI_TARGET_TRIPLE}")
set(CMAKE_ASM_COMPILER_TARGET "${WASI_TARGET_TRIPLE}")

# Custom sysroot (e.g. a wasm64 community sysroot). Default: wasi-sdk's own.
if(DEFINED WASI_SYSROOT AND WASI_SYSROOT)
    set(CMAKE_SYSROOT "${WASI_SYSROOT}")
endif()

# Don't run build-time checks that need to execute target binaries.
set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
