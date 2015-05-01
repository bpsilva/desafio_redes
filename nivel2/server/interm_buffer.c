#include "interm_buffer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void set_interm(char *data)
{

	sem_wait(&interm_buffer_access);	
	sem_wait(&interm_buffer_space);	
	interm_buffer[interm_set_pos] = data;


	if(interm_set_pos == INTERM_BUFFER_SIZE-1)
		{
			interm_set_pos = 0;	
		}else{
			interm_set_pos++;
		}
	

	sem_post(&interm_buffer_access);
	sem_post(&interm_data);
}

char* get_interm()
{

	char *destiny;

	sem_wait(&interm_data);
	sem_wait(&interm_buffer_access);	
	
		destiny = interm_buffer[interm_get_pos];
		interm_buffer[interm_get_pos] = 0;
		
		if(interm_get_pos == INTERM_BUFFER_SIZE-1)
		{
			interm_get_pos = 0;	
		}else{
			interm_get_pos++;
		}
			
	sem_post(&interm_buffer_space);		
	sem_post(&interm_buffer_access);

	return destiny;
}






