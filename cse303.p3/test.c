#include <stdio.h>
#include <stdlib.h>


int main(){
  
  int *ptr = malloc(sizeof(int));
  int *ptr2 = malloc(sizeof(int));
  int *ptr3 = malloc(sizeof(int));

  free(ptr2);
  free(ptr3);
  return 0;
}
