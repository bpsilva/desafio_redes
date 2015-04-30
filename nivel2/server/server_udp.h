#ifndef _SERVER_UDP_H_
#define _SERVER_UDP_H_

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#define PORT 4000

	struct sock
	{
		int sockfd, n;
		socklen_t clilen;
		struct sockaddr_in serv_addr, cli_addr;
	}sock;

	void connection_server();
#endif