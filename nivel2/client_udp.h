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
#include "out_buffer.h"
#include "interm_buffer.h"
#define PORT 4000

typedef struct SOCKET{
	int sockfd, n;
	unsigned int length;
	struct sockaddr_in serv_addr, from;
	struct hostent *server;

}SOCKET;
pthread_t send_out;
void send_message(message *msg);
char* receive_message();
void close_connection();
void connection(int argc, char *argv[]);

void send_out_buffer();
#endif