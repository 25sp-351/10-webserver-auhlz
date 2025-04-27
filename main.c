#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "server.h"

int main(int argc, char *argv[]) {
  int port = DEFAULT_PORT;

  for (int ix = 0; ix < argc; ix++) {
    if (strcmp(argv[ix], "-p") == 0 && ix + 1 < argc) {
      sscanf(argv[ix + 1], "%d", &port);
      if (port < 80 || port > 65535) {
        fprintf(stderr, "Invalid port number: %d\n", port);
        return 1;
      }
    }
  }

  int socket_fd = create_listening_socket(port);
  run_server(socket_fd);
  close(socket_fd);
  return 0;
}