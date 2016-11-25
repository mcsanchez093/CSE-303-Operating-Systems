#include <execinfo.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
/*
 * We aren't providing much code here.  You'll need to implement your own
 * dlsym(), as well as any constructors or destructors for your library.
 */

/* TODO: Your code goes here */

int pipe (int pipefd[2]){ 
   
  int buf;
  void *buffer[1];
  char **strings;

  buf = backtrace(buffer,1);
  strings = backtrace_symbols(buffer,buf);
  
  char * addr = strings [0];
  
  //  mprotect(strings[1],1024,PROT_NONE);
  

}





