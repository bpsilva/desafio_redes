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
#include <pthread.h>
#include <semaphore.h>
#define PORT 4000



pthread_t recv_in;

	struct sock
	{
		int sockfd, n;
		socklen_t clilen;
		struct sockaddr_in serv_addr, cli_addr;
	}sock;
struct sock serv;


	void connection_server();
	void close_connection();
	void entry_monitor();
	void recv_to_in();

	void send_message(char* msg);
#endif
