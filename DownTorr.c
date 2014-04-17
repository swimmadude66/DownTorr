#include <stdio.h>                  /* for printf() and fprintf() */
#include <sys/socket.h>             /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>              /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char* torrent_file;
char* file_dest;

int main(int argc, char*argv[]){
  if (argc !=3){
    printf("Unable to Parse. Correct usage: DownTorr path_To_File desired_Location\n");
  }
  else{
      torrent_file = argv[1];
      file_dest = argv[2];
  }
  
  printf("Downloading to %s\n",file_dest);
  
}