#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>



int main(int argc,char * argy[]){
  char a[256];
  char *s = a;
  fgets(a,sizeof(a),stdin);
  a[strlen(a) - 1] = 0;
  
  
  char * commands[256];
  int w = 0;
  while(w < sizeof(s)){
    commands[w] = strsep(&s, " ");
    w ++;
  }
  commands[w] = 0;
  
  //printf("%s\n", commands[0]);
  //printf("%s\n", commands[1]);
  execvp(commands[0],commands);
  //printf("works");
  return 0;
}
