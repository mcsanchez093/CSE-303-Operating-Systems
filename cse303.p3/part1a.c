#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "support.h"

/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s\n", progname);
    printf("(This is just a demonstration of the traditional way\n");
    printf(" to use .so filed)\n");
}

/*
 * declare the hello() function that lives in a shared library
 */
extern void *hello(void* param);


extern void *ucase(struct team_t *squad);

void printTeam(struct team_t *a1){
     printf("Student 1 : %s\n", a1->name1);
     printf("Email 1   : %s\n", a1->email1);
     printf("Student 2 : %s\n", a1->name2);
     printf("Email 2   : %s\n", a1->email2);
     printf("\n");
}



/*
 * main() - The main routine parses arguments and invokes hello
 */
int main(int argc, char **argv) {
    /* for getopt */
    long opt;

    /* run a student name check */
     check_team(argv[0]);

    /* parse the command-line options.  For this program, we only support  */
    /* the parameterless 'h' option, for getting help on program usage. */
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch(opt) {
          case 'h': help(argv[0]); break;
        }
    }

   hello(NULL);
   
 
   //printTeam((struct team_t *)ucase(&team));
   
   struct team_t *team1 = (struct team_t *)ucase(&team);  
   printTeam(team1);
   
   //free(&team);
   
   free(team1->name1);
   free(team1->email1);
   free(team1->name2);
   free(team1->email2);
   free(team1);
   team1 = NULL;
   
    //   free(team.name1);
   // free(team.name2);
   // free(team.email1);
   //free(team.email2);
   // free(team);
   //
 
    /* TODO: execute the new function "ucase" that you added to libpart1.c */
    
    exit(0);
}
