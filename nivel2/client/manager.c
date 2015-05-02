#include "manager.h"

#include "out_buffer.h"
#include "in_buffer.h"
#include "interm_buffer.h"
#include "client_udp.h"

void interm_to_out()
{
	char *text, * msg, *opcode,*flag, *address , *control, *crc;
	int NS = 0;
	do
	{
		text = get_interm();

		msg = (char*)calloc(sizeof(char), strlen(text)+7);

		
		address = (char*)calloc(sizeof(char), 1);
		strcpy(address, "A");
		
		control = (char*)calloc(sizeof(char), 1);
		control[0] = NS;

		opcode = (char*)calloc(sizeof(char), 1);
		opcode[0] = 3;

		crc = (char*)calloc(sizeof(char), 2);
		
		crc[0] = 'R';
		crc[1] = 'C';

		flag = (char*)calloc(sizeof(char), 1);
		flag[0] = 'F';
		
		strcat(msg, flag);
		strcat(msg, address);
		strcat(msg, control);		
		strcat(msg, opcode);
		strcat(msg, text);
		strcat(msg, crc);
		strcat(msg, flag);

		printf("%s\n", msg);
		set_out(msg);
	 	
	 	if(NS == OUT_BUFFER_SIZE-1)
	 	{	NS = 0; }
	 	else {	NS++;}

	}while(strlen(text) == INTERM_BLOCK_SIZE);
}

void readFile()
{
 FILE * pEntry;
  char * buffer;
  
  pEntry = fopen ( filename, "rb" );
 
  
  if (pEntry==NULL) {fputs ("File error",stderr); exit (1);}

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


void init(int argc, char *argv[])
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

//IN BUFFER INIT
	in_get = 0;
	in_win_end = 0;
	in_win_begin = 0;
	sem_init(&in_data, 0, 0);
	sem_init(&in_buffer_space, 0, IN_BUFFER_SIZE-1);//(2^n)-1
	sem_init(&in_buffer_access, 0, 1);

	for(index = 0; index < IN_BUFFER_SIZE ; index++)
	{
		in_buffer[index] = 0;
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




//CLIENT UDP INIT
	connection(argc, argv);
}

int cmp(char* msg)
{
	int i;
	for(i = 0 ; i < strlen(filename);i++)
	{
		if(filename[i]!=msg[i])
			return 0;
	}
	return 1;
}
void send_out_buffer()
{
	char *msg;

 	do
  	{
		msg = get_out();

		send_message(msg);
		printf("%i    %i\n", strlen(msg), INTERM_BLOCK_SIZE+7);
  	}while(strlen(msg) == INTERM_BLOCK_SIZE+7 || cmp(msg+4));
}




 void send_file()
 {
				pthread_create(&read_file_thread, NULL, (void*)readFile, NULL);
				pthread_create(&i_to_out_thread, NULL, (void*)interm_to_out, NULL);
				pthread_create(&send_out, NULL, (void*)send_out_buffer, NULL);
				//pthread_create(&recv_confirmation, NULL, (void*)recv_confirm, NULL);
				
				pthread_join(read_file_thread, NULL);
				pthread_join(i_to_out_thread, NULL);
				pthread_join(send_out, NULL);
				//pthread_join(recv_confirmation, NULL);
 }

void send_write_request()
{
		char * msg, *flag, *address , *control, *crc, *opcode;
		
		msg = (char*)calloc(sizeof(char), strlen(filename)+7);
	
		address = (char*)calloc(sizeof(char), 1);
		address[0] = BROADCAST;
	
		control = (char*)calloc(sizeof(char), 1);
		control[0] = 'W';
		
		opcode = (char*)calloc(sizeof(char), 1);
		opcode[0] = SEND_REQ;

		crc = (char*)calloc(sizeof(char), 2);
		
		crc[0] =  'R';
		crc[1] = 'C';

		flag = (char*)calloc(sizeof(char), 1);
		flag[0] = FLAG;
		
		strcat(msg, flag);
		strcat(msg, address);
		strcat(msg, control);		
		strcat(msg, opcode);
		strcat(msg, filename);
		strcat(msg, crc);
		strcat(msg, flag);

		set_out(msg);//send the request for reading to out_buffer
		
		send_file();
}

void recv_confirm()
{
	char* msg;

do
{
	msg =  receive_message();
	switch(msg[2]>>4)
	{
		case RR:
			while((msg[2] & 3) != out_win_begin)  //msg & 00000011 -> gets the 2 least singnificant bits
			{
				if(out_win_begin == OUT_BUFFER_SIZE-1)
				{
					out_win_begin = 0;	
				}else{
					out_win_begin++;
				}
			sem_post(&out_buffer_space);
			}
		break;
		case REJ:
			sem_wait(&out_buffer_access);
				out_get = (msg[2] & 3);	//sets the reading pointer to the rej pos (//msg & 00000011 -> gets the 2 least singnificant bits)
			sem_post(&out_buffer_access);
		break;
	}
}while(1);
}

