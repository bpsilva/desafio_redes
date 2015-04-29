


all: interm_buffer.o out_buffer.o main.o
	gcc -o level2 interm_buffer.o out_buffer.o main.o -pthread

main.o:main.c
	gcc -c main.c -Wall 

out_buffer.o:out_buffer.c
	gcc -c out_buffer.c -Wall 

interm_buffer.o: interm_buffer.c
	gcc -c interm_buffer.c -Wall 

clean:
		rm *.o
		rm level2
