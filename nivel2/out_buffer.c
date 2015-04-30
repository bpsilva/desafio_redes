#include "out_buffer.h"
#include "interm_buffer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void set_out(char *message)
{

	sem_wait(&out_buffer_space);
	sem_wait(&out_buffer_access);	

	
	out_buffer[out_win_end] = message;

	if(out_win_end == OUT_BUFFER_SIZE-1)
		{
			out_win_end = 0;	
		}else{
			out_win_end++;
		}
	

	sem_post(&out_buffer_access);
	sem_post(&out_data);
}

char* get_out()
{

	char *destiny;

	sem_wait(&out_data);
	sem_wait(&out_buffer_access);	
	
		destiny = out_buffer[out_get];
		out_buffer[out_get] = 0;

		if(out_get == OUT_BUFFER_SIZE-1)
		{
			out_get = 0;	
		}else{
			out_get++;
		}
	sem_post(&out_buffer_access);
	sem_post(&out_buffer_space);
	return destiny;
}


void copy_data(char* msg, char* text, int size)
{
	int i;
	for(i = 0; i < size;i++)
	{
		msg[i+24] = text[i];
	}
}

void interm_to_out()
{
	char *aux;
	char *text;
	char* msg;
	
	char *flag ;
	char *address ;
	char *control ;
	char *crc ;
	do
	{
		text = get_interm();

		msg = (char*)calloc(sizeof(char), strlen(text)+7);

		
		address = (char*)calloc(sizeof(char), 1);
		strcpy(address, "A");
		
		control = (char*)calloc(sizeof(char), 1);
		strcpy(control, "C");
		
		crc = (char*)calloc(sizeof(char), 1);
		
		text[strlen(text)-1] = 'R';
		crc[0] = 'C';

		flag = (char*)calloc(sizeof(char), 1);
		flag[0] = 'F';
		
		strcat(msg, flag);
		strcat(msg, address);
		strcat(msg, control);		
		strcat(msg, text);
		strcat(msg, crc);
		strcat(msg, flag);
		//strncpy(msg, text, strlen(text));
		//copy_data(msg, text, strlen(text));

		set_out(msg);
	 	
	}while(strlen(text) == INTERM_BLOCK_SIZE);
}


