#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

/*
    Multi process concurrent server --- by forking children several clients can be served at the same time...

    This program has to be killed to terminate, or alternately it will abort in
    120 seconds on an alarm...
*/

#define PORTNUMBER 4545

main () {
    int listenDesc;
    int connectionDesc;
    int receivedMsgLen;
    struct sockaddr_in myAddr;
    char messageBuf[1024];

    /* For testing purposes, make sure process will terminate eventually */
    alarm(120);  /* Terminate in 120 seconds */

    /* Create socket from which to read */
    if ((listenDesc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("open error on socket");
        exit(1);
    }

    /* Create "name" of socket */
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = INADDR_ANY;
    myAddr.sin_port = htons(PORTNUMBER);
        
    if (bind(listenDesc, (struct sockaddr *) &myAddr, sizeof(myAddr)) < 0) {
        perror("bind error");
        exit(1);
    }

    /* Start accepting connections.... */
    /* Up to 5 requests for connections can be queued... */
    listen(listenDesc,5);   

    while (1) /* Loop forever */ {
        /* Wait for a connection */
        connectionDesc = accept(listenDesc, NULL, NULL);
        
        /* Fork a child process to actually handle this client */
        if (fork() == 0) {
           /* This is the ==>CHILD<== code */
           if (connectionDesc < 0) {
               printf("Accept failed\n");
               exit(1);  /* Exit child */
           }
    
           /* Receive messages from sender... */
           while ((receivedMsgLen=read(connectionDesc,messageBuf,
                   sizeof(messageBuf))) > 0) {
               messageBuf[receivedMsgLen] = '\0';
               printf("Server: Message: %s\n",messageBuf);
               if (write(connectionDesc,"GOT IT\0",7) < 0) {
                   perror("Server: write error");
                   exit(1);
               }
           }
           close(connectionDesc);
           exit(0);    /* Normal exit from child process */
        }

        /* This is the continuation of the ==>PARENT<== code */
        /* Loop back and wait for next client */
        close(connectionDesc);
        printf("Parent ready for another connection \n");
    }
}