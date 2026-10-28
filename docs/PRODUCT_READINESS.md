# BrowserRaf — audited product-readiness gates (2026-10-28)

## Product boundary

The delivered artifact is an experimental **HTTP/1.1 text terminal browser** with C/ASM freestanding Linux/Android kernel-syscall ABIs. **It is not yet a full modern web browser, a TLS-secure browser, a bare-metal board image, or a reviewed Android APK.** Code appearing in a public DRAFT PR is not an authorized production release.

## Verified in GitHub-hosted CI (scope-limited)

| Gate | Current verified category | Evidence |
|---|---|---|
| x86-64 static no-libc ELF, no interpreter/NEEDED | PASS_SCOPED | `host-elf` |
| ARMv7/AArch64 C+ASM compilation and ELF link | PASS_SCOPED | `cross-abi-objects` |
| x86-64 actual HTTP loopback GET, valid/invalid encoding, redirect, interactive selection | PASS_SCOPED | `tests/loopback_http.py` |
| ARMv7/AArch64 executable under QEMU user emulation, same offline network tests | PASS_SCOPED_EMULATED | `arm-emulation` with QEMU test-only dependency |
| Deterministic malformed URL/HTTP/DNS/HTML/link input and integer arithmetic vectors | PASS_SCOPED | `negative-fixtures` with host ASan/UBSan |
| HTTPS fail-closed before DNS/connect | PASS_SCOPED | executable negative test, including emulation |
| Non-default Host port, strict IPv4 and DNS response bounds | PASS_SCOPED | offline fixtures and real loopback |
| No real on-device Android build/run | TOKEN_VAZIO_NOT_RUN | offline no-root script `tests/check_termux_device.sh` prepared but not executed |
| True OS-less bare metal | NOT_IMPLEMENTED | requires target board + drivers + entropy hardware |
| Independent security/license review | TOKEN_VAZIO_NOT_RUN | not a certification |

The repository has no connection to a physical Android device in this session; **QEMU/Linux != Android OS/SELinux/ABI device evidence**.

## Release-blocking defects / capabilities

| Severity | Capability or threat | Current state | Required acceptance |
|---|---|---|---|
| P0 | Trusted HTTPS with certificate validation, hostname verification, AEAD, protocol-level error handling, trust-store lifecycle, pinning policy | FAIL_CLOSED_NOT_IMPLEMENTED | Independent test vectors, interop, certificate negatives, review. Do not enable partial ClientHello |
| P0 | DNS trust/privacy: hardcoded plaintext 8.8.8.8; no DNSSEC/DoT/DoH; provider routing and privacy defaults | OPEN | Configurable policy/opt-in plus tests and a clear privacy notice; no silent data egress |
| P0 | Physical Android ARM32/ARM64 run and Android app integration | TOKEN_VAZIO_NOT_RUN | Native-on-device log, executable SHA-256, OS/kernel/API/ABI proof, rollback and user consent |
| P1 | Network body size >64KiB and streaming, general transfer codings, MIME/content type, charset handling, connection retry/cancel | PARTIAL | Bounded streaming state machine with corpus tests and backpressure |
| P1 | Full redirect RFC semantics / relative and cross-origin navigation policy | PARTIAL | Origin checks, loop detection, cycle limit, cross-origin/HTTPS consent and negative tests |
| P1 | Terminal rendering/security: complete HTML parsing, UTF-8, accessible link labels, forms, DOM, CSS, multimedia | PARTIAL | Design-specific explicit compatibility matrix, internationalization and accessibility tests |
| P1 | Deterministic DNS resolver portability, timeout behavior under unreachable hosts and malformed traffic | PARTIAL | Lab network tests incl. simulated timeouts, packet fuzz, Android resolver configuration and origin verification |
| P1 | Continuous libFuzzer/AFL corpus, cross-platform sanitizers, independent safety audit | PARTIAL | CI fuzz receipts, coverage and triage with no suppressed warnings |
| P2 | Memory history / persistent bookmarks, private browsing, downloads/storage lifecycle | PARTIAL | Explicit storage privacy policy and opt-in persistence; safe deletion, metadata provenance and rollback |
| P2 | Bare-metal board bring-up | NOT_IMPLEMENTED | Specific hardware target, boot ABI, clocks, TRNG, network driver, page table, isolation, verified device receipt |

## Invariants

- Source pinned `rafaelmeloreisnovo/RafGitTools@05b94848c5d3d359b9e02085fa07ef562f8c380a`, and public `BrowserRaf/main` unchanged by the isolated DRAFT.
- Exact initial import was **11/11 identical Git blobs**. After current branch fixes, original imported files retaining identical hashes are **6/11**: `Makefile`, `br_entropy.h`, `br_html.h`, `br_tls.h`, `br_types.h` and `LICENSE`. Modified source files: `br_dns.h`, `br_http.h`, `br_main.c`, `br_start.S`, `br_sys.h`; `br_links.h` is newly authored. Earlier S2/S3/S4 receipts remain historically true for their pinned commits.
- No secrets, private code, automatic merge or release; `PROMOTION=HOLD`; `claim_allowed=false` for complete/secure production browser.
- Rollback: close unmerged PR/discard candidate branch; no changes to producer and destination main.
- Strict failure semantics: `TOKEN_VAZIO` is not zero and `IMPLEMENTED_UNTESTED` is not `PASS`.

## R3

`F_ok`: scoped native host + ARM simulated runtime, HTTP text navigation, negative framing checks, audited provenance.

`F_gap`: physically observed Android, HTTPS trust, modern browser capabilities, bounded streaming and independent review.

`F_next`: secure TLS architecture decision (platform-provided TLS adapter versus independently audited implementation), Android device test, proper DNS privacy policy, wider interop and controlled release gate.
