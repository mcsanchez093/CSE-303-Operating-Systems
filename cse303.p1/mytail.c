#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "support.h"
#include <string.h>
/*
 * tail_file() - display the last numlines lines of a file or stdin
 */
void tail_file(char *filename, int numlines) {
  /* TODO: Complete this function */
  FILE *fp = fopen(filename,"r");

  char words[1024];
  char holder[numlines+1][1024];
  int ch=0;

  if(fp==NULL){
    while (fgets(words, 1024, stdin)){
      strcpy(holder[ch], words);
      if(ch==numlines){
	for(int j=0; j<numlines; j++){
	  strcpy(holder[j],holder[j+1]);
	}
      }
      else{
	ch++;
      }
    }

    for(int i=0; i<numlines; i++){
      printf("%s\n", holder[i]);
    }
  }

  else{
    while (fgets(words, 1024, fp)){
      strcpy(holder[ch], words);
      if(ch==numlines){
	for(int j=0; j<numlines; j++){
	  strcpy(holder[j],holder[j+1]);
	}
      }
      else{
	ch++;
      }
    }

    for(int i=0; i<numlines; i++){
      printf("%s\n", holder[i]);
    }
  }

}

/*
 * help() - Print a help message.
 */
void help(char *progname) {
  printf("Usage: %s [FILE]\n", progname);
  printf("Print the last 10 lines  of FILE to STDOUT\n");
  printf("With no FILE, read from STDIN\n");
  printf("  -nK    print the last K lines instead of the last 10\n");
}

/*
 * main() - The main routine parses arguments and dispatches to the
 *          task-specific code.
 */
int main(int argc, char **argv) {
  /* for getopt */
  long opt;
  int numlines=10;
  //    int index;
  /* run a student name check */
  check_student(argv[0]);

  /* parse the command-line options.  For this program, we only
     support  */
  /* the parameterless 'h' option, for getting help on program usage, */
  /* and the parametered 'n' option, for getting the number of lines. */
  /* TODO: parse the arguments correctly */
  while ((opt = getopt(argc, argv, "hn")) != -1) {
    switch(opt) {
    case 'h': help(argv[0]); break;
      //    printf("%s\n", optarg);
    case 'n': numlines = atoi(argv[optind]); break;
    }
  }
  if(optind!=1){
    optind++;
    /*  if(optind<argc){
	index = optind;
	      }
	            else{

		    }*/
  }
  
  tail_file(optind < argc ? argv[optind] : NULL, numlines);
}


/*#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "support.h"


 * tail_file() - display the last numlines lines of a file or stdin
 
//void tail_file(char *filename, int numlines) {
   TODO: Complete this function 


  
  //  printf("TEST");
    FILE *fp = fopen(filename,"r");
  int ch=0;
  char words[1024];
  char holder[numlines+1][1024];
  if(filename==NULL){
    while (fgets(words, 1024, stdin)){
      strcpy(holder[ch], words);
      if(ch==numlines){
	for(int j= 0; j <  numlines; j++){
	  strcpy(holder[j], holder[j+1]);
	}
      }
      else{
	ch++;
      }
    }

    for(int i = 0; i < numlines; i++){
      printf("%s\n", holder[i]);
    }
  }

  else{
    while (fgets(words, 1024, fp)){
      strcpy(holder[ch], words);
      if(ch==numlines){
	for(int j=0; j<numlines; j++){
	  strcpy(holder[j],holder[j+1]);
	}
      }
      else{
	ch++;
      }
    }

    for(int i=0; i<numlines; i++){
      printf("%s\n", holder[i]);
    }
  }

  



  


  
  int lines = 0;
  while(!feof(file))
    {
      ch = fgetc(file);
      if(ch == '\n')
	{
	  lines++;
	}
    }
  fclose(file);

  FILE *aFile = fopen(filename,"r");
  
  char aLine[1024];
  
  int printAll=1;
  if(lines-numlines>=0)
    {
      printAll = 0;
    }

  if(printAll==0)
    {
      int skipLines = lines-numlines;
      while(skipLines>0)
	{
	  fgets(aLine,1024,aFile);
	  skipLines--;
	}
    }

  
 
    
      
      while(!feof(file))
	{
	  fgets(aLine,1024,aFile);
	  printf("%s\n",aLine);
	}
 
  
}


 * help() - Print a help message.
 
void help(char *progname) {
    printf("Usage: %s [FILE]\n", progname);
    printf("Print the last 10 lines  of FILE to STDOUT\n");
    printf("With no FILE, read from STDIN\n");
    printf("  -nK    print the last K lines instead of the last 10\n");
}


 * main() - The main routine parses arguments and dispatches to the
 *          task-specific code.
 
//int main(int argc, char **argv) {
     for getopt 
   long opt;
    int numlines=10;
     run a student name check 
      check_student(argv[0]);

     parse the command-line options.  For this program, we only support  
     the parameterless 'h' option, for getting help on program usage, 
     and the parametered 'n' option, for getting the number of lines. 
     TODO: parse the arguments correctly
    while ((opt = getopt(argc, argv, "hn:")) != -1) {
        switch(opt) {
	case 'h': help(argv[0]); break;
	  //case 'n': numlines = *argv[2]- '0';break;
	case 'n': numlines = atoi(argv[optind]); break;
	}
    }
    if(optind!=1){
      optind++;
    }
    //    printf("%s",argv[3]);
    // TODO: fix this invocation 
    //   tail_file(argv[optind], numlines);
   tail_file(optind < argc ? argv[optind] : NULL, numlines);
}
*/
