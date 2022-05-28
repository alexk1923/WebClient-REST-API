CC=gcc
CFLAGS=-I -g

client: client.c requests.c helpers.c buffer.c parson.c commands.c prompts.c
	$(CC) -Wall -g client.c parson.c requests.c helpers.c buffer.c commands.c prompts.c -o client

run: client
	./client

clean:
	rm -f *.o client
