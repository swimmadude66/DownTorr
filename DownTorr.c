#include <stdio.h>      
#include <sys/socket.h>               
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Bencoding.h"


char* torrent_file;
char* file_dest;

char* file_buf;

int num_peers;

struct Peer{
  int piece;
  int size;
  char* peer_buf;
}peer;

int main(int argc, char*argv[]){
  if (argc !=3){
    printf("Unable to Parse. Correct usage: DownTorr path_To_File desired_Location\n");
    exit(1);
  }
  else{
      torrent_file = argv[1];
      file_dest = argv[2];
  }
  
  printf("Downloading to %s\n",file_dest);
  
  FILE *f = fopen(torrent_file, "rb");
	if(f==NULL){
		printf("Cannot open file %s\n",torrent_file);
		exit(-1);
	}

  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  rewind(f);

  char *string = malloc(fsize + 1);
  int size_read = fread(string, fsize, 1, f);
  fclose(f);

  printf("size_read: %d\n",size_read);
  printf("torrent file contents: %s\n",string);

  Bencoding *b=parse_start(string,fsize);
  print_bencoding(b,0);

  free(string);

  return 0; 
}

