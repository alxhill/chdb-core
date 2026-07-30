# WASI entry module for chdb: a plain wasip1 command (chdb.wasm) wrapping the
# chdb C API. No JS glue — any standard WASI host can run it (wasmtime, Node).

clickhouse_add_executable (chdb_wasi chdb_wasi.cpp wasi_compat_stubs.c wasi_pthread_mutex.c)

target_link_libraries (chdb_wasi PRIVATE clickhouse-local-lib)

# Poco logger setup in main() (chdb_connect logs errors instead of returning them).
target_include_directories (chdb_wasi SYSTEM PRIVATE
    "${ClickHouse_SOURCE_DIR}/base/poco/Foundation/include"
    "${ClickHouse_SOURCE_DIR}/base/poco/XML/include"
    "${ClickHouse_SOURCE_DIR}/base/poco/Util/include")

set_target_properties (chdb_wasi PROPERTIES
    OUTPUT_NAME chdb
    SUFFIX ".wasm"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

target_link_options (chdb_wasi PRIVATE
    # The exceptions-enabled unwinder; resolves after the objects that need it.
    -lunwind
    # ClickHouse recurses deeply (parser, planner); default WASI stacks are 1MB.
    "SHELL:-Wl,-z,stack-size=8388608"
    # Deep template instantiations produce >64Ki two-byte-LEB locals in a few
    # functions under -O0-ish codegen; let wasm-ld not fail on signature checks.
    -Wl,--no-check-features
    )
