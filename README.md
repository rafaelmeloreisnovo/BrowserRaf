# BrowserRaf — freestanding terminal HTTP browser (experimental)

**Status: PR #1 was merged to main by the repository owner on 2026-10-28; new security successor PR #2 is DRAFT/HOLD. A merged commit is not a security certification or a production release.**

BrowserRaf is a **text-mode HTTP/1.1 browser** with a C/Assembly core, fixed-size static buffers, no libc/malloc/GC in the produced executable, and separate Linux/Android syscall ABIs for x86-64, ARMv7 and AArch64. It is **not** a WebView/Chromium-class browser: no DOM/CSS layout engine, JavaScript, cookies, forms, WebSockets or image renderer. **HTTPS is deliberately FAIL-CLOSED**, because the TLS 1.3 ClientHello fragment is not a trusted, authenticated TLS client.

## Current implemented subset

- Parse bounded `http://` URLs including paths, query strings, fragments and explicit ports; reject unsupported schemes and ambiguous hosts.
- IPv4 literals or DNS A lookups; DNS uses connected UDP, a getrandom-generated transaction ID, exact question echo and bounded record parser. **Resolver is still hardcoded to 8.8.8.8, plaintext and non-DNSSEC; DNS privacy and configuration remain open.**
- HTTP/1.1 GET with correct `Host` port, no connection pooling, no cookies, full exact `Content-Length` framing, strict single `chunked` transfer support, no implicit gzip/other codings. Explicit `text/html`/`text/plain` MIME dispatch; non-text responses fail closed. Maximum response buffer is currently 65,536 bytes.
- 301/302/303/307/308 redirect support **only for same-origin absolute paths** (`/next`), at most four hops. Cross-origin, HTTPS and scheme-relative redirects are intentionally denied until separately implemented.
- Text/ANSI HTML rendering, script/style raw source suppression; print up to 24 sanitized anchor URLs. `--interactive` supports numeric link selection, relative links and 8-page volatile back history. No persistence.
- Receive and send kernel timeouts requested for TCP; connection-timeout behavior on physical Android still requires verification.

## Run and test

On a compatible Linux x86-64 host with Clang:

```sh
sh tests/check_host_freestanding.sh
sh tests/check_cross_abi_objects.sh
sh tests/check_negative_fixtures.sh
```

The host smoke test exercises a real no-libc ELF against offline loopback HTTP servers. The negative harness uses ASan/UBSan **only in the test executable**. CI also runs QEMU ARM32/ARM64 tests with a CI-only QEMU package; emulation is not physical Android evidence.

To browse from a **checked-out source tree** with a suitable native compiler and Linux kernel:

```sh
clang -O2 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -static \
  -Wl,-e,_start -Wl,-z,noexecstack \
  -Iinternal internal/br_start.S internal/br_main.c -o browserraf
./browserraf http://127.0.0.1:8080/
./browserraf http://127.0.0.1:8080/ --interactive
```

On **Android Termux**, from that source tree, perform the **no-root, offline, native build/safety smoke** with one command:

```sh
sh tests/check_termux_device.sh
```

This creates only a scoped local receipt and does **not** claim HTTP behavior on the device; it has not been executed on a physical phone in this PR.

## Source and release boundary

The initial port copied 11/11 original Git blobs identically from [RafGitTools BrowserRaf](https://github.com/rafaelmeloreisnovo/RafGitTools/tree/05b94848c5d3d359b9e02085fa07ef562f8c380a/BrowserRaf/internal), and only the isolated candidate branch was subsequently hardened. The producer remains unchanged. The initial import and previous hardening were merged by the owner in PR#1. The newer MIME security candidate lives on PR#2 and requires its own exact-head CI and review. See [SOURCE_MANIFEST](docs/SOURCE_MANIFEST.md), [MULTI_ABI_BAREMETAL_BOUNDARY](docs/MULTI_ABI_BAREMETAL_BOUNDARY.md), [SAFETY_GATES](docs/SAFETY_GATES.md) and [PRODUCT_READINESS](docs/PRODUCT_READINESS.md).

Original copied LICENSE is GPL-3.0-or-later; complete legal/contributor review is pending. This is **freestanding Linux/Android user space using kernel syscalls**, not OS-less bare metal. A real bare-metal board needs its own storage/clock/network/entropy/boot port.

**SOURCE ≠ ARTIFACT ≠ EXECUTION ≠ EVIDENCE ≠ CLAIM. PROMOTION=HOLD.**
