#include "out_buffer.h"
#include "interm_buffer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
void init()
{
	int index;

//OUT BUFFER INIT
	out_get = 0;
	out_win_end = 0;
	out_win_begin = 0;
	sem_init(&out_data, 0, 0);
	sem_init(&out_buffer_space, 0, OUT_BUFFER_SIZE-1);//(2^n)-1
	sem_init(&out_buffer_access, 0, 1);

	for(index = 0; index < OUT_BUFFER_SIZE ; index++)
	{
		out_buffer[index] = 0;
	}


//INTERM BUFFER INIT
	interm_set_pos = 0;
	interm_get_pos = 0;
	sem_init(&interm_data, 0, 0);
	sem_init(&interm_buffer_space, 0, INTERM_BUFFER_SIZE);
	sem_init(&interm_buffer_access, 0, 1);

	for(index = 0; index < INTERM_BUFFER_SIZE ; index++)
	{
		interm_buffer[index] = 0;
	}

}

void write_file_aux()
{
	
	FILE * pOut;
  	pOut = fopen ("myfileout", "wb+");
  	char* buffer;
  	
  	do
  	{

	buffer = get_out()->data;
	fwrite (buffer , sizeof(char), strlen(buffer), pOut);	
  	}while(strlen(buffer) == INTERM_BLOCK_SIZE);
  	  
}

int main()
{
	init();
	pthread_t write_aux;
/*	int opt;
	

	while(1)
	{
		printf("1 - Receber aquivo.\n");
		printf("2 - enviar aquivo.\n");
		
		fflush(stdin);
		scanf("%d", &opt);

		if(opt == 0) 
			break;

		switch(opt)
		{
			case 1:
				break;
			case 2:*/
				printf("ENVIAR.\n");
				printf("Digite o nome do arquivo:\n");
				scanf("%s", filename);

				pthread_create(&read_thread, NULL, (void*)readFile, NULL);
				pthread_create(&i_to_out_thread, NULL, (void*)interm_to_out, NULL);
				pthread_create(&write_aux, NULL, (void*)write_file_aux, NULL);

				pthread_join(read_thread, NULL);
				pthread_join(i_to_out_thread, NULL);
				pthread_join(write_aux, NULL);
			/*
				break;
			default:
				printf("Opção inválida.\n");
		}	
	}	*/
	
	return 0;
}