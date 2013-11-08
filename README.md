Concurent Client Server
========================

A concurrent client server implementation in C.

###Setting up & building using makefile

Once in the mtTcp or mpTcp folder, run 'make' to compile the source into executables

The output should be as follows -

>- gcc -o server server.c -lpthread (In case of multi threaded server)
>- gcc -o client client.c -lpthread

This should give you 'client' & 'server' executables

Run './server' and you shall get the following output --

>- Socket created successfully.
>- The assigned port is 51833

Now run './client', it needs 2 parameters, one the host to run and the port that the server is running on.

> ./client <ip> <port>

Expected output

>- Creating datagram socket.
>- Socket created successfully.
>- Sending the message: Hello there!
>- The message from the server was: Hello there!
>- Client closed successfully

Client / Server socket connections are then closed.