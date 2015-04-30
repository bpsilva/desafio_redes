#include "manager.h"

#include "out_buffer.h"
#include "in_buffer.h"
#include "interm_buffer.h"
#include "client_udp.h"

void interm_to_out()
{
	char *text, * msg, *flag, *address , *control, *crc;
	
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

void send_out_buffer()
{
	char *msg;
 	do
  	{
		msg = get_out();

		send_message(msg);
  	}while(strlen(msg) == INTERM_BLOCK_SIZE+5);
}

void send_write_request()
{
		char * msg, *flag, *address , *control, *crc;
		
		msg = (char*)calloc(sizeof(char), strlen(filename)+7);
	
		address = (char*)calloc(sizeof(char), 1);
		address[0] = BROADCAST;
	
		control = (char*)calloc(sizeof(char), 1);
		strcpy(control, "C");
		
		crc = (char*)calloc(sizeof(char), 1);
		
		filename[strlen(filename)-1] = 'R';
		crc[0] = 'C';

		flag = (char*)calloc(sizeof(char), 1);
		flag[0] = FLAG;
		
		strcat(msg, flag);
		strcat(msg, address);
		strcat(msg, control);		
		strcat(msg, filename);
		strcat(msg, crc);
		strcat(msg, flag);

		send_message(msg);
}

void rcv_out_buffer()
{
	char *msg, *aux;
	int condition = 0;
	do
	{
	msg = receive_message();
	aux = (char*)malloc(strlen(msg)-6);
	strncpy(aux, msg+3, strlen(msg)-6);
	//test CRC: 
		switch(msg[3])
		{
			case RCV_REQ:

			break;
			case SEND_REQ:

			break;
			case DATA:
			set_in(aux);
			condition = (strlen(aux) == IN_BLOCK_SIZE+1);
			break;
			case ERROR:

			break;
			default:
				printf("INVALID CODE RECEIVED");
		}
	}while(condition);	 

}