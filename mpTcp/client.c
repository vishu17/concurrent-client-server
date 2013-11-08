#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

/*
	Client --- sends message to socket 5824
*/

#define PORTNUMBER 4545

main (argc, argv) 
int argc;
char *argv[];
{
    int socketDesc;
    struct sockaddr_in destinationAddr;
    struct hostent *hostAddrPtr;

    char messageBuf[1024];
    int loopCnt;
    char thisHostName[128];

    if (argc < 2) {
        printf("Usage: connsend host\n");
        exit(1);
    }

    /* For testing purposes, make sure process will terminate eventually */
    alarm(60);  /* Terminate in 60 seconds */

    /* Create STREAM socket from which to send */
    if ((socketDesc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("open error on socket");
        exit(1);
    }
    
    /* Get numeric address of machine argv[1] */
    if ((hostAddrPtr = gethostbyname(argv[1])) == 0) {
        printf("Could not get address of %s\n", argv[1]);
        exit(1);
    }

    /* Create "name" of socket */
    destinationAddr.sin_family = AF_INET;
    memcpy((char *)&destinationAddr.sin_addr.s_addr, 
           (char *) hostAddrPtr->h_addr,
           hostAddrPtr-> h_length);
    destinationAddr.sin_port = htons(PORTNUMBER);
        

    /* Create connection to socket on machine argv */
    if (connect(socketDesc, (struct sockaddr *)&destinationAddr, 
                sizeof(destinationAddr)) < 0)
    {
        perror("Cannot form connection to socket...");
        exit(1);
    }

    /* Just for demo purposes get this host's name for messages... */
    gethostname(thisHostName,sizeof(thisHostName));

    /* Now send five messages to machine argv[1]...*/
    for (loopCnt=0;loopCnt<5;loopCnt++) {
        /* Create demo message */
        sprintf(messageBuf,"Message %d from host %s\n",loopCnt,thisHostName);
        /* The following could be :
             "send(socketDesc, messageBuf, strlen(messageBuf), 0)"
        */
        if (write(socketDesc, messageBuf, strlen(messageBuf)) < 0) {
            perror("socket send error");
            exit(1);
        }
        printf("Client: Message send: %s\n",messageBuf);
        sleep(1);
        /* The following could be:
             "recv(socketDesc, messageBuf, sizeof(messageBuf), 0)"
        */
        if (read(socketDesc,messageBuf,sizeof(messageBuf)) < 0) {
            perror("Socket read error in client");
            exit(1);
        }
        printf("Client: received %s\n",messageBuf);
    }

    /* All done */
    close(socketDesc);
    printf("Messages sent...\n");
}