#define _GNU_SOURCE
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include "support.h"
#include <unistd.h>

/*
 * host() - Print the hostname of the machine on which this program is
 *          running, formatted a few different ways
 */
void host(void) {
    /* TODO: Complete this function */
  char name[128];
  gethostname(name, sizeof name);
  printf("%s\n",name);
  
  for(int i = 0; name[i]; i++){
    name[i] = tolower(name[i]);
  }
  printf("%s\n",name);

  for(int i = 0; name[i]; i++){
    name[i] = toupper(name[i]);
  }
  printf("%s\n",name);

  for(int i = 0; name[i]; i++){
    if(i%2==0)
      {
	name[i] = tolower(name[i]);
      }
    else
      name[i]=toupper(name[i]);
  }
  printf("%s\n",name);

  gethostname(name, sizeof name);
  int length = strlen(name);
  for(int i = length-1; name[i]; i--){
    printf("%c",name[i]);
  }
  printf("\n");
}

/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s\n", progname);
    printf("Display the hostname of the machine\n");
    printf("The name will be displayed four ways, in the following order, on "
           "four lines:\n");
    printf("  all lowercase\n");
    printf("  ALL UPPERCASE\n");
    printf("  mIxEd LoWeRaNdUpPeR\n");
    printf("  emantsoh (i.e., reversed)\n");
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
    host();
}
