#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "request.h"
#include "response.h"
#include "server.h"

void *handle_connection(int *sock_fd) {
  int client_fd = *sock_fd;
  free(sock_fd);
  // Handle message
  printf("Handling connection on %d\n", client_fd);
  while (1) {
    Request *req = read_client_request(client_fd);
    if (req == NULL) {
      break;
    }
    // Generate response
    Response *response = generate_response(req);
    send_response(client_fd, response);
    free_request(req);
    free_response(response);
  }
  close(client_fd);
  printf("Closing connection on %d\n", client_fd);
  return NULL;
}