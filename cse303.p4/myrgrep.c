#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "support.h"
#include <dirent.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <getopt.h>

int  myCompare(const void* word1, const void* word2);
int  myCompare(const void* word1, const void* word2)
{
  const char* word11 = *(const char **)word1;
  const char* word22 = *(const char **)word2;

  int i = 0;
  int countx =0;
  int county = 0;
  for(i=0;i<sizeof(word11);i++)
    {
      char c1 = tolower(word11[countx]);
      while(c1 == 46 || c1 == 47)
	{
	  if((countx+1) <  sizeof(word11))
	    {
	      countx++;
	      c1 = tolower(word11[countx]);
	    }
	  else
	    c1 = 0;
	}

      char c2 = tolower(word22[county]);
      while(c2 == 46 || c2 ==47)
	{
	  if((county+1) <  sizeof(word22))
	    {
	      county++;
	      c2 = tolower(word22[county]);
	    }
	  else c2=0;
	}
      if(c1 == 0)
	return -1;
      else if(c2 == 0)
	return 1;

      if(c1 < c2)
	return -1;
      else if(c1 > c2)
	return 1;
      countx++;
      county++;
    }
  return 0;
}


/*
 * myrgrep() - find files (recursively) with matching pattern
 */
void myrgrep(char *pattern, char **roots) {
    /* TODO: Complete this function */
  DIR *dir;
  if ((dir = opendir (roots[2])) == NULL)
    {
      perror ("Cannot open .");
      exit (1);
    }

  int numFiles = 0;
  struct dirent *dp;
  while ((dp = readdir (dir)) != NULL)
    {
      //printf("READING-> %s\n",dp->d_name);
      struct stat stats;
      char* fullPath =  malloc (sizeof(dp->d_name)+sizeof(char)+20);
      
      if((strcmp(dp->d_name,".")!=0) && (strcmp(dp->d_name,"..")!=0))
	{
	  strcpy(fullPath,roots[2]);
	  strcat(fullPath,"/");
	  strcat(fullPath,dp->d_name);
	  stat(fullPath,&stats);
	  if(S_ISDIR(stats.st_mode) != 0)
	    {
	      //printf("%s is a DIR\n",fullPath);
	      char **recursion = (char**) malloc(sizeof(dp->d_name)+20+sizeof(char));
	      int u=0;
	      for(u=0;u<3;u++)
		{
		  recursion[u]=(char*) malloc(sizeof(dp->d_name)+sizeof(char)+20);
		}
	      //printf("malloced\n");
	      //strncpy(recursion[0]," ",20);
	      //strncpy(recursion[1],pattern,20);
	      strncpy(recursion[2],fullPath,strlen(fullPath));
	      //printf("About to recur\n");
	      myrgrep(pattern,recursion);
	    }
	  else if(S_ISREG(stats.st_mode) != 0)
	    {
	      //printf("is a file: %s\n",fullPath);
	      char *daLine;
	      int foundit=0;
	      FILE *file =fopen(fullPath,"r");
	      if(file!=NULL)
		{
		  if(file == 0)
		    {
		      printf("cant open file\n");
		    }
		  else
		    {
		      //puts("file!=0");
		      char *line = NULL;
		      size_t  len = 0;
		      ssize_t read;
		      while((read = getline(&line, &len, file)) != -1)
			{
			  //puts("in the loop ");
			  char *found = strstr(line,pattern);
			  if(found != NULL)
			    {
			      daLine=line;
			      foundit=1;
			      //printf("%s:%s\n",allFiles[c],line);
			    }
			}
		    }
		}
	      if(foundit==1)
		printf("%s:%s",fullPath,daLine);
	    }
	}
    }
}

  
    
    

  
      
      



/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s [OPTIONS] [PATTERN] [FILE]...\n", progname);
    printf("List the names of the FILEs (the current directory by default)\n");
    printf("whose contents contain PATTERN.\n");
    printf("The search should be recursive into subdirectories of any FILE\n");
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
    /* TODO: make sure to handle any other arguments specified by the */
    /*       assignment */
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch(opt) {
          case 'h': help(argv[0]); break;
        }
    }
    //printf("num args= %i\n",argc);
    /* TODO: fix this invocation */
    myrgrep(argv[1], argv);
}
