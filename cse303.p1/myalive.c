#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "support.h"

void ctrlc(int c);
void seconds(int c);
void quitIt(int c);

volatile sig_atomic_t end=0; //kill
volatile sig_atomic_t quit=0; //leave
volatile sig_atomic_t input=0;



void alive(void) {
    /* TODO: Complete this function */
    /* Note: you will probably need to implement some other functions */


  struct timeval time1;
  struct timeval time2;

  signal(SIGALRM,seconds);
  signal(SIGKILL,quitIt);
  signal(SIGINT, ctrlc);

  gettimeofday(&time1,NULL);
  alarm(10);

  int x =0;
  while(x==0)
    {
      if(end==1)
	{
	  end=0;
	  break;
	}
      if(quit==1)
	{
	  quit=0;
	  break;
	}
      if(input==1)
	{
	  printf("\nno\n");
	  alarm(10);
	  input=0;
	}
    }
  gettimeofday(&time2,NULL);
  double passed = (time2.tv_sec-time1.tv_sec)+((time2.tv_usec-time1.tv_usec)/1000000.00);
  printf("%f\n",passed);

}

void seconds(int c)
{
  end=1;
}
void quitIt(int c)
{
  quit=1;
}
void ctrlc(int c)
{
  input=1;
}
/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s\n", progname);
    printf("Swallow SIGINT, exit on SIGALARM\n");
}

/*
 * main() - The main routine parses arguments and dispatches to the
 *          task-specific code.
 */
int main(int argc, char **argv) {
    /* for getopt */
    long opt;

    /* run a student name check */
    check_student(argv[0]);

    /* parse the command-line options.  For this program, we only support  */
    /* the parameterless 'h' option, for getting help on program usage. */
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch(opt) {
          case 'h': help(argv[0]); break;
        }
    }

    /* no need to look at arguments before calling the code */
    alive();
}
