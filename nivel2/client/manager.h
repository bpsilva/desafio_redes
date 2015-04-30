#ifndef MANAGER_H
#define MANAGER_H

#define RCV_REQ 1
#define SEND_REQ 2
#define DATA 3
#define ERROR 4

#define BROADCAST 0x0FF
#define FLAG 0x7E

#include "out_buffer.h"
#include "in_buffer.h"
#include "interm_buffer.h"
#include "client_udp.h"

void init(int argc, char *argv[]);

//----------------FILE / INTERMIDIATE ----------------
void readFile();
pthread_t read_file_thread;

char filename[30];

void saveFile();
pthread_t write__file_thread;

//----------------- INTERMIDIATE BUFFER / OUT BUFFER----------
void interm_to_out();
pthread_t i_to_out_thread;


//----------------- OUT BUFFER / EXTERN----------
void send_out_buffer();
void rcv_out_buffer();

void send_write_request();

#endif