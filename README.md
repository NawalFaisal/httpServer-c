# http-server-c

A production HTTP/1.1 server built from scratch in C.

## What it does
- Handles HTTP requests — GET, POST, PUT, DELETE
- Serves static files from disk
- Proper error handling — 400, 403, 404, 405, 500
- Multi-process request handling via fork()
- Deployed on AWS EC2 with automated CI/CD

## Tech
- Language: C
- OS: Linux
- Cloud: AWS EC2
- CI/CD: GitHub Actions
- Process management: systemd

## Build
```bash
mkdir -p build && make
./build/server
```

## Project Structure
```
src/        — source files
include/    — header files
static/     — HTML files served by the server
docs/       — documentation
.github/    — GitHub Actions CI/CD workflow
```

## Live
http://35.182.108.2:8080


## Author
Nawal — Systems engineer in progress
Building from the ground up