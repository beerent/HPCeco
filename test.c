#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  int test[4] = {1,2,3,4};
  char test2[6] = "BR";
  printf("%s\n", test2);
  int i;
  for(i = 0; i < 4; i++){
    sprintf(test2, "%s%d", test2,test[i]);
    printf("%s\n", test2);
  }
}
