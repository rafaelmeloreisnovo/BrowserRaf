# Pixel Manifold S7 — gates, semantic types and rollback

## Scope and authority
This is a **data-only, freestanding CPU kernel**, not the BrowserRaf PNG/JPEG/SVG decoder or HTML graphics compositor. Branch `feat/pixel-manifold-freestanding-s7-20261028` is isolated from main `a5f905ebfb5310e1ae6b8a1b18567d5f09a60c6a` at creation, DRAFT/HOLD. All user's original image bytes remain outside the public repo; do not publish Drive file IDs, thumbnails, private metadata or texts without a separate publication decision.

Original source of math semantics: author's **RAFAELIA — PIXEL MANIFOLD SCHEMA + GOLDEN VECTORS V1 (2026-09-26)**. Exact V1 luminance: `(54R+183G+19B)>>8`. Descriptors: RGB mean, Y mean, Y variance, L1 gradient, 5 quantized keys and samples. All coordinates are **image pixel coordinates** and 16×16 tiles are **not 16 dimensions**. Symbols and narratives are provenance-bearing labels, not algorithms without defined contracts.

## Deterministic contracts

| Layer | Type | Measurable invariant | State |
|---|---|---|---|
| Raster | RGB8, RGBA8 | no out-of-bounds, alpha copied exactly, 4 quarter turns = identity | TESTED_SCOPED |
| Gray | Y8 | `(54R+183G+19B)>>8` under the author's fixed integer convention | TESTED_SCOPED |
| PB | byte 0/1 | threshold selected by user (default test 128); 0 is measured black, not missing | TESTED_SCOPED |
| CMYK | C8M8Y8K8 idealized | exact 8-bit RGB round-trip with defined rounding, not color-managed proof | TESTED_SCOPED_NUMERIC |
| Geometry | bbox + centroid Q8 | 4-turn raster identity; positive mask count and centroid; measured empty is valid | TESTED_SCOPED |
| Tiles | 16×16 descriptors | stable reference golden vectors and no heap/libc/syscalls/float | TESTED_SCOPED |
| Print reproduction | ICC/profile/illuminant | substrate/ink-specific calibrated ΔE / spectral proof | TOKEN_VAZIO_UNMEASURED |
| Original image-to-vector topology | SVG or author control points | equivalent geometry is not proof of identical source vectors | TOKEN_VAZIO_UNIDENTIFIABLE |
| Decoding actual PNG in BrowserRaf | PNG/JPEG/WebP etc | image header/authenticity/limits/CRC/zlib and integrated renderer | NOT_IMPLEMENTED |
| True OS-less bare metal | board-specific startup/drivers | specific board + pixel output + entropy + I/O physical receipt | NOT_IMPLEMENTED |
| Device Android | ABI/OS physical kernel | test script on physical device, SHA256 and OS model | TOKEN_VAZIO_NOT_RUN |

**Strict null semantics:** `∅ != 0 != "0" != "" != NULL != "    "`. `TOKEN_VAZIO_UNKNOWN` (unidentified), `TOKEN_VAZIO_UNMEASURED` (known but unmeasured), `TOKEN_VAZIO_UNIDENTIFIABLE` (not inferable from raster), `TOKEN_VAZIO_NOT_RUN` (not executed), `TOKEN_VAZIO_CONTRADICTORY` (mutually inconsistent) are **states**, never silently collapsed to zero.

## Semantic Ω mapping (non-normative)
- `Ω00..Ω15` are **user-authored sample IDs**, not opcodes, language translations or universal archetypes.
- `molho` has several Portuguese senses (food sauce, bundle of keys, informal variants) and cannot be collapsed without linguistic context; other scripts and scripts directions must be handled without forced equivalence.
- `ΩΠμ[∆ª&∆ⁿ&∆º&→(#;#;#)]` is a **symbolic annotation** with undefined field grammar. `TOKEN_VAZIO_UNIDENTIFIABLE` until a formal schema specifies each axis, unit, coordinate system, orientation, and allowed conversion.
- The *Six Parables / Twelve Workshops* are useful authorial metaphors for evidence closure and interdisciplinary invariants; no inferred historical transmission between Hebrew, Hellenic, Indian, Chinese, African, or ancestral traditions. Respect source cultural autonomy and consent.

## Security and provenance gates
1. **G0 Source**: pinned RGB bytes + raw file hash retained privately, no media copied to public fork; images contain transparency and original ICC may be absent.
2. **G1 Arithmetic**: deterministic host ASan/UBSan checks, bounds/overflow guards, all 8-bit channel reference cases and negative invalid inputs.
3. **G2 ABI**: cross-compile ARM32/AArch64 freestanding C, no host-test sanitizers shipped. ABI compile ≠ ARM runtime.
4. **G3 Raster**: rotate RGB/RGBA without interpolation or alpha changes; exact coordinate mapping in every quadrant.
5. **G4 Operational**: P0 release blockers (HTTPS verified TLS/PKIX, DNS privacy, real devices, independent review) remain separate; this library does not waive them.
6. **G5 Governance**: Zero Trust, least privilege, integrity receipts and typed uncertainty; controls align in spirit with NIST SSDF, ISO/IEC 27001 and ISO 8000, **not certified compliance**. Brazilian LGPD and childhood protections require data minimization/consent where data is involved.

## CI evidence and closure
- Github Actions `pixel-manifold` job: compile the scalar kernel with `clang -Werror` and run `tests/pixel_vectors.c` under ASan + UBSan; compile ARM32 and ARM64 probe objects with `-ffreestanding -nostdlib -Werror`.
- Baseline: `BrowserRaf/main@a5f905ebfb5310e1ae6b8a1b18567d5f09a60c6a`. Old four CI jobs must remain green; new fifth job must pass without muting warnings.
- **Rollback**: close the unmerged DRAFT, or revert the isolated S7 commits if the owner merges them; never force-reset main or delete original image sources.
- `CLAIM_ALLOWED`: kernel numeric/geometry test scope only. `CLAIM_ALLOWED_FULL_BROWSER=false`, `RELEASE=HOLD`.

R3 = `<F_ok: algebraic RGB/CMYK/PB and bounded geometry in freestanding C with exact golden vectors; F_gap: original raster-to-vector provenance, ICC, PNG decoding, physical Android/bare-metal board and release review; F_next: authored small decoder and byte-exact offline image harness, defined Ω field grammar, device ABI runtime and independent release gate>`.
