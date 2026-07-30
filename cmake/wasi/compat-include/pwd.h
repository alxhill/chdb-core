/* chdb WASI stub: no user database in WASI; lookups fail cleanly. */
#ifndef CHDB_WASI_COMPAT_PWD_H
#define CHDB_WASI_COMPAT_PWD_H

#include <sys/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct passwd
{
    char * pw_name;
    char * pw_passwd;
    uid_t pw_uid;
    gid_t pw_gid;
    char * pw_gecos;
    char * pw_dir;
    char * pw_shell;
};

static inline struct passwd * getpwnam(const char * name) { (void)name; return (struct passwd *)0; }
static inline struct passwd * getpwuid(uid_t uid) { (void)uid; return (struct passwd *)0; }
static inline int getpwnam_r(const char * name, struct passwd * pwd, char * buf, size_t buflen, struct passwd ** result)
{
    (void)name; (void)pwd; (void)buf; (void)buflen;
    if (result) *result = (struct passwd *)0;
    return 0;
}
static inline int getpwuid_r(uid_t uid, struct passwd * pwd, char * buf, size_t buflen, struct passwd ** result)
{
    (void)uid; (void)pwd; (void)buf; (void)buflen;
    if (result) *result = (struct passwd *)0;
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif
