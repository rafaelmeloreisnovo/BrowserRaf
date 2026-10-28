# Multi-ABI + freestanding + bare-metal boundary

Status: proposed architecture constraints and existing scoped evidence, NOT a promise of OS-less browser execution.

## Separation of responsibilities

1. **Core C/ASM** (`internal/br_types.h`, `br_http.h`, `br_html.h`, `br_tls.h`): fixed buffers, deterministic length checks and state transitions; no standard C library and no heap in the shipped binary. Some existing headers include platform headers transitively; a pure-platform split is a future, separately tested refactor.
2. **Linux/Android ABI boundary** (`internal/br_sys.h`, `br_start.S`, `br_entropy.h`): syscalls, ELF entry, socket operations, time and getrandom. Syscall numbers/ABI are architecture-specific and must not be confused with processor-independent C.
3. **Network transport**: DNS, TCP, HTTP/1.1. DNS security, bounded framing, redirects and privacy are incomplete. HTTPS remains fail-closed.
4. **Real bare-metal target**: there is no OS, no Linux syscall table, no process ELF ABI and no kernel entropy provider by default. An explicit board, boot contract, network driver, timing source, entropy/TRNG validation, interrupts, MMU/cache and trust boundary would be required. Only the portable parser/state-machine portion is eligible for reuse after isolating OS dependencies; no bare-metal browser binary is claimed.
5. **Test harness** uses host compiler and sanitizer runtimes for testing only. This does not introduce libc/ASan into the deployed freestanding binary.

## Current ABI evidence

| Target | Source object C + ASM | Host/link ELF | Real device |
|---|---|---|---|
| x86-64 Linux | Host compile PASS_SCOPED | x86-64 static executable, no INTERP/NEEDED PASS_SCOPED | NOT_RUN |
| ARMv7 Android/Linux | C+ASM cross-object compile PASS_SCOPED | LINK_NOT_VERIFIED | NOT_RUN |
| AArch64 Android/Linux | C+ASM cross-object compile PASS_SCOPED | LINK_NOT_VERIFIED | NOT_RUN |
| RISC-V/other | NOT_IMPLEMENTED | NOT_RUN | NOT_RUN |
| Microcontroller/OS-less | NOT_IMPLEMENTED | NOT_RUN | NOT_RUN |

## Reliability gates before promoting any new ABI

- Define target triple, ELF class and ABI including call conventions and alignment.
- Compile all target C/ASM; independently link freestanding ELF and inspect headers and dynamic dependencies.
- Verify physical execution, no accidental libc dependencies, lifecycle, timeouts, error propagation, and deterministic negative tests.
- Validate entropy and private-key handling against hardware/kernel trust, or **fail closed**. Never synthesize security randomness from clock, addresses, CRC or geometric states.
- Keep TLS and credentials disabled without authenticated end-to-end cryptography. A self-written ClientHello and tests of byte layout are not evidence of secure transport.

## Observed follow-up

- Negative fixtures reproduce and then verify two hardened HTTP behaviors (invalid HTTP versions; duplicate Content-Length). See exact source and CI receipt on PR #1.
- Other parser, DNS, HTTP framing, render, TLS/PKIX, device and platform validation gaps remain open; test warnings are retained.
- Rollback: discard the isolated draft branch; the original RafGitTools producer main and BrowserRaf public main are unchanged.

`SOURCE ≠ ARTIFACT ≠ EXECUTION ≠ EVIDENCE ≠ CLAIM`.
