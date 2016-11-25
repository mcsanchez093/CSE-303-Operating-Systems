#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
// hello() - print some output so we know the shared object loaded.
//
// The function signature takes an untyped parameter, and returns an untyped
// parameter.  In this way, the signature *could* support any behavior, by
// passing in a struct and returning a struct.

extern struct team_t {
  char *name1;
  char *email1;
  char *name2;
  char *email2;
} team;


/*char *strup(char *str){
   int i = 0;   
   char * dup = malloc(sizeof(str));
   while(str[i]){
     dup[i]= toupper(str[i]);
     i++;
   }
   return dup;
   }*/

void *hello(void *input) {
    printf("hello from a .so\n");
    return NULL;   
}


void *ucase(struct team_t *squad){
   struct team_t *a1=malloc(sizeof(struct team_t));;
   
   int i = 0;
   a1->name1 = malloc(sizeof(squad->name1));
   while((squad->name1)[i])
     {
       a1->name1[i]=toupper((squad->name1)[i]);
       i++;
     }


   i=0;
   a1->email1 = malloc(sizeof(squad->email1));
   while((squad->email1)[i])
     {
       a1->email1[i]=toupper((squad->email1)[i]);
       i++;
     }

   i=0;
   a1->name2 = malloc(sizeof(squad->name2));
   while((squad->name2)[i])
     {
       a1->name2[i]=toupper((squad->name2)[i]);
       i++;
     }

   i=0;
   a1->email2 = malloc(sizeof(squad->email2));
   while((squad->email2)[i])
     {
       a1->email2[i]=toupper((squad->email2)[i]);
       i++;
     }
  /*free(squad->email1);
  free(squad->name1);
  free(squad->email2);
  free(squad->name2);
  free(squad);*/
   squad = NULL;
   
   return a1;
   //a1.name1 = strup(squad->name1);
   //strcpy(a1.name1,copy);
   //free(copy);
   
   // a1.email1 = strup(squad->email1);
   //a1.name2 = strup(squad->name2);
   //a1.email2 = strup(squad->email2);
   
   //printTeam(a1);
}
