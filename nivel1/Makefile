


all: interm_buffer.o
	gcc -o level1 interm_buffer.o -pthread

interm_buffer.o: interm_buffer.c
		gcc -c interm_buffer.c -Wall 

clean:
		rm *.o
		rm level1
