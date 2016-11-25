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


void *malloc(size_t size){
  int buf;
  void *buffer[100];
  char **strings;


  //fputs("in malloc",stdout);
  static void *(*true_malloc)(size_t) = NULL;
  if(!true_malloc)
      true_malloc = dlsym(RTLD_NEXT, "malloc");

  void *rPoint = true_malloc(size);
  static int i = 0;
  if(!i){
    i = 1;
     buf = backtrace(buffer,100);
     strings = backtrace_symbols(buffer,buf);
     map_insert((uintptr_t) rPoint, strings[0], strings[1]);
    i = 0;
   }

  return rPoint;
}


void free(void *ptr){
 static void *(*true_free)(void *) = NULL;
 
 if(!true_free)
      true_free = dlsym(RTLD_NEXT, "free");
  
  static int i = 0;
  
  if(!i){
     i=1;
     map_remove((uintptr_t)ptr);
     true_free(ptr);  
     i=0;
     }
}
