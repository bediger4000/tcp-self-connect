CFLAGS = -g -Wall -Wextra
CC = cc
self_connect: self_connect.c
	$(CC) $(CFLAGS) -o self_connect self_connect.c

run: self_connect
	./self_connect localhost 50000
