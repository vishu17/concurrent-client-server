/* server program, run this first */
/* tcp version */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

main()
{
	int sockint, s; /* socket for accepting connections */
	int namelen; /* length of client name */
	int ns; /* client socket */
	struct sockaddr_in client; /* client address information */
	struct sockaddr_in server; /* server address information */
	char buf[32]; /* data buffer */

	s = socket(AF_INET, SOCK_STREAM, 0); /* create stream socket using TCP */
	if( s == -1 ) {
		fprintf(stderr, "socket() Socket was not created.\n");
		exit(2);
	}
	else
		fprintf(stderr,"Socket created successfully.\n");

	server.sin_family = AF_INET; /* set up the server name */
	server.sin_port = 0; /* use first available port number */
	server.sin_addr.s_addr = INADDR_ANY;

	if( bind(s, &server, sizeof( server )) < 0 ) { /* bind server address to socket */
		fprintf(stderr,"bind() Error binding server.\n");
		exit(3);
	}

	/* find out what port was assigned */
	namelen = sizeof( server );
	if( getsockname( s, (struct sockaddr *) &server, &namelen) < 0 ) {
		fprintf(stderr, "getsockname() failed to get port number\n");
		exit(4);
	}
	fprintf(stderr,"The assigned port is %d\n", ntohs( server.sin_port));

	if( listen(s, 1) != 0 ) { /* listen for a connection */
		fprintf(stderr, "listen() failed\n");
		exit(5);
	}

	namelen = sizeof(client); /* accept connection request */
	if( (ns = accept(s, &client, &namelen)) == -1) {
		fprintf(stderr, "accept() failed to accept client connection request.\n");
		exit(6);
	}

	if( recv(ns, buf, sizeof(buf), 0) == -1 ) { /* wait for a client message to arrive */
		fprintf(stderr, "recv() did not get client data\n");
		exit(7);
	}

	if( send( ns, buf, sizeof(buf), 0) < 0) { /* echo the client message back to the client */
		fprintf(stderr, "send() failed to send data back to client.\n");
		exit(8);
	}

	close(ns); /* close client socket connection */
	close(s); /* close server socket connection */

	printf("Server finished.\n");
	exit(0);
}
