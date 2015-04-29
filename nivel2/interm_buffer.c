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
		//destiny = (char*)calloc(sizeof(char),strlen(interm_buffer[interm_get_pos]));
		
		//strncpy(destiny, interm_buffer[interm_get_pos], strlen(interm_buffer[interm_get_pos]));
		
		//free(interm_buffer[interm_get_pos]);
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
  
  pEntry = fopen ( filename, "rb" );
 
  
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
  	buffer = (char*) calloc(sizeof(char) , INTERM_BLOCK_SIZE);
   	fread (buffer, 1 , INTERM_BLOCK_SIZE, pEntry);
  	
  	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
  	set_interm(buffer);
  }while(strlen(buffer) == INTERM_BLOCK_SIZE);
  
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
  }while(strlen(buffer) == INTERM_BLOCK_SIZE);
  	  
}




