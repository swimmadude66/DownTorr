CC=gcc
CFLAGS=-Wall -g
DEPS=tbl/tbl.h
OBJS=DownTorr.o

DownTorr: DownTorr.c
	$(CC) $(CFLAGS) DownTorr.c tbl/tbl.c tbl/tbl.h -o DownTorr 

Bencoding: Bencoding.c
	$(CC) $(CFLAGS) Bencoding.c -o Bencoding
clean:
	rm DownTorr
	
