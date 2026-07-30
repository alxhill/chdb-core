/* Link-level compatibility stubs for the WASI build: symbols referenced from
 * already-compiled code where wasi-libc has no definition. All are safe
 * single-threaded/no-signals degradations. */

#include <errno.h>
#include <stddef.h>
#include <string.h>

/* Single process: advisory file locks trivially succeed. */
int flock(int fd, int operation)
{
    (void)fd;
    (void)operation;
    return 0;
}

int getlogin_r(char * buf, size_t bufsize)
{
    const char * name = "default";
    if (bufsize < strlen(name) + 1)
        return ERANGE;
    strcpy(buf, name);
    return 0;
}

/* thread_local destructors: one thread, so process-exit destruction is
 * equivalent. */
extern int __cxa_atexit(void (*func)(void *), void * arg, void * dso_handle);
int __cxa_thread_atexit(void (*func)(void *), void * arg, void * dso_handle)
{
    return __cxa_atexit(func, arg, dso_handle);
}

/* No frame-walking unwinder API on wasm (the VM owns the stack); report zero
 * frames — StackTrace prints an empty trace. */
int unw_backtrace(void ** buffer, int size)
{
    (void)buffer;
    (void)size;
    return 0;
}

/* Thread creation is unavailable (single-threaded WASI build): report EAGAIN,
 * which std::thread turns into a catchable std::system_error — the engine's
 * CHDB_WASM_SINGLE_THREADED paths degrade to inline execution on exactly this
 * signal, as on the single-threaded Emscripten build. */
int pthread_create(void * thread, const void * attr, void * (*start)(void *), void * arg)
{
    (void)thread;
    (void)attr;
    (void)start;
    (void)arg;
    return EAGAIN;
}
int pthread_join(void * thread, void ** retval)
{
    (void)thread;
    (void)retval;
    return EINVAL; /* no thread was ever created */
}
int pthread_detach(void * thread)
{
    (void)thread;
    return EINVAL;
}
int pthread_getattr_np(void * thread, void * attr)
{
    (void)thread;
    (void)attr;
    return ENOSYS; /* checkStackSize falls back to not knowing the bounds */
}

/* No network interfaces on WASI. */
struct ifaddrs;
int getifaddrs(struct ifaddrs ** ifap)
{
    (void)ifap;
    errno = ENOSYS;
    return -1;
}
void freeifaddrs(struct ifaddrs * ifa)
{
    (void)ifa;
}

int gethostname(char * name, size_t len)
{
    const char * host = "localhost";
    if (len < strlen(host) + 1)
    {
        errno = ENAMETOOLONG;
        return -1;
    }
    strcpy(name, host);
    return 0;
}

/* No fork, so atfork handlers can never fire. */
int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
    (void)prepare;
    (void)parent;
    (void)child;
    return 0;
}

/* Advisory only. */
int posix_madvise(void * addr, size_t len, int advice)
{
    (void)addr;
    (void)len;
    (void)advice;
    return 0;
}

/* No dynamic loading in a wasm module. */
void * dlopen(const char * filename, int flags)
{
    (void)filename;
    (void)flags;
    return NULL;
}
void * dlsym(void * handle, const char * symbol)
{
    (void)handle;
    (void)symbol;
    return NULL;
}
int dlclose(void * handle)
{
    (void)handle;
    return 0;
}
char * dlerror(void)
{
    return (char *)"dynamic loading is not available on WASI";
}
