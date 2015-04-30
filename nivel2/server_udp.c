#include "server_udp.h"
#include "interm_buffer.h"
#include "out_buffer.h"
struct sock serv;

void saveFile(char *buffer)
{

	FILE * pOut;
  	pOut = fopen ("myfileout", "wb+");
  	fwrite (buffer , sizeof(char), strlen(buffer), pOut);
  
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
	char buf[600];
	do
	{
		
		serv.n = recvfrom(serv.sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &serv.cli_addr, &serv.clilen);
		if (serv.n < 0) 
			printf("ERROR on recvfrom");

		printf("%s", buf);
	}while(strlen(buf) ==600);
	
	close(serv.sockfd);
}

int main()
{
	connection_server();
	return 0;
}
