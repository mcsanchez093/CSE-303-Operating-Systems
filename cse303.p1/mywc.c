#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "support.h"
#include <string.h>
/*
 * wc_file() - count characters, words, and lines
 */
void wc_file(char *filename) {
    /* TODO: Complete this function */
 
  if(strlen(filename)>0)
    {
     FILE  *fp = fopen(filename,"r");
     int c = 0;
     int chars =0;
     int wordCompleted = 1;
     int words = 0;
     int lines = 0;
     while(!feof(fp))
       {
	 c = fgetc(fp);
	 chars++;
	 if(c=='\t' || c=='\n' || c==' ' )
	   {
	     if(wordCompleted==0)
	       {
		 wordCompleted=1;
		 words++;
	       }
	     if(c=='\n')
	       {
		 lines++;
	       }
	   }
	 if((c>=65 && c<=90)||(c>=97 && c<=122)||(c>=48 && c<=57))
	   {
	     wordCompleted = 0;
	   }
       }
     printf("%d  %d  %d %s", lines, words, chars, filename);
    }
}

/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s [FILE]\n", progname);
    printf("Print newline, word, and byte counts for FILE\n");
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

    /* error if we don't have a file, else call wc_file */
    if (argc != 2) {
        printf("Error: no file given\n");
        exit(1);
    }
    wc_file(argv[1]);
}
