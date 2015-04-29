#ifndef _CLIENT_UDP_H_
#define _CLIENT_UDP_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PORT 4000

struct SOCKET{
	int sockfd, n;
	unsigned int length;
	struct sockaddr_in serv_addr, from;
	struct hostent *server;

}SOCKET;
void connection(int argc, char *argv[]);
#endif