#include "in_buffer.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void set_in(char *message)
{

	sem_wait(&in_buffer_space);
	sem_wait(&in_buffer_access);	

	
	in_buffer[in_win_end] = message;

	if(in_win_end == IN_BUFFER_SIZE-1)
		{
			in_win_end = 0;	
		}else{
			in_win_end++;
		}
	

	sem_post(&in_buffer_access);
	sem_post(&in_data);
}

char* get_in()
{

	char *destiny;

	sem_wait(&in_data);
	sem_wait(&in_buffer_access);	
	
		destiny = in_buffer[in_get];
		in_buffer[in_get] = 0;

		if(in_get == IN_BUFFER_SIZE-1)
		{
			in_get = 0;	
		}else{
			in_get++;
		}
	sem_post(&in_buffer_access);
	sem_post(&in_buffer_space);
	return destiny;
}




