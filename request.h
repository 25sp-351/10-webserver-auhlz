#ifndef REQUEST_H
#define REQUEST_H

#define METHOD_LEN 8
#define PATH_LEN 256
#define VERSION_LEN 16

typedef struct Request {
  char *method;
  char *path;
  char *version;
  char *body;
  size_t body_length;
} Request;

void free_request(Request *req);

Request *parse_request(const char *request);

Request *read_client_request(int client_fd);

#endif // REQUEST_H