LIB = -lpthread
CC = gcc
 
all: server client
 
server: server.c
	$(CC) -o server server.c $(LIB)
 
client: client.c
	$(CC) -o client client.c $(LIB)
 
clean:
	/bin/rm -f client server core *.o *~

