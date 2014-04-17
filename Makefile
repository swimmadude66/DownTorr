CC=gcc
CFLAGS=

DownTorr: DownTorr.o
	$(CC) -o DownTorr DownTorr.o
	
clean:
	rm DownTorr
	rm *.o
	
