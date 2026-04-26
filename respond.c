// take the parsed request struct
// look at the path and decide what to serve
//if path is / serve index.html
//if path is /about serve about.html
// if file exists read it from disk
// if file doesn't exist send 404
// build the status line — HTTP/1.1 200 OK
// build the headers — Content-Type, Content-Length
// write headers to client_fd
// write file contents to client_fd