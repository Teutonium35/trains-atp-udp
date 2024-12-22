all : client server

client: client.c rbc.c
	gcc client.c -o client

server: server.c rbc.c
	gcc server.c -o server

rbc: rbc.c
	gcc rbc.c -o rbc

clean:
	rm -rf client server rbc