#define _GNU_SOURCE
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "support.h"

/*
 * sed_file() - modify lines of filename based on pattern, and print result
 *              to STDOUT
 */
void sed_file(char *filename, char *pattern) {
    /* TODO: Complete this function */
  int numToks=0;
  char* real = malloc(sizeof(pattern));
  char *dup = malloc(sizeof(pattern));

  strcpy(real,pattern);
  strcpy(dup,pattern);

  //  printf("dup=%s\n",dup);
  //printf("real=%s\n",real);
  /**char *waste = strtok(real,"/");
  numToks++;
  printf("%s\n",waste);
  */
  char *find = NULL;
  char *replace = NULL;
  if(find==replace)
    {
      
    }
  int x= 0;
  int validInput=1;
  
  /*if(*waste!=115)
    {
      validInput = 0;
    }
*/
  int notNull=1;
  int dupToks=0;
  // char *dup = pattern;
  //printf("now dup is=%s\n",dup);
  char *w2 = strtok(dup,"/");
  //printf("BEFORE LOOP:%s\n",w2);
  while(w2!=NULL)
    {
      //printf("Inside tok loop\n");
      dupToks++;
      w2=strtok(NULL,"/");
      //printf("INSIDE LOOP:%s\n",w2);
    }
 
  if(dupToks==3)
    {
      notNull=0;
    }
  //printf("NOT NULL=%i\n",notNull);

  //printf("real is=%s\n",real);
  char *waste = strtok(real,"/");
  numToks++;
  //printf("%s\n",waste);
  if(*waste!=115)
    {
      validInput = 0;
    }
  
  
  while(numToks<dupToks && (validInput==1))
    {
      //printf("starting the inner loop\n");
      waste=strtok(NULL,"/");
      numToks++;
      // printf("WASTE=%s\n",waste);
      if(numToks == 2)
	{
	  find = waste;
	}
      if(notNull==1 && numToks ==3 )
	{
	  replace= waste;
	  /* if(*waste!=0)
	    {
	      replace=waste;
	      printf("shouldnt hit this line\n");
	      printf("%s\n",replace);
	      //replace="";
	    }
	  else
	    {
	      printf("waste=null\n");   
	      replace="";
	    }
	  */
	}
      if(notNull==1 && numToks==4)
	{
	  if(*waste!=103)
	    {
	      validInput=0;
	    }
	}
      if(notNull == 0 && numToks == 3)
	{
	  //if not g
	  if(*waste!=103)
	    {
	      validInput=0;
	    }
	  //if last tok  is g
	  else
	    {
	      replace="";
	    }
	}
      /*if(x==2)
	{
	  if(*waste!=103)
	    {
	      validInput=0;
	      // printf("invalid end char\n");
	    }
	    }*/
      x++;
    }
  //free(dup);
  //free(real);
  if(validInput==1)
    {
      FILE *fp = fopen(filename,"r");
      ssize_t read;
      if(read)
	{

	}
      size_t size = 0;
      char* buf=NULL;
      
      read = getline(&buf,&size,fp);
      while(read!=-1)
	{
	  int x =0;
	  while(buf[x]!='\n')
	    {
	      int found = 0;
	      int y=0;
	      int temp = x;
	      while((buf[temp]==find[y]) && (y<strlen(find)))
		{
		  if(strlen(find)-y==1)
		    {
		      printf("%s",replace);
		      found=1;
		    }
		  temp+=1;
		  y++;
		}
	      if(found==0)
		{
		  char p = buf[x];
		  printf("%c",p);
		  x++;
		}
	      else
		{
		  x+=strlen(find);
		}
	    }
	  printf("\n");
	  //char *after =strstr(buf,find);
	  //while(after!=NULL)
	  //{
	  //  int flen = strlen(find);
	  // }
	  /*
	  //char tok[1024];// = strtok(buf," ");
	  //strcpy(tok,strtok(buf," "));
	  char *tok = strtok(buf," ");
	  while(tok !=  NULL)
	    {
	      printf("TOK= %s\n",tok);
	      char* fromFindOn = strstr(tok,find);
	      printf("FFO= %s\n",fromFindOn);
	      int  x = strlen(tok)-strlen(fromFindOn);
	      char holder[1024]="";
	      int z = 0;
	      printf("x = %i\n",x);
	      
	      while(z < x)
		{
		  printf("TOK[0]= %i\n",tok[0]);
		  //int r = tok[z];
		  
		  char g=tok[z];
		  printf("g=%c\n",g);
		  //char b[1];
		  //sprintf(b,g);
		  //printf("b=%s\n",b);
		  // char *y = &b;
		  // printf("r= %i\n",r);
		  char *q = &g;
		  printf("%s\n",q);
		  strcat(holder,&g);
		  printf("HOLDER= %s\n",holder);
		  z++;
		}
	      printf("%s\n",holder);
	      strcat(holder,replace);
	      printf("%s\n",holder);
	      //int uptor = x
	      //tok=strtok(NULL," ");
	      strcpy(tok,strtok(NULL," "));
	      
	    }
	  */
	  read=getline(&buf,&size,fp);
	}
      
    }
  free(dup);
  free(real);
 }

/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s [FILE]\n", progname);
    printf("Print the contents of FILE to STDOUT\n");
    printf("With no FILE, read standard input\n");
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
    
   
    
    
    if(argc==3)
      {
	//printf(argv[1]);
	//char *x = argv[1];
	//printf(&x[0][0]);
	//printf("------");
	//char * f = argv[1];
	//printf("%i\n",f);

	sed_file(argv[2],argv[1]);
      }
    /* TODO: fix this invocation */
    //   sed_file(NULL, NULL);
}
