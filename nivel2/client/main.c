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

				break;
			default:
				printf("Opção inválida.\n");
		}	
	return 0;
}