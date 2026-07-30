/* chdb WASI shim: add the terminal-window ioctl bits (no terminals in WASI;
 * callers get -1 from ioctl and fall back to defaults). */
#ifndef CHDB_WASI_COMPAT_SYS_IOCTL_H
#define CHDB_WASI_COMPAT_SYS_IOCTL_H

#include_next <sys/ioctl.h>

#ifdef __wasi__

#ifndef TIOCGWINSZ
#define TIOCGWINSZ 0x5413
#define TIOCSWINSZ 0x5414
struct winsize
{
    unsigned short ws_row;
    unsigned short ws_col;
    unsigned short ws_xpixel;
    unsigned short ws_ypixel;
};
#endif

/* Interface-query ioctls (no interfaces on WASI; ioctl returns -1). */
#ifndef SIOCGIFFLAGS
#define SIOCGIFCONF 0x8912
#define SIOCGIFFLAGS 0x8913
#define SIOCGIFADDR 0x8915
#define SIOCGIFDSTADDR 0x8917
#define SIOCGIFBRDADDR 0x8919
#define SIOCGIFNETMASK 0x891b
#define SIOCGIFMTU 0x8921
#define SIOCGIFHWADDR 0x8927
#define SIOCGIFINDEX 0x8933
#endif

#endif /* __wasi__ */
#endif
