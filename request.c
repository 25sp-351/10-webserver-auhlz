#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "request.h"
#include "server.h"

void free_request(Request *req) {
  if (req != NULL) {
    free(req->method);
    free(req->path);
    free(req->version);
    free(req->body);
    free(req);
  }
}

Request *parse_request(const char *request) {
  Request *req = malloc(sizeof(Request));
  char method[METHOD_LEN];
  char path[PATH_LEN];
  char version[VERSION_LEN];

  sscanf(request, "%s %s %s", method, path, version);

  req->method = strdup(method);
  req->path = strdup(path);
  req->version = strdup(version);
  req->body = strdup(request);

  const char *content_length_header = strstr(request, "Content-Length:");
  if (content_length_header != NULL) {
    sscanf(content_length_header, "Content-Length: %zu", &req->body_length);
  } else {
    req->body_length = 0;
  }
  return req;
}

// Read the client request from the socket and return a Request structure
Request *read_client_request(int client_fd) {
  static char buffer[BUFFER_SIZE];
  size_t total_read = 0;

  // Read the request into the buffer
  ssize_t bytes_read = 0;
  while (total_read < BUFFER_SIZE - 1) {
    bytes_read =
        read(client_fd, buffer + total_read, BUFFER_SIZE - total_read - 1);
    if (bytes_read <= 0) {
      return NULL;
    }
    total_read += bytes_read;

    if (strstr(buffer, "\r\n\r\n") != NULL) {
      break;
    }
  }
  buffer[total_read] = '\0';
  // Load the request into a Request structure
  Request *req = parse_request(buffer);
  return req;
}