#ifndef IN_BUFFER_H
#define IN_BUFFER_H

#define IN_BUFFER_SIZE 8
#define IN_BLOCK_SIZE 512


#include <pthread.h>
#include <semaphore.h>
int in_get;
int in_win_end;
int in_win_begin;


char* in_buffer[IN_BUFFER_SIZE];

char* get_in(); // gets the avaliable data but dont deletes any data from the buffer
void set_in(char *data); // insert data 

sem_t in_data; // how many data blocks are avaliable for reading
sem_t in_buffer_access;//mutual exclusion to set or get data
sem_t in_buffer_space;//manage the maximum capacity of the buffer


#endif
