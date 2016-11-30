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


int execute(char * a){
  char *s = a;
  //must check for \n
  if(strstr(a,"\n")){
    *(strstr(a,"\n")) = 0;
  }
  char * commands[256];
  int w = 0;
  while(w < sizeof(s)){
    commands[w] = strsep(&s, " ");
    w ++;
  }
  commands[w] = 0;
  
  execvp(commands[0],commands);
  return 0;
  }

char ** parsesemi(char * a){
  char ** b = (char **)(malloc(sizeof(char *)));
  int w = 0;
  char * s = a;
  while(s){
    b[w] = strsep(&s, ";");
    //printf("%s\n", strsep(&s, ";"));
    w ++;
  }
  return  b;
}

int main(){
  int p = getpid();
  while(1){
    fork();
    if (getppid() == p){
      printf(">>>");
      char a[256];
      fgets(a,sizeof(a),stdin);
      char ** p = parsesemi(a);
      execute(p[0]);
      exit(0);
    }
    wait();
  }
  return 0;
}
