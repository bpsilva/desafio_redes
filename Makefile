target: 

server: server_udp.c
	gcc -o server server_udp.c

client:	client_udp.c
	gcc -o client client_udp.c

clean:
	rm client
	rm server
