CC=gcc
CFLAGS=-Wall -g

DownTorr: DownTorr.c
	$(CC) $(CFLAGS) DownTorr.c Utils.c Utils.h -lssl -lcrypto -o DownTorr 
	
clean:
	rm DownTorr
	
