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
  if(strcmp(commands[0], "cd") == 0){
    // doesn't seem to change confused 
    //printf("%s\n", commands[1]);
    chdir(commands[1]);
    return 0;
  }

  if(strcmp(commands[0], "exit") == 0){
    kill(getppid(), SIGUSR1);
    return 0;
  }
  
  execvp(commands[0],commands);
  return 0;
  }

void redirOut(char* a, char* file) {
  int sout = dup(STDOUT_FILENO); 
  int f = open(file, O_WRONLY|O_CREAT, 0644);
  dup2(f,STDOUT_FILENO); 
  execute(a);
  dup2(sout, STDOUT_FILENO);
  close(f);
}

void redirIn(char* a, char* file) {
  int sin = dup(STDIN_FILENO);
  int f = open(file, O_RDONLY, 0644);
  dup2(f, STDIN_FILENO);
  execute(a);
  dup2(sin, STDIN_FILENO);
  close(f);
}

void pipes(char *a, char *b){
  char *file = "connector";
  redirOut(a, file);
  redirIn(b, file);
  remove(file);
}

char ** parsesemi(char * a, char * spliter){
  char ** b = (char **)(malloc(sizeof(char *)));
  int w = 0;
  char * s = a;
  while(s){
    b[w] = strsep(&s, spliter);
    //printf("%s\n", strsep(&s, ";"));
    w ++;
  }
  return  b;
}

int main(){
  int p = getpid();
  while(1){
    if(getpid() == p){
      char a[256];
      printf(">>> ");
      fgets(a,sizeof(a),stdin);
      char ** f = parsesemi(a, ";");
      int i = 0;
      // cd only works when it's first
      if(strstr(f[0], "cd ")){
	execute(f[0]);
	i = 1;
      }
      while(f[i]){
	fork();
	// output
	if (getppid() == p && strstr(f[i],">") != NULL){
	  char ** m = parsesemi(f[i],">");
	  if(strstr(m[1],"\n")){
	    *(strstr(m[1],"\n")) = 0;
	  }
	  redirOut(m[0],m[1]);
	}
	// input
	else if (getppid() == p && strstr(f[i],"<") != NULL){
          char ** m = parsesemi(f[i],"<");
          if(strstr(m[1],"\n")){
            *(strstr(m[1],"\n")) = 0;
          }
          redirIn(m[0],m[1]);
        }
	// pipes
	else if (getppid() == p && strstr(f[i],"|") != NULL){
	  char ** m = parsesemi(f[i],"|");
	  pipes(m[0],m[1]);
	}
	// regular commands
	else if (getppid() == p){
	  execute(f[i]);
	}
	wait();
	// forks stop after execute
	i ++;
      }
      wait();
    }
  }
  return 0;
}

