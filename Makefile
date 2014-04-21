CC=gcc
CFLAGS=-Wall -g

DownTorr: DownTorr.c
	$(CC) $(CFLAGS) DownTorr.c Bencoding.c Bencoding.h -o DownTorr 
	
clean:
	rm DownTorr
	
