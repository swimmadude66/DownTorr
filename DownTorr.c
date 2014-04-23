#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <openssl/sha.h>
#include "Utils.h"


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




void generate_GET(Torrent *t)
{ 
    char * tracker= (char *)malloc(1024);
    strcpy(tracker, "GET ");
    strcat(tracker, t->announce);
    strcat(tracker,"?");
    strcat(tracker,"info_hash=");
    strcat(tracker,(char *)t->info_hash);
    strcat(tracker,"&peer_id=~||||_DOWNTORR_||||~&port=6881&uploaded=0&downloaded=0&left=");
    char length[100];
    sprintf(length,"%d",t->length);
    strcat(tracker,length);

    char *encoded =  url_encode(tracker);
    GET_request = (char *)malloc(1+strlen(encoded));
    strcpy(GET_request, encoded);
    free(tracker);

    printf("Get Request: %s\n",GET_request);
}






int main(int argc, char*argv[]){
  if (argc !=3){
    printf("Unable to Parse. Correct usage: DownTorr path_To_File desired_Location\n");
    exit(1);
  }
  else{
      torrent_file = argv[1];
      file_dest = argv[2];
  }

  setlocale(LC_ALL,"en_US.UTF-8");

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
  str_t *info_dict = get_info_dict(string);
//  t->info_hash = (unsigned char *)malloc(20);
//  unsigned char hashed[20];
  SHA1((unsigned char *)info_dict->string, info_dict->length, t->info_hash);
  free(string);
//  printf("INFO HASH: %s\n", hashed);
//  memcpy(t->info_hash,hashed,20);
  printf("Downloading to %s%s\n",file_dest,t->name);

  printf("announce: %s\n",t->announce);
  printf("name: %s\n",t->name);
  printf("piece length: %d\n",t->piece_length);
  printf("length: %d\n",t->length);
  printf("path: %s\n",t->path);
  printf("url-list: %s\n",t->url_list);
  printf("num_pieces: %d\n",t->pieces_size/20);
/*
  int i =0;
  for(;i< t->pieces_size;i++){
    printf("%c",t->pieces[i]);
  }
  printf("\n");
*/

 generate_GET(t);


  return 0; 
}

