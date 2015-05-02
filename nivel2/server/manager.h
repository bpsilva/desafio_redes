#ifndef MANAGER_H
#define MANAGER_H

#define RCV_REQ 1
#define SEND_REQ 2
#define DATA 3
#define ERROR 4

#define FLAG_POS 0
#define ADDRESS_POS 1
#define CONTROL_POS 2
#define OPCODE_POS 3

#define BROADCAST 0x0FF
#define FLAG 0x7E

#include "out_buffer.h"
#include "in_buffer.h"
#include "interm_buffer.h"
#include "server_udp.h"

void init();

//----------------FILE / INTERMIDIATE ----------------
void readFile();
pthread_t read_file_thread;

char filename[30];

void saveFile();
pthread_t write__file_thread;

//----------------- INTERMIDIATE BUFFER / OUT BUFFER----------
void interm_to_out();
pthread_t i_to_out_thread;
pthread_t confirm;
pthread_t save_f;
pthread_t i_to_out_thread;
void confirmation();

//----------------- OUT BUFFER / EXTERN----------
void send_out_buffer();
void rcv_out_buffer();
void send_write_request();

void recv_to_in();
void entry_monitor();

#endif
