# Pinned source and import receipt — 2026-10-28

SOURCE=rafaelmeloreisnovo/RafGitTools@05b94848c5d3d359b9e02085fa07ef562f8c380a
DESTINATION=rafaelmeloreisnovo/BrowserRaf
DESTINATION_BASE=91d159c78547cb95a48c1dfc78e2a0e2b4895727
BRANCH=port/freestanding-core-from-rafgittools-20261028
COPY_POLICY=source blob equality; no internal code rewrites; keep RafGitTools unchanged
PR_POLICY=DRAFT_HOLD; no auto-merge; no release

| Original file | Source Git blob SHA | New path |
|---|---|---|
| `BrowserRaf/internal/Makefile` | `d97022b5557eb17a3417971cae627dbf1ad35330` | `internal/Makefile` |
| `BrowserRaf/internal/br_dns.h` | `8749a962cafc006e915524d9d48405a6570d13c1` | `internal/br_dns.h` |
| `BrowserRaf/internal/br_entropy.h` | `a2eeef90893aace96cdaf332207bd1fb5fb49839` | `internal/br_entropy.h` |
| `BrowserRaf/internal/br_html.h` | `fc2e25aca7820c337cd60b44c7601e2be1ff3f18` | `internal/br_html.h` |
| `BrowserRaf/internal/br_http.h` | `93df335600260d6140931299a0290bdc002646a2` | `internal/br_http.h` |
| `BrowserRaf/internal/br_main.c` | `7b797ebc27336bb7926d11fae603241c23f6ea5b` | `internal/br_main.c` |
| `BrowserRaf/internal/br_start.S` | `a390af27d0f99f1daa894e0cec88854ddf6707fb` | `internal/br_start.S` |
| `BrowserRaf/internal/br_sys.h` | `0ef5aa2bdc70c57f186864b21872c41572be41c7` | `internal/br_sys.h` |
| `BrowserRaf/internal/br_tls.h` | `67b67b95349ef0e413e1c4d1e3fa06214592a17e` | `internal/br_tls.h` |
| `BrowserRaf/internal/br_types.h` | `317a5e9edde3eaa0aa55754f0424c3ad4b69f12a` | `internal/br_types.h` |
| `LICENSE` | `f0433f024df053a0e11a5b8eac35a9d73da99b70` | `LICENSE` |

**Boundary:** only already-public `BrowserRaf/internal/` files plus the originating `LICENSE`. Do not copy unrelated GaiaPhi files, generated shell archives, private credentials or personal data.

A validated source-object SHA proves exact file content, not runtime safety. CI, ARM ABI/device test, HTTPS trust, independent reviewer and legal completeness remain separate gates. Rollback: close draft PR, discard isolated branch; main remains intact.

## Post-import ABI corrections (successor; never rewrite original provenance)

The original import at commit `381dc68e4eeab257c6fa6e810775aaa183ea5832` passed **11/11 identical Git blob SHAs** against the pinned RafGitTools source. Subsequent isolated verification exposed two architecture-specific assembly issues in the **destination branch only**:

1. ARM GNU-stack assembler section type needs `%progbits`, whereas x86-64 uses `@progbits`. Scoped fix commit `8de2c3377a2f2418e35d8ab2cbb2ab6bb782acd5`.
2. The AArch64 instruction `and sp,sp,#-16` is invalid; Linux ELF process entry already provides 16-byte SP alignment. The invalid operation was removed, with that ABI precondition documented. Scoped fix commit `9d5675cfe1fa7c2dd17ef0e1a31c41453acdd26e`.

Current `internal/br_start.S` has **deviated intentionally** from source blob `a390af27d0f99f1daa894e0cec88854ddf6707fb` to destination blob `dee15ce56700b4dca8a4398ac7198ae96e848d4a`. All other ten imported objects (9 files plus LICENSE) still match their original source Git blob SHAs. No change was made to the RafGitTools source. This note is an append-only successor to the initial transfer receipt; compile-only ABI CI and real-device runtime validation must remain independently bound to an exact branch HEAD.

## S3 — parser security successor (2026-10-28)

- Pinned producer source remains `rafaelmeloreisnovo/RafGitTools@05b94848c5d3d359b9e02085fa07ef562f8c380a`, unchanged.
- New destination-only `internal/br_http.h` fix: invalid HTTP protocol/version or status rejected; duplicate `Content-Length` fails closed. Exact changed blob `054360f7c25a965de6f3be23ca9dba56fdf786bc` supersedes imported blob `93df335600260d6140931299a0290bdc002646a2`; final fix commit `2aa7981856c273c3a39d8742799809f2ae4a1564`.
- `tests/negative_fixtures.c` + `tests/check_negative_fixtures.sh` were newly authored, offline host-only, ASan/UBSan. First targeted run `40651282568` **FAILED as expected** (2 parser defects, 40 checks); corrected exact-head run `40651399476` **SUCCESS** (three jobs: x86-64 host ELF, ARM32/ARM64 object compile, negative fixtures).
- **Current import fidelity = 9/11 original objects byte-identical**, with two documented destination-only fixes (`br_start.S` and `br_http.h`). This supersedes the earlier 10/11 statement for the latest branch without erasing original 11/11 import evidence.
- ABI cross-link, ARM/Android device, full DNS/HTTP robustness, browser rendering security, TLS/PKIX, and independent review remain `TOKEN_VAZIO_NOT_RUN`. `PROMOTION=HOLD; claim_allowed=false`.

## S4 — exact-head ELF links and offline functional exercise (2026-10-28)

- Source pinned to `RafGitTools@05b94848c5d3d359b9e02085fa07ef562f8c380a`; producer main unchanged.
- CI initially **failed as designed** when ARMv7 link attempted: `ld.lld: undefined symbol: __aeabi_uldivmod` (run `40652671580`). Reason: `u64 / 1000000` latency reporting introduced a compiler runtime dependency on ARM32 despite earlier C/ASM object success.
- **Destination-only `br_main.c`** changes: statically bounded bitwise `BR_DIV_U64_U32` with no compiler division runtime, 64 iterations; HTTPS is refused before DNS/socket operations and socket ownership is initialized to `-1` before any early error. No mutation of producer source. Source `br_main.c` blob `7b797ebc27336bb7926d11fae603241c23f6ea5b`; current destination blob `003652a37f3ff539b226289c3abd75a540002b4f`.
- ARM32 and AArch64 **C+ASM linked successfully** as freestanding ELF, with no `INTERP`, `NEEDED`, or named unresolved symbols in the scoped `readelf` check. Run `40652819044` success at code HEAD `f32c0f0af437e617d63f16e8788a20770cea36f1`. Linux/Android ABI and static ELF link are structural tests, **not device execution**.
- Host sanitizer mutation suite: 4,096 deterministic raw inputs (seed `0xBADC0DE`) for URL/HTTP/HTML including truncated and malformed bytes; **8,289 checks / 0 failures**. Additional **1,091 reference arithmetic checks / 0 failures** for `BR_DIV_U64_U32`. Test-only ASan/UBSan and standard-library harnesses are not linked to production ELF.
- Offline real executable host x86-64 loopback integration: three local HTTP exchanges, `Content-Length` and `chunked` both accepted, conflicting duplicate length rejected; the actual executable also rejects `https://127.0.0.1/` before starting DNS/TCP. No internet tests and no claim of general website compatibility.
- Scoped CI three jobs (host ELF + loopback, ARM32/AArch64 compile+ELF link, negative mutations+division) SUCCESS at exact executable/test HEAD `541d1d0d2d0d623f8eaeee0000433f651e9b3bab`, run `40653023809`. Warnings about unused heading color, incomplete TLS routines and Android driver `-no-pie` remain observations, not suppressed.
- Current original import identity = **8/11 objects**; three intentionally modified destination-only code files: `br_start.S`, `br_http.h`, and `br_main.c`. The first import remains **11/11 original blobs** at its pinned Git commit; fidelity must never be relabeled as ongoing byte identity.
- TOKEN_VAZIO_NOT_RUN: physical ARM32/ARM64 Android devices, isolated board-level bare metal, TLS trust/PKIX, arbitrary network compatibility, broad parser fuzz, independent legal/security review.
- POLICY=public DRAFT PR; no merge, release or private credentials; `claim_allowed=false`, `PROMOTION=HOLD`; rollback by closing PR and discarding isolated branch.

## S5 — terminal HTTP navigation + emulated ABI (2026-10-28)

- URL parser supports query strings, fragments and explicit ports. HTTP Host reflects non-default ports. Response framing rejects unsupported/multiple Transfer-Encoding and handles Content-Length completion before peer EOF. Same-origin path-absolute redirect is bounded to four hops. 
- DNS A resolver: connected UDP, getrandom transaction ID, echoed query and bounded answer parser; **plaintext default 8.8.8.8 remains a privacy/reliability gap**. IPv4 parsing rejects ambiguous empty octets and leading zeroes.
- Added `br_links.h` bounded anchor scanner, terminal link listing, optional `--interactive` navigation with history, no heap. Request stderr dumping removed for privacy. `SO_SNDTIMEO` added to the existing `SO_RCVTIMEO` boundary; real Android connect-timeout behavior unverified.
- QEMU is test-only: exact-code HEAD `bd6044cd23478aa335f76a341ad3dcee8c4973d2`, run `40657831493` PASS (four jobs, x86-64 + ARMv7/AArch64 QEMU, parser negative fixtures and cross-linked ELF). Historical same-time source identity: **6/11** original imported blobs still equal.
- Original producer `RafGitTools@05b94848c5d3d359b9e02085fa07ef562f8c380a` unchanged.

## S6 — owner merge and MIME hardening (2026-10-28)

- The owner merged PR#1 at `2026-10-28T08:35:40Z`; public `BrowserRaf/main` moved to merge commit `1e465fe35d234f2f09c98e4b208c21e08bd8b251`. **This merge was not performed by this assistant and must not be confused with a security audit or release.**
- Subsequent branch-only changes: `br_http.h` `HTTP_CONTENT_TYPE` rejects unsupported binary MIME; `br_html.h` literal `TEXT_RENDER` for text/plain with control-character filtering; `br_main.c` routes MIME before rendering; `tests/loopback_http.py` covers binary rejection and literal markup. Other capability gaps remain.
- New successor `BrowserRaf` public **DRAFT PR#2** `https://github.com/rafaelmeloreisnovo/BrowserRaf/pull/2` at code/test HEAD `41f23d8a30ba208bb0191a2cc9ed5ee29cba994c`, scoped CI `40658159150`: **4/4 jobs SUCCESS**. Host ELF passed 13 offline HTTP loopback sessions, including MIME cases and correct early Content-Length. QEMU ARMv7 and AArch64 emulated runtime suites passed. Physical Android **NOT_RUN**.
- Current original-source blob equality **5/11** vs original exact-transfer **11/11 at import**. Unchanged original paths: `internal/Makefile`, `br_entropy.h`, `br_tls.h`, `br_types.h`, `LICENSE`. Intentionally changed on isolated history: `br_dns.h`, `br_html.h`, `br_http.h`, `br_main.c`, `br_start.S`, `br_sys.h`. `br_links.h` is newly authored. This supersedes S4 8/11 and S5 6/11 for the latest branch, preserving both historical receipts by exact ref.
- `PROMOTION=HOLD` for new PR#2; `claim_allowed=false` for production or complete modern browser. `TOKEN_VAZIO_NOT_RUN`: device, independent security/legal review, authenticated TLS, modern HTML5/JS/DOM/CSS, generalized DNS and arbitrary website compatibility. Rollback new PR#2 by closing/reverting successor; do **not** force-reset owner-merged main.
