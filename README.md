# httpServer-c

An HTTP/1.1 server written from scratch in C — no web framework, no HTTP library. POSIX sockets, TLS through OpenSSL, and a hand-written request parser.

Built to understand what a web server actually is: how bytes off a socket become a parsed request, where TLS sits in that path, and which parts the operating system hands you versus which parts you write yourself.

## What it does

- Terminates TLS using OpenSSL (`SSL_CTX` configured once, per-connection `SSL` objects)
- Parses HTTP/1.1 request lines and headers by hand — no parsing library
- Serves static files, with content type resolved from the file extension
- Rejects path traversal before touching the filesystem
- Returns real status codes — 400, 403, 404, 405, 500 — from a single status/reason lookup table
- Handles concurrent connections with one process per connection via `fork()`
- Builds at zero warnings under `-Wall -Wextra -Werror`

## Design notes

**Process per connection, not threads.** Each accepted connection is handled in its own process, so a fault in one connection cannot corrupt another's state. The cost is per-connection memory and no shared cache across connections. That trade is worth it here because there is no shared mutable state to coordinate — nothing is gained by threads except the ability to corrupt a neighbour.

**One source of truth for status codes.** The `HttpStatus` enumerators *are* the numeric codes (`HTTP_NOT_FOUND = 404`), so the code prints directly with `%d`, and the reason phrase comes from one table in `httpstatus.c`. Adding a status means adding one row, not editing every call site that might emit it.

**Status rides on the request struct.** `parse_request_function` returns an `HttpRequest` carrying a `status` field rather than signalling failure out of band. The connection handler then has exactly one branch: anything that isn't `HTTP_OK` goes to the error responder, everything else goes to the file responder.

## Building

Requires `gcc`, `make`, and the OpenSSL development headers (`libssl-dev` on Debian and Ubuntu).

```sh
make
```

### Certificates

The server expects a certificate and key in `certs/`. For local development, generate a self-signed pair:

```sh
mkdir -p certs
openssl req -x509 -newkey rsa:2048 -nodes \
  -keyout certs/key.pem -out certs/cert.pem \
  -days 365 -subj "/CN=localhost"
```

`certs/` is gitignored. Private keys do not belong in version control.

## Running

```sh
./build/server
```

The server listens on port 8443. From another terminal:

```sh
curl -k https://localhost:8443/
```

`-k` tells curl to accept the self-signed certificate.

To exercise the error paths:

```sh
curl -ki https://localhost:8443/missing.html                 # 404
curl -ki --path-as-is https://localhost:8443/../etc/passwd   # 403
```

`--path-as-is` is not optional on that second one. curl collapses `..` segments client-side before sending, so without it the server receives an already-normalized path and the traversal check is never tested at all.

## Project layout

```
src/        implementation
include/    headers
static/     files served to clients
certs/      TLS certificate and key (gitignored)
.github/    CI workflow
```

## Known limitations

Tracked here rather than left for someone else to find:

- **GET only.** Other methods are rejected with 405. There is no request body handling.
- **One `SSL_read` per request.** A request split across TCP segments is parsed from whatever arrived in the first read. This needs a loop that reads until the header block is complete.
- **No child reaping.** Forked children are never waited on, so process table entries accumulate over long runs. Fixed by handling `SIGCHLD`.
- **The traversal check is literal.** `strstr(path, "..")` catches the raw form but not percent-encoded variants such as `%2e%2e%2f`. The path needs decoding before the check runs.
- **No keep-alive.** One request per connection, and `Connection: close` is not sent explicitly.

## Deployment

Runs on AWS EC2 under `systemd`. A GitHub Actions workflow builds and deploys on push to `master`.
