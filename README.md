# BrowserRaf — freestanding C/Assembly browser core

**Status: isolated public port candidate / DRAFT / HOLD.** This is the existing BrowserRaf core, not a rewrite. It is an experimental HTTP/1.1 text-mode browser; **HTTPS is fail-closed**, not TLS 1.3 authenticated. It is not yet a general-purpose browser: JavaScript, DOM/CSS, cookies/WebSocket and modern web-app compatibility are not implemented.

## Provenance and architecture

Existing implementation comes from [RafGitTools/BrowserRaf/internal](https://github.com/rafaelmeloreisnovo/RafGitTools/tree/05b94848c5d3d359b9e02085fa07ef562f8c380a/BrowserRaf/internal) pinned at `05b94848c5d3d359b9e02085fa07ef562f8c380a`. The original source stays in place, unmodified. See [SOURCE_MANIFEST](docs/SOURCE_MANIFEST.md) for per-file Git object identity.

The core is C/Assembly, no malloc, libc or GC, with code paths for ARM32, ARM64 and x86-64. It is a **freestanding Linux/Android userspace process using syscalls**, not OS-less bare metal. No network credential or API-token integration is proposed. Read [SAFETY_GATES](docs/SAFETY_GATES.md) before executing or promoting.

## Test

On a compatible x86-64 host with Clang and binutils: `sh tests/check_host_freestanding.sh`. The test makes **no network request** and checks host compilation plus ELF dependency boundaries. It does not validate ARM device behavior, HTTP parser security, or TLS.

## Licensing and release

The imported source is supplied under the existing RafGitTools GPL-3.0-or-later notice; LICENSE is copied unchanged from the source. Third-party contributor and legal review remain a release gate. No automatic merge, production build, or security/HTTPS claim.

`SOURCE != ARTIFACT != EXECUTION != EVIDENCE != CLAIM`; `claim_allowed=false`.
