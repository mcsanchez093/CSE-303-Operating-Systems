#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <execinfo.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

/*
 * We aren't providing much code here.  You'll need to implement your own
 * malloc() and free(), as well as any constructors or destructors for your
 * library.
 */

/*
 * Declarations for the functions in list.c, so that we don't need an
 * extra header file.
 */



int map_insert(uintptr_t pointer, char *module, char *line);
int map_remove(uintptr_t pointer);
int map_count();
void map_dump();






/* TODO: Your code goes here */


//void *malloc(size_t sz){
   //fputs("in malloc",stdout); 
   
  // int sizeBuf;
   //void *buf [100];
   //char **strings;
    
   //static int inside = 0;
   
        /*fputs(strings[0],stdout);
	fputs("\n",stdout);
	fputs(strings[1],stdout);
	fputs("\n",stdout);
	fputs(strings[2],stdout);
	fputs("\n",stdout);
	fputs(strings[3],stdout);
	fputs("\n",stdout);
	fputs(strings[4],stdout);*/

	

   
   //fputs("in malloc\n",stdout);
void *malloc(size_t sz){
   fputs("in malloc\n",stdout);
   void *(*true_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
   void *rPointer = true_malloc(sz);
   fputs("made it",stdout);
   return rPointer;
      
  }
  /* void * returnPointer = NULL;
   
    if(!true_malloc)
        fputs("inside",stdout);
	true_malloc = dlsym(RTLD_NEXT, "malloc");

    returnPointer = true_malloc(sz);
    
    fputs("made it through",stdout);
    return returnPointer; */
// }
    //fputs("here\n",stdout);
    /*
    if(!inside)
      {
	fputs("inside the if\n",stdout);
	inside = 1;
	//sizeBuf =(uintptr_t)backtrace(buf,100);
	//sizeBuf=backtrace(buf,100);
	fputs("HERE",stdout);
	//strings = backtrace_symbols(buf, sizeBuf);
	fputs(strings[0],stdout);
	fputs("here yo",stdout);
	if(strings == NULL){
	  fputs("this shit aint sopposed to print out\n",stdout); 
	  perror("backtrace_symbols");
	  exit(EXIT_FAILURE);
	}
	//fputs("fdfefewfwefwef",stdout);
	//map_insert((uintptr_t) returnPointer, strings[1],strings[2]);
	//fputs("this shit shood print",stdout);
	inside = 0;
      }*/
    //fputs("this is working",stdout);
    //fputs((char*)(uintptr_t)sizeBuf,stdout);
    // map_insert((uintptr_t) returnPointer,strings[1],strings[2]);
    
    // fputs("in malloc\n", stdout);
    //inside = 0;
  
  
  //return returnPointer;





void free(void *ptr){
   
   static void (*true_free)(void *) = NULL; 
   
   // fputs("in free\n",stdout);

   if(!true_free){
      true_free = dlsym(RTLD_NEXT, "free");
      //fputs("in !true_free\n",stdout);
   }      
    
    //map_remove((uintptr_t)ptr);
    static int inside = 0;
    if(!inside){
       inside = 1;
       //fputs("in free\n",stdout);
       true_free(ptr);
       //fputs("true_free worked\n",stdout);
       inside = 0;
    }
}




