#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "request.h"
#include "response.h"
#include "server.h"

// Handle static file requests
Response *static_file_response(Request *req) {
  Response *res = malloc(sizeof(Response));

  char file_path[BUFFER_SIZE];
  snprintf(file_path, sizeof(file_path), ".%s", req->path);

  // Prevent directory traversal
  if (strstr(file_path, "..") != NULL) {
    res = set_response("403 Forbidden", "text/plain", "Access denied.",
                       strlen("Access denied."));
    return res;
  }

  // Check if path is a directory
  struct stat path_stat;
  if (stat(file_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode)) {
    return set_response(
        "403 Forbidden", "text/plain",
        "Access denied because it is a directory, not a file.",
        strlen("Access denied because it is a directory, not a file."));
  }

  // Open the file
  FILE *file = fopen(file_path, "rb");
  if (file == NULL) {
    perror("Failed to open file");
    res = set_response("404 Not Found", "text/plain", "File not found.",
                       strlen("File not found."));
    return res;
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  char *file_content = malloc(file_size);
  fread(file_content, 1, file_size, file);
  fclose(file);

  // Determine Content-Type based on file extension
  const char *content_type = "application/octet-stream";
  if (strstr(file_path, ".png")) {
    content_type = "image/png";
  } else if (strstr(file_path, ".jpg")) {
    content_type = "image/jpg";
  }
  // Populate the Response object
  res = set_response("200 OK", content_type, file_content, file_size);
  free(file_content);
  return res;
}
// Handle calculation requests
Response *send_calc_response(Request *req) {
  Response *res = malloc(sizeof(Response));

  char operation[OPERATIONS_LEN];
  int num1 = 0;
  int num2 = 0;

  // Parse the operation and numbers from the URL path
  if (sscanf(req->path, "/calc/%7[^/]/%d/%d", operation, &num1, &num2) !=
      OPERATIONS_COUNT) {
    res = set_response("400 Bad Request", "text/plain",
                       "Invalid calculation request format",
                       strlen("Invalid calculation request format"));
    return res;
  }

  int result = 0;
  char operator_symbol = '/';
  if (strcmp(operation, "add") == 0) {
    result = num1 + num2;
    operator_symbol = '+';
  } else if (strcmp(operation, "sub") == 0) {
    result = num1 - num2;
    operator_symbol = '-';
  } else if (strcmp(operation, "mul") == 0) {
    result = num1 * num2;
    operator_symbol = '*';
  } else if (strcmp(operation, "div") == 0) {
    if (num2 != 0) {
      result = num1 / num2;
    } else {
      res = set_response("400 Bad Request", "text/plain", "Division by zero",
                         strlen("Division by zero"));
      return res;
    }
  } else {
    res = set_response("400 Bad Request", "text/plain", "Invalid operation",
                       strlen("Invalid operation"));
    return res;
  }

  // Generate the response body
  char response_body[BUFFER_SIZE];
  snprintf(response_body, sizeof(response_body), "result: %d %c %d = %d", num1,
           operator_symbol, num2, result);

  // Populate the Response object
  res = set_response("200 OK", "text/plain", response_body,
                     strlen(response_body));

  return res;
}