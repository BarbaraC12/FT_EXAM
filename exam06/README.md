# EXAM06: Mini serv [![bcano's 42 Exam Rank 06 Score](https://badge42.vercel.app/api/v2/cl1e0jvxp000909mxrmuzq4x2/project/3084697)](https://github.com/JaeSeoKim/badge42)

- **Assignment name**   : mini_serv  
- **Expected files**    : mini_serv.c  
- **Allowed functions** : write, close, select, socket, accept, listen, send, recv, bind, strstr, malloc, realloc, free, calloc, bzero, atoi, sprintf, strlen, exit, strcpy, strcat, memset
--------------------------------------------------------------------------------

Write a program that will listen for client to connect on a certain port and only on 127.0.0.1. It will let clients to speak with each other

This program will take as first argument the port to bind to
If no argument is given, it should write in stderr "Wrong number of arguments\n" and exit with status 1
If a System Calls returns an error before the program start accepting connection, it should write in stderr "Fatal error\n" and exit with status 1
If you cant allocate memory it should write in stderr "Fatal error" followed by a \n and exit with status 1

Your program must be non-blocking but client can be lazy and if they don't read your message you must NOT disconnect them...

Your program must not contains #define preproc
The fd that you will receive will already be set to make 'recv' or 'send' to block if select hasn't be called before calling them, but will not block otherwise. 

When a client connect to the server:
- the client will be given an id. the first client will receive the id 0 and each new client will received the last client id + 1
- a message is sent to all the client that was connected to the server: "server: client %d just arrived\n" %d is id

Clients must be able to send messages to your program.
- message will only be printable characters, no need to check
- a single message can contains multiple '\n'
- when the server receive a message, it must resend it to all the other client with "client %d: " at the begin of every line!

When a client disconnect from the server:
- a message is sent to all the client that was connected to the server: "server: client %d just left\n"
- the right fd will be close

Memory or fd leaks are forbidden

Hint: you can use nc to test your program
