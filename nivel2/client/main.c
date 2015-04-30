#include "manager.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void write_file_aux()
{
	
	FILE * pOut;
  	pOut = fopen ("myfileout", "wb+");
  	char* buffer;
  	
  	do
  	{
  	buffer = get_out();
  	//buffer = (char*)calloc(sizeof(char), strlen(aux)-6);	
	//strncpy(buffer, aux+3, strlen(buffer));
	printf("%i\n", strlen(buffer));
	fwrite (buffer , sizeof(char), strlen(buffer), pOut);	
  	}while(strlen(buffer) == INTERM_BLOCK_SIZE+5);
  	
}
 
 void send_file()
 {


				pthread_create(&read_file_thread, NULL, (void*)readFile, NULL);
				pthread_create(&i_to_out_thread, NULL, (void*)interm_to_out, NULL);
				pthread_create(&send_out, NULL, (void*)send_out_buffer, NULL);
				
				pthread_join(read_file_thread, NULL);
				pthread_join(i_to_out_thread, NULL);
				pthread_join(send_out, NULL);
 }


int main(int argc, char *argv[])
{
	init(argc, argv);

	
	int opt;
	
				
		fflush(stdin);
		scanf("%d", &opt);

		switch(opt)
		{
			case 1:
				//send server request
				//receive file
				break;
			case 2:

				printf("Digite o nome do arquivo:\n");
				scanf("%s", filename);
				send_write_request();
				//send server request
				//send file
				break;
			default:
				printf("Opção inválida.\n");
		}	
	return 0;
}