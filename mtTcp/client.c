/* rem invoke with a.out 156.59.20.100 portnum */
/* tcp client */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h> #including pthreads lib for threading

main( argc, argv)
int argc;
char **argv;
{
	int sockint, s; /* client socket */
	unsigned short port ; /* port number client will connect to */
	struct sockaddr_in server; /* server address */
	char buf[32]; /* data buffer for sending and receiving */

	if( argc != 3 ) { /* check command line arguments */
		fprintf( stderr, "Usage: %s hostname port\n", argv[0] );
		exit(1);
	}

	port = (unsigned short) atoi(argv[2]); /* get the port number */

	strcpy( buf, "Hello there!"); /* create the message */

	/* create stream socket using TCP */
	fprintf(stderr, "Creating datagram socket.\n");
	s = socket(AF_INET, SOCK_STREAM, 0);
	if( s == -1 ) {
		fprintf(stderr, "Socket was not created.\n");
		exit(2);
	}
	else
		fprintf(stderr, "Socket created successfully.\n");

	server.sin_family = AF_INET; /* set up the server name */
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr( argv[1] );

	/* connect to the server */
	if( connect(s, &server, sizeof(server)) < 0) {
		fprintf(stderr, "Failed to connect to socket.\n");
		exit(3);
	}

	printf("Sending the message: %s\n", buf); /* send the message */
	if( send(s, buf, sizeof(buf), 0) < 0 ) {
		fprintf(stderr, "Failed to send data to socket.\n");
		exit(4);
	}

	/* receive the echoed message from the server */
	if( recv(s, buf, sizeof(buf), 0) < 0 ) {
		fprintf(stderr, "Failed to receive data from socket.\n");
		exit(5);
	}
	printf("The message from the server was: %s\n", buf);

	close(s); /* close the socket */
	printf("Client closed successfully\n"); /* successfully exit */
	exit(0);
}