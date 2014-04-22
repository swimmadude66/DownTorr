#include <stdio.h>      
#include <sys/socket.h>               
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <openssl/sha.h>
#include "Bencoding.h"


char* torrent_file;
char* file_dest;


char* file_buf;
char* GET_request;


int num_peers;


struct Downloader{
  int piece;
  int size;
  char* peer_buf;
} Downloader;


int main(int argc, char*argv[]){
  if (argc !=3){
    printf("Unable to Parse. Correct usage: DownTorr path_To_File desired_Location\n");
    exit(1);
  }
  else{
      torrent_file = argv[1];
      file_dest = argv[2];
  }
 
  char *locale;
  locale = setlocale(LC_ALL,"en_US.UTF-8");

  FILE *f = fopen(torrent_file, "rb");
	if(f==NULL){
		printf("Cannot open file %s\n",torrent_file);
		exit(-1);
	}

  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  rewind(f);

  char *string = malloc(fsize + 1);
  fread(string, fsize, 1, f);
  fclose(f);
  string[fsize]=0;

  Torrent *t=parse_torrent(string,fsize);  
  
  char *info;
  info = get_info_dict(string);
  printf("info %s", info);
  free(string);

  printf("Downloading to %s%s\n",file_dest,t->name);

  int i =0;
  printf("announce: %s\n",t->announce);
  printf("name: %s\n",t->name);
  printf("piece length: %d\n",t->piece_length);
  printf("length: %d\n",t->length);
  printf("path: %s\n",t->path);
  printf("url-list: %s\n",t->url_list);
  printf("pieces: ");
  for(;i< t->pieces_size;i++){
    printf("%c",t->pieces[i]);
  }
  printf("\n");

//  int get_length = strlen(t->announce)+strlen(t->info_hash)+strlen(t->length)+46; 

//  GET_request = t->announce;
  

  

  return 0; 
}

