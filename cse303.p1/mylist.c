#include <stddef.h>
#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "support.h"
#include <unistd.h>


typedef struct n
{
  int vals;
  struct n  *r;
} n1;

n1 *head= NULL;

void push(int i)
{
  n1 *holder;
  n1 *past;
  n1 *present;

  int len =0;
  int ii = i;

  while(ii!=0)
    {
      len++;
      ii/=10;
    }

  present =malloc((len *(sizeof(int)))+sizeof(void *));
  present -> vals = i;
  present -> r = NULL;

  if(head == NULL)
    {
      head = present;
    }
  else if(i<head -> vals)
    {
      present -> r = head;
      head = present;
    }
  else
    {
      past = head;
      holder= head -> r;


      while(holder -> vals <=i && holder != NULL)
	{
	  past=holder;
	  holder = holder -> r;
	  
	}
      if(holder == NULL)
	{
	  past -> r = present;
	}
      else
	{
	  present -> r = holder;
	  past->r=present;
	} 
    }  
}

void pop(int i)
{
  n1 *holder;
  n1 *future;
  n1 *present;

  future = head -> r;
  present = head;

  while(present!=NULL && future != NULL)
    {
      int c =0;
      if(future -> vals == i)
	{
	  holder = future;
	  present -> r = future -> r;
	  free(holder);
	  c=1;
	}
      if(c==0)
	{
	  present = future;
	  future = future -> r;
	}
    }
}


void clearList()
{
  n1 *holder;
  n1 *future;
  future = NULL;
  holder = head;

  while(holder != NULL)
    {
      future = holder -> r;
      free(holder);
      holder=future;
    }
  head=NULL;
}


void printer()
{
  n1 *holder;
  holder=head;

  while(holder!= NULL)
    {
      if(holder->r != NULL)
	{
	  printf("%d-->",holder->vals);
	}
      else
	{
	  printf("%d",holder->vals);
	}
      holder=holder->r;
    }    
}


void list_task(void) {
    /* TODO: Complete this function */
  char input[4];
  int num;

  while(fgets(input,4,stdin)!=NULL)
    {
      //int num;
      printf("LOOP\n");
      char v;
      v=input[2];
      num=v-'0';   
      printf("input[0]=%c\n",input[0]);
      printf("num=%i\n",num);
      char c = input[0];
      printf("c=%c\n",c);
      if(c==105)
	{
	  printf("105\n");
	  push(num);
	  printf("pushed\n");
	}
      else if(c==114)
	{
	  printf("114\n");
	  pop(num);
	  
	}
      else if(c==112)
	{
	  printf("112\n");
	  
	  printer();
	  
	}
      else if(c==120)
	{
	  printf("120\n");
      
	  clearList();
	  exit(EXIT_SUCCESS);
	}
      printf("BOTTOM\n");
    }
 }

/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s\n", progname);
    printf("Run an interactive list manipulation program\n");
    printf("Commands: [i]insert, [r]emove, [p]rint, e[x]it\n");
    printf("  (Commands require an integer argument)\n");
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

    /*
     * parse the command-line options.  For this program, we only support the
     * parameterless 'h' option, for getting help on program usage.
     */
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch(opt) {
          case 'h': help(argv[0]); break;
        }
    }

    /* run the list program */
    list_task();
}
