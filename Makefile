all: main

OBJS = main.o connection.o server_socket.o response.o request.o server.o routes.o
CC = gcc
CFLAGS = -Wall

main: $(OBJS)
	$(CC) -o main $(CFLAGS) $(OBJS) $(LDFLAGS)

server.o: server.c server.h 

connection.o: connection.c server.h

server_socket.o: server_socket.c server.h

response.o: response.c response.h

routes.o: routes.c response.h

request.o: request.c request.h

clean:
	rm -f main $(OBJS) server.o server_socket.o response.o request.o connection.o routes.o
