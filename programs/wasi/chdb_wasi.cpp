// WASI CLI entry point for chdb.
//
// Uses the connection-based C API (chdb_connect/chdb_query), the same code
// path as the Emscripten module (programs/wasm/chdb_wasm.cpp) — NOT the
// query_stable_v2 CLI-client path, whose ClientApplicationBase spins up a
// fatal-signal listener thread that cannot exist on the single-threaded WASI
// build. Runnable by any wasip1 host:
//
//     wasmtime run --dir=<host-dir>::/chdb chdb.wasm "SELECT 1" [format]
//
// The engine keeps its working data under /chdb/data (preopen a writable
// host directory at /chdb).

#include "../local/chdb.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include <unistd.h>

#include <Poco/AutoPtr.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/Logger.h>

static void apply_single_threaded_settings(chdb_connection conn)
{
    // Mirrors chdb_wasm_apply_settings for the single-threaded build.
    chdb_result * r = chdb_query(conn, "SET max_threads = 1", "Null");
    if (r)
        chdb_destroy_query_result(r);
    r = chdb_query(conn, "SET max_parsing_threads = 1, input_format_parquet_enable_row_group_prefetch = 0", "Null");
    if (r)
        chdb_destroy_query_result(r);
}

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        std::fprintf(stderr, "usage: chdb.wasm <sql> [output-format] [--path=/chdb/data]\n");
        return 2;
    }
    const char * sql = argv[1];
    const char * format = argc > 2 ? argv[2] : "PrettyCompactNoEscapes";

    // chdb_connect swallows exceptions into LOG_ERROR; give the root logger a
    // console channel so they reach stderr. Level via env (wasmtime --env
    // CHDB_LOG_LEVEL=trace ...), default: errors only.
    Poco::AutoPtr<Poco::ConsoleChannel> log_channel(new Poco::ConsoleChannel);
    Poco::Logger::root().setChannel(log_channel);
    const char * log_level = std::getenv("CHDB_LOG_LEVEL");
    Poco::Logger::root().setLevel(log_level ? log_level : "error");

    char arg0[] = "clickhouse";
    char path_arg[] = "--path=/chdb/data";

    // argv[3..] are forwarded verbatim to the embedded server (e.g.
    // --log-level=trace, --path=..., settings), so behavior can be tuned
    // without relinking. The default path is added only if none is given.
    std::vector<char *> conn_args;
    conn_args.push_back(arg0);
    bool have_path = false;
    for (int i = 3; i < argc; ++i)
    {
        if (std::strncmp(argv[i], "--path", 6) == 0)
            have_path = true;
        conn_args.push_back(argv[i]);
    }
    if (!have_path)
        conn_args.push_back(path_arg);

    chdb_connection * conn = chdb_connect(static_cast<int>(conn_args.size()), conn_args.data());
    if (!conn)
    {
        std::fprintf(stderr, "chdb: connect failed\n");
        return 1;
    }
    apply_single_threaded_settings(*conn);

    chdb_result * res = chdb_query(*conn, sql, format);
    int rc = 0;
    if (!res)
    {
        std::fprintf(stderr, "chdb: null result\n");
        rc = 1;
    }
    else
    {
        const char * err = chdb_result_error(res);
        if (err)
        {
            std::fprintf(stderr, "chdb error: %s\n", err);
            rc = 1;
        }
        else if (chdb_result_buffer(res))
        {
            std::fwrite(chdb_result_buffer(res), 1, chdb_result_length(res), stdout);
            std::fprintf(stderr, "\n[chdb] rows_read=%llu elapsed=%.3fs\n",
                         (unsigned long long)chdb_result_rows_read(res), chdb_result_elapsed(res));
        }
        chdb_destroy_query_result(res);
    }
    chdb_close_conn(conn);

    /// Skip static destructors and atexit hooks: the engine's shutdown
    /// machinery expects worker threads that never existed on this build and
    /// spins. Results are already flushed; terminate via proc_exit, as
    /// clickhouse-local does for fast shutdown.
    std::fflush(stdout);
    std::fflush(stderr);
    _exit(rc);
}
