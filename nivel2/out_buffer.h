#ifndef OUT_BUFFER_H
#define OUT_BUFFER_H
#define OUT_BUFFER_SIZE 8
#define OUT_BLOCK_SIZE sizeof(message)
#define OP_DATA 3

#include <pthread.h>
#include <semaphore.h>
int out_get;
int out_win_end;
int out_win_begin;

typedef struct message
{
char flag;
char address;
char control;
char opcode;
char* data;
char crc[2];
}message;


message* out_buffer[OUT_BUFFER_SIZE];

message* get_out(); // gets the avaliable data but dont deletes any data of the buffer
void set_out(message *data); // insert data on out buffer
void interm_to_out();

sem_t out_data; // how many data blocks are avaliable for reading
sem_t out_buffer_access;//mutual exclusion to set or get data
sem_t out_buffer_space;//manage the maximum capacity of the buffer


pthread_t read_thread;
pthread_t write_thread;
pthread_t i_to_out_thread;
#endif
