#include "server_udp.h"



void send_message(char* msg)
{
		serv.n = sendto(serv.sockfd, msg, strlen(msg), 0,(struct sockaddr *) &serv.cli_addr, sizeof(struct sockaddr));
		if (serv.n  < 0) 
			printf("ERROR on sendto");
}

void connection_server()
{
	
		printf("Running server!!!\n");
    if ((serv.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR opening socket");

	serv.serv_addr.sin_family = AF_INET;
	serv.serv_addr.sin_port = htons(PORT);
	serv.serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv.serv_addr.sin_zero), 8);    
	 
	if (bind(serv.sockfd, (struct sockaddr *) &serv.serv_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR on binding");
	
	serv.clilen = sizeof(struct sockaddr_in);
	
	/* receive from socket */


}

void close_connection()
{
	close(serv.sockfd);	
}


