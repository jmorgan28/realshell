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

int main(){
      char a[256];
      char * b[256];
      fgets(a,sizeof(a),stdin);
      int w = 0;
      char * s = a;
      while(w < sizeof(s)){
	b[w] = strsep(&s, ";");
	w ++;
      }
      printf("%s\n",b[0]);
      printf("%s\n",b[1]);
      printf("%s\n",b[2]);
      return 0;
}
