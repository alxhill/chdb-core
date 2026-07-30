/* chdb WASI stub: no group database in WASI; lookups fail cleanly. */
#ifndef CHDB_WASI_COMPAT_GRP_H
#define CHDB_WASI_COMPAT_GRP_H

#include <sys/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct group
{
    char * gr_name;
    char * gr_passwd;
    gid_t gr_gid;
    char ** gr_mem;
};

static inline struct group * getgrnam(const char * name) { (void)name; return (struct group *)0; }
static inline struct group * getgrgid(gid_t gid) { (void)gid; return (struct group *)0; }
static inline int getgrnam_r(const char * name, struct group * grp, char * buf, size_t buflen, struct group ** result)
{
    (void)name; (void)grp; (void)buf; (void)buflen;
    if (result) *result = (struct group *)0;
    return 0;
}
static inline int getgrgid_r(gid_t gid, struct group * grp, char * buf, size_t buflen, struct group ** result)
{
    (void)gid; (void)grp; (void)buf; (void)buflen;
    if (result) *result = (struct group *)0;
    return 0;
}
static inline int getgrouplist(const char * user, gid_t group, gid_t * groups, int * ngroups)
{
    (void)user; (void)group; (void)groups;
    if (ngroups) *ngroups = 0;
    return -1;
}

#ifdef __cplusplus
}
#endif

#endif
