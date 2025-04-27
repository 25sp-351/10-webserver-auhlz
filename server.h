#ifndef CONNECTION_H
#define CONNECTION_H

#define LISTEN_BACKLOG 5
#define BUFFER_SIZE 1024
#define DEFAULT_PORT 80

void *handle_connection(int *sock_fd);

void run_server(int socket_fd);

int create_listening_socket(int port);

#endif
