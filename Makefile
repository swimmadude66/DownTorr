CC=gcc
CFLAGS=-Wall -g

DownTorr: DownTorr.c
	$(CC) $(CFLAGS) DownTorr.c Bencoding.c Bencoding.h -lssl -lcrypto -o DownTorr 
	
clean:
	rm DownTorr
	
