# Safety gates — scope-specific

G0 SOURCE: verify each imported file's Git blob SHA against `docs/SOURCE_MANIFEST.md`.
G1 HOST: compile no-libc static ELF and check INTERP/NEEDED absent; this is **structural only**.
G2 ABI: ARM32 and ARM64 real cross builds and Android device operation = TOKEN_VAZIO_NOT_RUN.
G3 PARSERS: malformed URL/DNS/HTTP/HTML, control bytes, truncation, timing/fuzz/ASan/UBSan negative tests = TOKEN_VAZIO_NOT_RUN.
G4 TLS: HTTPS must remain FAIL_CLOSED. TLS handshake, chain trust, hostname, transcript, HKDF, AEAD, Finished and interop are not completed.
G5 RENDER: HTML-to-terminal text is not DOM/CSS/JavaScript browser. No claim of compatibility with SPAs or login.
G6 GOVERNANCE: source and notices checked, third-party rights independently reviewed, PR exact-head CI and rollback required; release remains HOLD.

DNS privacy/anti-poisoning, safe ANSI rendering, HTTP response bounds and return codes require negative fixtures. Static checks alone do not demonstrate secure runtime. A Linux syscall-based binary is not true bare-metal; no unsupported promise of kernel independence.
