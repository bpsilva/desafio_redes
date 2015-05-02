#include "manager.h"

#include "out_buffer.h"
#include "in_buffer.h"
#include "interm_buffer.h"
#include "server_udp.h"


void saveFile()
{
	
	FILE * pOut;
  	pOut = fopen (filename, "wb+");
  	char* buffer;
  do
  {
	
  	buffer = get_in();
  	fwrite (buffer , sizeof(char), strlen(buffer), pOut);	
  }while(strlen(buffer) == INTERM_BLOCK_SIZE);
}


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
	connection_server();

}

void recv_to_in()
{
	char buf[518];
	char aux[512];
	do
	{	
		memset(buf,0,strlen(buf));
		serv.n = recvfrom(serv.sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &serv.cli_addr, &serv.clilen);
		if (serv.n < 0) 
			printf("ERROR on recvfrom");

		//teste CRC
		printf("%s\n", buf);
		strncpy(aux, buf+4, strlen(buf)-6);
		set_in(aux);
			
	}while(strlen(buf) == INTERM_BLOCK_SIZE+6);	
}


void confirmation()
{

		
		char * msg, *flag, *address , *control, *crc, *opcode;
		
		msg = (char*)calloc(sizeof(char), strlen(filename)+6);
	
		address = (char*)calloc(sizeof(char), 1);
		address[0] = BROADCAST;
	
		control = (char*)calloc(sizeof(char), 1);
		control[0] = in_win_end;
		
		opcode = (char*)calloc(sizeof(char), 1);
		opcode[0] = 0;

		crc = (char*)calloc(sizeof(char), 2);
		
		crc[0] =  'R';
		crc[1] = 'C';

		flag = (char*)calloc(sizeof(char), 1);
		flag[0] = FLAG;
		
		strcat(msg, flag);
		strcat(msg, address);
		strcat(msg, control);		
		strcat(msg, opcode);
		strcat(msg, crc);
		strcat(msg, flag);

		send_message(msg);//mensagem de confirmação
}

void entry_monitor()
{
		char buf[520];
		printf("testa");
		serv.n = recvfrom(serv.sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &serv.cli_addr, &serv.clilen);
		if (serv.n < 0) 
			printf("ERROR on recvfrom");
printf("testb");
	printf("%i",buf[OPCODE_POS]);
	
	switch(buf[OPCODE_POS])
	{
		case RCV_REQ:
	
			break;
		case SEND_REQ:

			pthread_create(&recv_in, NULL, (void*)recv_to_in, NULL);
			pthread_create(&save_f, NULL, (void*)saveFile, NULL);
			pthread_create(&confirm, NULL, (void*)confirmation, NULL);

			pthread_join(recv_in, NULL);
			pthread_join(save_f, NULL);
			//as confirmações param quando o arquivo terminou de ser salvo, por isso não precisa de join
			break;
//		case DATA:
//			break;
		case ERROR:
			break;
	}
}