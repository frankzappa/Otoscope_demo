all: client

client : aes_ende.c client.c
	gcc -g -Wall -o $@ $^ -lpthread

clean:
	-rm client
	-rm -rf client.dSYM
	-rm .DS_Store
	-rm 1.h264
