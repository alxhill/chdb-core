/* Single-threaded pthread mutex implementation, overriding musl's weak
 * definitions (whose owner-tid bookkeeping is broken when no thread pointer
 * is initialized). Layout-independent: only the first two ints of the
 * (much larger) pthread_mutex_t are used.
 *   word0: type (from init attr; PTHREAD_MUTEX_INITIALIZER = zeroed = NORMAL)
 *   word1: lock count
 */
#include <errno.h>
#include <pthread.h>
#include <string.h>

typedef struct
{
    int type;
    int count;
} chdb_mu_t;

int pthread_mutex_init(pthread_mutex_t * m, const pthread_mutexattr_t * attr)
{
    chdb_mu_t * mu = (chdb_mu_t *)m;
    mu->type = attr ? (*(const unsigned *)attr & 3) : 0;
    mu->count = 0;
    return 0;
}

int pthread_mutex_destroy(pthread_mutex_t * m)
{
    memset(m, 0, sizeof(chdb_mu_t));
    return 0;
}

int pthread_mutex_lock(pthread_mutex_t * m)
{
    chdb_mu_t * mu = (chdb_mu_t *)m;
    if (mu->count == 0 || mu->type == PTHREAD_MUTEX_RECURSIVE)
    {
        ++mu->count;
        return 0;
    }
    return EDEADLK; /* one thread relocking a non-recursive mutex */
}

int pthread_mutex_trylock(pthread_mutex_t * m)
{
    chdb_mu_t * mu = (chdb_mu_t *)m;
    if (mu->count == 0 || mu->type == PTHREAD_MUTEX_RECURSIVE)
    {
        ++mu->count;
        return 0;
    }
    return EBUSY;
}

int pthread_mutex_timedlock(pthread_mutex_t * m, const struct timespec * abstime)
{
    (void)abstime;
    return pthread_mutex_lock(m);
}

int pthread_mutex_unlock(pthread_mutex_t * m)
{
    chdb_mu_t * mu = (chdb_mu_t *)m;
    if (mu->count > 0)
        --mu->count;
    return 0;
}
