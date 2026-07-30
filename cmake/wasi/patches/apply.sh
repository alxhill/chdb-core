#!/bin/sh
# Apply the contrib patches the WASI port needs.
#
# These live here rather than in submodule forks: they are a handful of lines
# against three upstream repos (see each patch's Subject/body), and vendoring
# them keeps .gitmodules pointing at the same URLs everyone else uses.
#
# Run after `git submodule update --init --recursive`, before configuring a
# WASI build. Idempotent — re-running on an already-patched tree is a no-op.
#
#   ./cmake/wasi/patches/apply.sh          apply
#   ./cmake/wasi/patches/apply.sh -R       revert
#
# See cmake/wasi/STATUS.md for where each patch is consumed.

set -eu

here=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
root=$(git -C "$here" rev-parse --show-toplevel)

# Direction of the action, and of the "is it already in the target state?"
# probe — which is always the opposite one.
case "${1-}" in
    "")
        action=""
        probe="-R"
        verb="applied"
        ;;
    -R | --revert)
        action="-R"
        probe=""
        verb="reverted"
        ;;
    *)
        echo "usage: $0 [-R]" >&2
        exit 2
        ;;
esac

# <patch file>:<submodule path>
patches="0001-double-conversion-wasm64.patch:contrib/double-conversion
0002-arrow-vendored-double-conversion-wasm64.patch:contrib/arrow
0003-llvm-libunwind-cpp-exception-tag.patch:contrib/llvm-project
0004-llvm-libcxx-restore-musl-locale-header.patch:contrib/llvm-project"

status=0

for entry in $patches; do
    patch=${entry%%:*}
    sub=${entry#*:}
    label=$(printf '%-46s -> %-26s' "$patch" "$sub")

    if [ ! -e "$root/$sub/.git" ]; then
        echo "$label MISSING (run: git submodule update --init $sub)"
        status=1
        continue
    fi

    if git -C "$root/$sub" apply --check $probe "$here/$patch" 2>/dev/null; then
        echo "$label already $verb"
        continue
    fi

    if git -C "$root/$sub" apply $action "$here/$patch" 2>/dev/null; then
        echo "$label $verb"
    else
        echo "$label FAILED"
        git -C "$root/$sub" apply $action "$here/$patch" || true
        status=1
    fi
done

exit $status
