#include <sys/socket.h>
#include <locale.h>
#include <openssl/sha.h>
#include <stdio.h>		    	/* for printf() and fprintf() */
#include <sys/socket.h>		    /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		    /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* For addrinfo types */ 
#include <sys/types.h>
#include <netdb.h>
#include "Utils.h"

/* Constants */
#define RCVBUFSIZE 512		    /* The receive buffer size */
#define SNDBUFSIZE 512		    /* The send buffer size */
#define REPLYLEN 32

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
    char tracker[1024];
    char params[1024];
    char *encode;
    strcpy(tracker, t->announce);
    strcat(tracker,"?info_hash=");
    strcat(tracker,t->info_hash);
    strcpy(params,"&peer_id=");
    strcat(params,"xxDOWNTORRDOWNTORRxx");
    strcat(params,"&port=6881&compact=0&uploaded=0&downloaded=0&left=");
    char length[100];
    sprintf(length,"%d",t->length);
    strcat(params,length);
    encode = url_encode(params);
    GET_request = (char *)malloc(1+strlen(tracker)+strlen(encode));
    strcpy(GET_request, tracker);
    strcat(GET_request, encode);
}


int main(int argc, char*argv[]){
  if (argc !=3){
    printf("Unable to Parse. Correct usage: DownTorr path_To_File desired_Location\n");
    exit(1);
  }
  else{
      torrent_file = argv[1];
      file_dest = argv[2];
      setlocale(LC_ALL,"en_US.UTF-8");
  }

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
  unsigned char hashed[20];
  SHA1((unsigned char *)info_dict->string, info_dict->length, hashed);
  free(string);
  int i= 0;
  int big = 0;
  for(; i < 20; i++) {
	sprintf(&t->info_hash[big], "%%%02x", hashed[i]);
	big += 3;
  }

  generate_GET(t);

  printf("announce: %s\n",t->announce);
  printf("name: %s\n",t->name);
  printf("piece length: %d\n",t->piece_length);
  printf("length: %d\n",t->length);
  printf("url-list: %s\n",t->url_list);
  printf("num_pieces: %d\n",t->pieces_size/20);
  printf("Get request: %s\n",GET_request);

  Response *r = get_peers(GET_request, t->announce);
  
  
		
  
/*
  int i =0;
  for(;i< t->pieces_size;i++){
    printf("%c",t->pieces[i]);
  }
  printf("\n");
*/



//  printf("Downloading to %s%s\n",file_dest,t->name);

  return 0; 
}

