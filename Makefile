CC=gcc
CFLAGS=-Wall -g
DEPS=tbl.h
OBJS=DownTorr.o

DownTorr: DownTorr.c
	$(CC) $(CFLAGS) DownTorr.c tbl.c tbl.h -o DownTorr 
clean:
	rm DownTorr $(OBJS) *.gch
	
