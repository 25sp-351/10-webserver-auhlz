#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "server.h"

void run_server(int socket_fd) {

  if (socket_fd < 0) {
    fprintf(stderr, "Failed to create listening socket\n");
    return;
  }

  struct sockaddr_in client_address;
  socklen_t client_address_len = sizeof(client_address);

  while (1) {

    int *client_fd_buf = malloc(sizeof(int));
    if (!client_fd_buf) {
      perror("Failed to allocate memory for client_fd_buf");
      continue;
    }

    *client_fd_buf = accept(socket_fd, (struct sockaddr *)&client_address,
                            &client_address_len);

    if (*client_fd_buf < 0) {
      perror("Failed to accept connection");
      free(client_fd_buf);
      continue;
    }
    printf("Accepted connection on %d\n", *client_fd_buf);

    pthread_t thread;
    pthread_create(&thread, NULL, (void *(*)(void *))handle_connection,
                   (void *)client_fd_buf);
  }
  close(socket_fd);
}