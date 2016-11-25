#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/*
 * Note: 'uintptr_t' is a special type of unsigned integer that is guaranteed
 *       to be the same size as pointers.  This is the preferred way to cast
 *       between pointers and integers in code that could be compiled in
 *       32-bit or 64-bit mode.
 */

/*
 * A type for nodes in a linked list.  The linked list will be used to map
 * pointers to strings, so we can save information about the state of the
 * program each time a pointer was returned by malloc()
 */
typedef struct map_node
{
  uintptr_t allocated_pointer;
  char      *call_site;
  char      *program_counter;
  struct map_node *nextHead;
} map_node_t;

/*
 * A list, based on map_node_t
 *
 * NB: The list is static, so that its name isn't visible from other .o files
 */
static map_node_t* alloc_info;
/*
 * insert() - when malloc() is called, your interpositioning library
 *            should use this to store the pointer that malloc returned,
 *            along with the module name and line of code that
 *            backtrace() returned.  Return 1 if the pointer was
 *            successfully added, and 0 if the pointer was already in the
 *            map.
 */

int map_insert(uintptr_t pointer, char *module, char *line) {

  struct map_node *next;
  struct map_node *temp;  
    
  //do we even need these mallocs??? yes, yes we do
  next  =  malloc(sizeof(struct map_node)); 
  temp  =  malloc(sizeof(struct map_node));
  
  next -> program_counter = malloc(sizeof(char *));
  strcpy(next -> program_counter,line);
  
 
  next -> call_site = malloc(sizeof(char *));
  strcpy(next -> call_site, module);
 
  next -> allocated_pointer = pointer;

  next -> nextHead = malloc(sizeof(struct map_node));
  next -> nextHead = NULL;
  

  if(alloc_info == NULL){
    alloc_info = malloc(sizeof(struct map_node));
   
    alloc_info -> call_site = malloc(sizeof(char *));
    strcpy( alloc_info  -> call_site,module);
    
    alloc_info -> program_counter = malloc(sizeof(char *)); 
    strcpy(alloc_info  -> program_counter , line);
   
    
    alloc_info  -> allocated_pointer = pointer;
    
    alloc_info -> nextHead = malloc(sizeof(struct map_node));
    alloc_info  -> nextHead = NULL;
   
     return 1;
  }
  
  
  else{
   
    if(alloc_info -> nextHead == NULL)
      {
	if(alloc_info -> allocated_pointer == pointer)
	  return 0;
	
	alloc_info -> nextHead = next;
	next -> nextHead = NULL;
	return 1;
      }
    temp -> allocated_pointer = alloc_info -> allocated_pointer;
    temp -> call_site = malloc(sizeof(char *));
    strcpy(temp->call_site,alloc_info->call_site);
    temp-> program_counter = malloc(sizeof(char *));
    strcpy(temp->program_counter,alloc_info->program_counter);
    temp -> nextHead = malloc(sizeof(struct map_node));
    temp -> nextHead = alloc_info -> nextHead;
      
    
    while(temp -> nextHead != NULL)
      {   
	if(temp -> allocated_pointer == pointer)
	  {

	    return 0;
	  }
	else
	  temp = temp -> nextHead;
      }
    
    //if youve made it this far in the code, then the allocated_pointer is new and the node should be added
    temp -> nextHead = next;
    next -> nextHead = NULL;
    return 1;
  }
}

/*
 * remove() - when free() is called, your interpositioning library should use
 *            this to remove the pointer and its strings from the list
 *            declared above.  In this way, you'll be able to track, at all
 *            times, which allocations are outstanding, and what line of code
 *            caused those allocations.  Return 1 if the remove was
 *            successful, and 0 if the pointer was already removed from the
 *            map (which would suggest a double-free).
 */

int map_remove(uintptr_t pointer) {

  struct map_node *n = alloc_info;
  struct map_node *prev = NULL;

  while(n)
    {
      struct map_node *next = n->nextHead;

      if(n->allocated_pointer == pointer)
	{
	  free(n);
	  if(prev)
	    {
	      prev->nextHead = next;
	      return 1;
	    }
	  else
	    {
	      alloc_info = next;
	      return 1;
	    }
	}
      else
	{
	  prev = n;
	}
      n = next;
    }
  return 0;
}
 

/*
 * count() - return the number of elements in the map.  This can indicate
 *           that there are un-freed allocations (memory leaks).
 */


int map_count() {
   struct map_node *next;
   int count =0;
   next = alloc_info;
   while(next != NULL){
     next = next ->nextHead;
     count++;
   }
   free(next);
   next = NULL;
   return count;
}

/*
 * dump() - output the contents of the list
 */

__attribute__((destructor)) 
void map_dump() {
  /* TODO: complete this code */
  map_node_t* curr = alloc_info;
   while (curr) {
    printf("  0x%x allocated by %s::%s\n", curr -> allocated_pointer, curr -> call_site, curr -> program_counter);
    curr = curr -> nextHead;
    }
 fprintf(stdout,"map_count: %i\n", map_count());
}
