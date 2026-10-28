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
