#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"

int create_listening_socket(int port) {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    perror("socket");
    return -1;
  }

  struct sockaddr_in server_address;
  memset(&server_address, '\0', sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(port);

  printf("Binding to port %d\n", port);

  int return_val = bind(socket_fd, (struct sockaddr *)&server_address,
                        sizeof(server_address));
  if (return_val < 0) {
    perror("bind");
    return -1;
  }

  return_val = listen(socket_fd, LISTEN_BACKLOG);
  if (return_val < 0) {
    perror("listen");
    return -1;
  }

  return socket_fd;
}
