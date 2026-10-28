#!/bin/sh
# Compile-only ABI check: no Android SDK, runtime, libc, or network needed.
set -eu
command -v clang >/dev/null 2>&1 || { echo "MISSING_TOOL clang"; exit 2; }
command -v readelf >/dev/null 2>&1 || { echo "MISSING_TOOL readelf"; exit 2; }
cd "$(dirname "$0")/.."
d="$(mktemp -d)"
trap 'rm -rf "$d"' EXIT HUP INT TERM
for triple in armv7a-linux-androideabi21 aarch64-linux-android28; do
  case "$triple" in
    armv7a-*) arch=ARM ;;
    aarch64-*) arch=AArch64 ;;
    *) echo "INTERNAL_ERROR unsupported target"; exit 1 ;;
  esac
  clang --target="$triple" -O2 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib \
    -Wall -Wextra -Iinternal -c internal/br_main.c -o "$d/$triple-main.o"
  clang --target="$triple" -ffreestanding -nostdlib \
    -c internal/br_start.S -o "$d/$triple-start.o"
  readelf -h "$d/$triple-main.o" | grep -q "Machine:.*$arch" || { echo "FAIL target main ABI $triple"; exit 1; }
  readelf -h "$d/$triple-start.o" | grep -q "Machine:.*$arch" || { echo "FAIL target ASM ABI $triple"; exit 1; }
  echo "PASS_SCOPED_COMPILE_ONLY $triple: C and ASM objects"
done
