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
#include "shell.h"

/*========int execute(char * a)=========
	Inputs: char * a
	Returns: int (0)

	Takes in a command
	Parses along space
	If cd does special case
	if exit does special case
	Else calls execvp
	====================*/

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


	/*======== void reOut(char* a, char* file) ==========
	Inputs: char* a
		char* file
	Returns: Nothing! It is void.

	Redirection for >
	Puts output of command to file
	====================*/

void redirOut(char* a, char* file) {
  int sout = dup(STDOUT_FILENO); 
  int f = open(file, O_WRONLY|O_CREAT, 0644);
  dup2(f,STDOUT_FILENO); 
  execute(a);
  dup2(sout, STDOUT_FILENO);
  close(f);
}


	/*======== void reIn(char* a, char* file) ==========
	Inputs: char* a
		char* file
	Returns: Nothing! It is void.

	Redirection for <
	Puts output of command to file
	====================*/

void redirIn(char* a, char* file) {
  int sin = dup(STDIN_FILENO);
  int f = open(file, O_RDONLY, 0644);
  dup2(f, STDIN_FILENO);
  execute(a);
  dup2(sin, STDIN_FILENO);
  close(f);
}

/*======== void pipes(char *a, char *b) ==========
	Inputs: char* a
		char* b
	Returns: Nothing! It is void!

	Uses redirOut and redirIn to implement pipes
	====================*/

void pipes(char *a, char *b){
  char *file = "connector";
  int p = getpid();
  int pp = 0;
  fork();
  if(getppid() == p){
    pp = fork();
    if(!pp){
      redirOut(a, file);
      //      printf("a");
    }
    else{
      wait();
      redirIn(b, file);
      //      printf("b");
    }
  }
  //  printf("c");
  wait();
  remove(file);
  exit(0);
}

/*======== char ** parsesemi(char * a, char * spliter) ==========
	Inputs: char* a
		char* spliter
	Returns: char **

	Takes in a line of text
	Parses based off given character
	Returns a 2d array with each command seperated
	====================*/


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


	/*======== int main() ==========
	Returns: int (0)

	Uses fgets
	Calls functions and forks based contents
	Deals with parent and child functions
	====================*/

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
	  if(strstr(m[1],"\n")){
            *(strstr(m[1],"\n")) = 0;
          }
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

