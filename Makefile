CC=gcc
CFLAGS=-I -g

client: client.c requests.c helpers.c buffer.c parson.c
	$(CC) -Wall -g client.c parson.c requests.c helpers.c buffer.c -o client

run: client
	./client

clean:
	rm -f *.o client
