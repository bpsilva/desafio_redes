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
	
		destiny = (char*)calloc(sizeof(char),strlen(interm_buffer[interm_get_pos]));
		
		strncpy(destiny, interm_buffer[interm_get_pos], strlen(interm_buffer[interm_get_pos]));
		
		free(interm_buffer[interm_get_pos]);
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

void readFile()
{
 FILE * pEntry;
  char * buffer;
  
  pEntry = fopen ( "myfile" , "rb" );
 
  
  if (pEntry==NULL) {fputs ("File error",stderr); exit (1);}

  /*
  // obtain file size:
  fseek (pEntry , 0 , SEEK_END);
  lSize = ftell (pEntry);
  rewind (pEntry);
  printf("%li\n", lSize);
*/

  do
  {
  	buffer = (char*) calloc(sizeof(char) , BLOCK_SIZE);
   	fread (buffer, 1 , BLOCK_SIZE, pEntry);
  	
  	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}


  	set_interm(buffer);

  	
  }while(strlen(buffer) == BLOCK_SIZE);
  
}


void saveFile()
{
	
	FILE * pOut;
  	pOut = fopen ("myfileout", "wb+");
  	char* buffer;
  	
  do
  {
	
  	buffer = get_interm();
  	fwrite (buffer , sizeof(char), strlen(buffer), pOut);	
  }while(strlen(buffer) == BLOCK_SIZE);
  	  
}

void init()
{
	int index;

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


int main()
{
	init();
	pthread_create(&read_thread, NULL, (void*)readFile, NULL);
	pthread_create(&write_thread, NULL, (void*)saveFile, NULL);

	
	
	pthread_join(read_thread, NULL);
	pthread_join(write_thread, NULL);
	


	return 0;
}