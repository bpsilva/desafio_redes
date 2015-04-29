#include "server_udp.h"
struct sock s;

void saveFile(char *buffer)
{

	FILE * pOut;
  	pOut = fopen ("myfileout", "wb+");
  	fwrite (buffer , sizeof(char), strlen(buffer), pOut);
  
}





void connection()
{
	char buf[256];
		
    if ((s.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR opening socket");

	s.serv_addr.sin_family = AF_INET;
	s.serv_addr.sin_port = htons(PORT);
	s.serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(s.serv_addr.sin_zero), 8);    
	 
	if (bind(s.sockfd, (struct sockaddr *) &s.serv_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR on binding");
	
	s.clilen = sizeof(struct sockaddr_in);
	
	/* receive from socket */
	s.n = recvfrom(s.sockfd, buf, 256, 0, (struct sockaddr *) &s.cli_addr, &s.clilen);
		if (s.n < 0) 
			printf("ERROR on recvfrom");

	printf("%s", buf);
		
	/* send to socket */
	s.n = sendto(s.sockfd, "Got your message\n", 17, 0,(struct sockaddr *) &s.cli_addr, sizeof(struct sockaddr));
	if (s.n  < 0) 
		printf("ERROR on sendto");
	
	close(s.sockfd);
}

int main(int argc, char *argv[])
{
	connection();
	return 0;
}