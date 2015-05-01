#ifndef OUT_BUFFER_H
#define OUT_BUFFER_H
#define OUT_BUFFER_SIZE 8

#define OP_DATA 3

#include <pthread.h>
#include <semaphore.h>
int out_get;
int out_win_end;
int out_win_begin;


char* out_buffer[OUT_BUFFER_SIZE];

char* get_out(); // gets the avaliable data but dont deletes any data of the buffer
void set_out(char *data); // insert data on out buffer


sem_t out_data; // how many data blocks are avaliable for reading
sem_t out_buffer_access;//mutual exclusion to set or get data
sem_t out_buffer_space;//manage the maximum capacity of the buffer


#endif
