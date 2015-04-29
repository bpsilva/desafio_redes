#include "client_udp.h"
struct SOCKET s;

char* readFile()
{
 FILE * pEntry;

  long lSize;
  char * buffer;
  size_t result;

  pEntry = fopen ( "myfile" , "rb" );
 
  
  if (pEntry==NULL) {fputs ("File error",stderr); exit (1);}

  // obtain file size:
  fseek (pEntry , 0 , SEEK_END);
  lSize = ftell (pEntry);
  rewind (pEntry);
  printf("%li\n", lSize);

  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  result = fread (buffer,1,lSize,pEntry);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

  /* the whole file is now loaded in the memory buffer. */

return buffer;
}

void connection(int argc, char *argv[])
{
	char *buffer;
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
	bzero(&(s.serv_addr.sin_zero), 8);  

	buffer = readFile();

	s.n = sendto(s.sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &s.serv_addr, sizeof(struct sockaddr_in));
	if (s.n < 0) 
		printf("ERROR sendto");
	
	s.length = sizeof(struct sockaddr_in);
	s.n = recvfrom(s.sockfd, buffer, 256, 0, (struct sockaddr *) &s.from, &s.length);
	if (s.n < 0)
		printf("ERROR recvfrom");

	printf("Got an ack: %s\n", buffer);
	
	close(s.sockfd);

}


int main(int argc, char *argv[])
{
    
	connection(argc, argv);
	
	return 0;
}