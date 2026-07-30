/* chdb WASI stub: conventional path constants (nothing at these paths in the
 * WASI filesystem sandbox; callers handle open() failure). */
#ifndef CHDB_WASI_COMPAT_PATHS_H
#define CHDB_WASI_COMPAT_PATHS_H

#define _PATH_DEVNULL "/dev/null"
#define _PATH_TTY "/dev/tty"
#define _PATH_BSHELL "/bin/sh"
#define _PATH_TMP "/tmp/"

#endif
