#ifndef RESPONSE_H
#define RESPONSE_H

#include "request.h"

#define OPERATIONS_LEN 8
#define OPERATIONS_COUNT 3
#define STATIC_LEN 8
#define CALC_LEN 6

typedef struct Response {
  char *body;
  char *status;
  char *content_type;
  size_t body_length;
} Response;

void send_response(int client_fd, Response *response);

void free_response(Response *response);

Response *set_response(const char *status, const char *content_type,
                       const char *body, size_t body_length);

Response *generate_response(Request *req);

Response *static_file_response(Request *req);

Response *send_calc_response(Request *req);

#endif // RESPONSE_H