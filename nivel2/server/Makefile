
all: interm_buffer.o out_buffer.o main.o server_udp.o in_buffer.o manager.o
	gcc -o server interm_buffer.o out_buffer.o main.o server_udp.o in_buffer.o manager.o -pthread

main.o:main.c
	gcc -c main.c -Wall 

manager.o: manager.c
	gcc -c manager.c -Wall 

server_udp.o: server_udp.c
	gcc -c server_udp.c -Wall 

out_buffer.o:out_buffer.c
	gcc -c out_buffer.c -Wall 

in_buffer.o: in_buffer.c
	gcc -c in_buffer.c -Wall

interm_buffer.o: interm_buffer.c
	gcc -c interm_buffer.c -Wall 

clean:
		rm *.o
		rm server
