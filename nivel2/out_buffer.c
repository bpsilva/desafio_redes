#include "out_buffer.h"
#include "interm_buffer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void set_out(message *message)
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

message* get_out()
{

	message *destiny;

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

void interm_to_out()
{
	
	message *msg;
	int i;
	do
	{
		sem_getvalue(&out_buffer_space, &i);
		msg = (message*)malloc(sizeof(struct message));
		msg->flag = 0x7E;
		//message.address
		//control;
		msg->opcode = OP_DATA;
		msg->data = get_interm();
		//crc;
		set_out(msg);
	 	
	}while(strlen(msg->data) == INTERM_BLOCK_SIZE);
}


