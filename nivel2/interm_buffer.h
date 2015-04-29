#ifndef INTERM_BUFFER_H
#define INTERM_BUFFER_H
#define INTERM_BUFFER_SIZE 100
#define INTERM_BLOCK_SIZE 512
#include <pthread.h>
#include <semaphore.h>
int interm_get_pos;
int interm_set_pos;


char* interm_buffer[INTERM_BUFFER_SIZE];//100

char* get_interm(); // gets the avaliable data
void set_interm(char *data); // insert data on interm buffer
void init();// semaphores, interm_set_pos, interm_get_pos

sem_t interm_data; // how many data blocks are avaliable for reading
sem_t interm_buffer_access;//mutual exclusion to set or get data
sem_t interm_buffer_space;//manage the maximum capacity of the buffer
void saveFile();
void readFile();
char filename[30];
pthread_t interm_read_thread;
pthread_t interm_write_thread;
#endif
