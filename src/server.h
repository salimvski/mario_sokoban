#ifndef SERVER_H
#define SERVER_H

// TODO: Start TCP echo server on given port


ssize_t read_n_bytes(int sock, void *buf, size_t n);

void handle_sigint(int sig);

void run_server(int port);

#endif