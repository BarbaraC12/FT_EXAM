#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

typedef struct t_client {
	int id;
	char msg[110000];
} t_client;

int max = 0 /*socket max open*/, id = 0 /*actual id set*/, server /*socket server*/, cli /*socket client*/, rec /*receive*/;
fd_set fd, fdRead, fdWrite;
char buffR[110000], buffW[110000];
struct sockaddr_in addr;
t_client clients[1024] = {0}; /*bzero array of client*/
socklen_t _len = sizeof(addr); /*size of sockaddr struct*/

void error_msg(const char *txt) {
	write(2, txt, strlen(txt));
	exit(1);
}

void send_all(int cli) {
	for (int i = 0; i <= max; i++)
		if (FD_ISSET(i, &fdWrite) && i != cli)
			send(i, buffW, strlen(buffW), 0);
}

int main(int ac, char **av) {
// Argument number
	if (ac != 2)
		error_msg("Wrong number of arguments\n");
// Socket server init
	if (( server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error_msg("Fatal error\n");
	max = server;
//  Set fd 
	FD_ZERO(&fd);
	FD_SET(server, &fd);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(2130706433);
	addr.sin_port = htons(atoi(av[1]));
// Bind server and listen 
	if ((bind(server, (const struct sockaddr *)&addr, sizeof(addr))) < 0 || listen(server, 128) < 0)
		error_msg("Fatal error\n");
	while (1) {
		fdRead = fdWrite = fd;
// Select Waiting to fd ready
		if (select(max + 1, &fdRead, &fdWrite, 0, 0) < 0) continue;
		for (int s = 0; s <= max; s++) {
			if (FD_ISSET(s, &fdRead)) {								// check if any event in fdRead
				if (s == server) {									// FD_SET() new client
					if ((cli = accept(server, (struct sockaddr *)&addr, &_len)) < 0) continue ;
					max = (cli > max) ? cli : max;
					clients[cli].id = id++;
					FD_SET(cli, &fd);
					sprintf(buffW, "server: client %d just arrived\n", clients[cli].id);
					send_all(cli);
					break;
				} else {
					if ((rec = recv(s, buffR, 110000, 0)) > 0) {	// normal msg
						for (int i = 0, j = strlen(clients[s].msg); i < rec; i++, j++) {
							clients[s].msg[j] = buffR[i];
							if (buffR[i] == '\n') {
								clients[s].msg[j] = '\0';
								sprintf(buffW, "client %d: %s\n", clients[s].id, clients[s].msg);
								send_all(s);
								bzero(&clients[s].msg, strlen(clients[s].msg));
								j = -1;
						}   }
						break;
					} else {										// FD_CLR() leave msg
						sprintf(buffW, "server: client %d just left\n", clients[s].id);
						send_all(s);
						FD_CLR(s, &fd);
						close(s);
						break;
}	}	}	}	}	}
