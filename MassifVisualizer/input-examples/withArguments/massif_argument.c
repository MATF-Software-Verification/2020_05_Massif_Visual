#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{

  if (argc < 2) {
    return 0;
  }

  int n = atoi(argv[1]);
  int i;
  int* a[n];
  
  for (i = 0; i < n; i++) {
    a[i] = malloc(1000);
  }

  
  for (i = 0; i < n; i++) {
    free(a[i]);
  }

  




  return 0;
}
