#!/usr/bin/env python3
"""Offline host integration: real static ELF against loopback-only HTTP fixtures.
Test harness uses Python stdlib; BrowserRaf production binary does not."""
import socket
import subprocess
import sys
import threading

BIN = sys.argv[1]


def case(name, response, expected_success):
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(("127.0.0.1", 0))
    srv.listen(1)
    srv.settimeout(5)
    port = srv.getsockname()[1]
    problems = []

    def serve():
        try:
            conn, _ = srv.accept()
            with conn:
                conn.settimeout(4)
                request = bytearray()
                while b"\r\n\r\n" not in request and len(request) < 2048:
                    data = conn.recv(2048)
                    if not data:
                        break
                    request.extend(data)
                if not request.startswith(b"GET /probe HTTP/1.1\r\n"):
                    problems.append("invalid GET wire syntax")
                if b"\x00" in request:
                    problems.append("NUL leaked into HTTP request")
                conn.sendall(response)
        except Exception as exc:
            problems.append(str(exc))
        finally:
            srv.close()

    t = threading.Thread(target=serve, daemon=True)
    t.start()
    try:
        p = subprocess.run([BIN, "http://127.0.0.1:%d/probe" % port],
                           capture_output=True, timeout=8, check=False)
    finally:
        t.join(timeout=6)
    out = (p.stdout + p.stderr).decode("utf-8", errors="replace")
    good = "[OK] Fetch completo" in out
    if problems or good != expected_success or t.is_alive():
        raise AssertionError("%s expected_success=%s got=%s; server=%s; output=%s" %
                             (name, expected_success, good, problems, out[-1800:]))
    print("PASS_LOOPBACK %s expected_success=%s" % (name, expected_success))


case("content_length", b"HTTP/1.1 200 OK\r\nContent-Length: 9\r\nConnection: close\r\n\r\nOK-LOCAL!", True)
case("chunked", b"HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\nConnection: close\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n0\r\n\r\n", True)
case("duplicate_length", b"HTTP/1.1 200 OK\r\nContent-Length: 3\r\ncontent-length: 9\r\n\r\nBAD", False)
print("PASS_SCOPED 3 real ELF loopback HTTP sessions; no external DNS or internet")
