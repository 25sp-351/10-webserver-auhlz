#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "request.h"
#include "response.h"
#include "server.h"

void send_response(int client_fd, Response *res) {
  char header[BUFFER_SIZE];

  // Generate the HTTP response header
  int header_length =
      snprintf(header, sizeof(header),
               "HTTP/1.1 %s\r\n"
               "Content-Type: %s\r\n"
               "Content-Length: %zu\r\n"
               "Connection: close\r\n\r\n",
               res->status, res->content_type, res->body_length);

  // Send the header
  write(client_fd, header, header_length);
  // Stream the body in chunks if it's too large
  size_t total_sent = 0;
  while (total_sent < res->body_length) {
    size_t chunk_size = BUFFER_SIZE;
    if (res->body_length - total_sent < BUFFER_SIZE) {
      chunk_size = res->body_length - total_sent;
    }
    write(client_fd, res->body + total_sent, chunk_size);
    total_sent += chunk_size;
  }
}

void free_response(Response *res) {
  if (res != NULL) {
    free(res->status);
    free(res->content_type);
    free(res->body);
    free(res);
  }
}

Response *set_response(const char *status, const char *content_type,
                       const char *body, size_t body_length) {
  Response *res = malloc(sizeof(Response));

  res->status = strdup(status);
  res->content_type = strdup(content_type);
  res->body = malloc(body_length);
  memcpy(res->body, body, body_length);
  res->body_length = body_length;

  return res;
}

Response *generate_response(Request *req) {
  Response *res = malloc(sizeof(Response));

  // Check the HTTP method
  if (strcmp(req->method, "GET") == 0) {
    printf("Thread [%ld]: Received complete HTTP request:\n%s\n",
           pthread_self(), req->body);

    // Check if the request is for a static file
    if (strncmp(req->path, "/static/", STATIC_LEN) == 0) {
      res = static_file_response(req);
      // Check if the request is for a calculation
    } else if (strncmp(req->path, "/calc/", CALC_LEN) == 0) {
      res = send_calc_response(req);
      // Handle unknown paths
    } else {
      res = set_response("404 Not Found", "text/plain", "i got it",
                         strlen("i got it"));
    }
    // Handle unsupported HTTP methods
  } else {
    res = set_response("405 Method Not Allowed", "text/plain",
                       "Method not allowed", strlen("Method not allowed"));
  }
  return res;
}