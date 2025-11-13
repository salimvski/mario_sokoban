#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>  // for ssize_t
#include <stddef.h>     // for size_t
#include <unistd.h>

typedef struct {
    int sock;
    int id;
    char* level_name;
} client_info_t;


ssize_t read_n_bytes(int sock, void* buf, size_t n);

void handle_sigint(int sig);

void run_server(int port);

#endif