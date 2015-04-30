#include "client_udp.h"
#include "out_buffer.h"
SOCKET s;



void connection(int argc, char *argv[])
{
	
	if (argc < 2) {
		fprintf(stderr, "usage %s hostname\n", argv[0]);
		exit(0);

	}
	
	s.server = gethostbyname(argv[1]);
	if (s.server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }	
	
	if ((s.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("ERROR opening socket");
	
	s.serv_addr.sin_family = AF_INET;     
	s.serv_addr.sin_port = htons(PORT);    
	s.serv_addr.sin_addr = *((struct in_addr *)s.server->h_addr);
}

void send_message(char *msg)
{
	
	s.n = sendto(s.sockfd, msg, strlen(msg), 0, (const struct sockaddr *) &s.serv_addr, sizeof(struct sockaddr_in));
	if (s.n < 0) 
		printf("ERROR sendto");
}
char* receive_message()
{
	char* buffer = 0;
	s.length = sizeof(struct sockaddr_in);
	s.n = recvfrom(s.sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &s.from, &s.length);

	if (s.n < 0)
		printf("ERROR recvfrom");

	return buffer;
}

void close_connection()
{
	close(s.sockfd);
}


