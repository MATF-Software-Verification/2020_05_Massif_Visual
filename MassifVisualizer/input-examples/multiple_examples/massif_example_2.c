#include <stdlib.h>

void g(void)
{
  malloc(4000);
}

void f(void)
{
  malloc(2000);
  g();
}

int main(void)
{
  int i;
  int* a[10];

	f();
  	g();

  for (i = 0; i < 10; i++) {
    a[i] = malloc(1000);
  }

f();
  	g();
f();
  	g();
f();
  	g();
f();
  	g();
f();
  	g();

  
  for (i = 0; i < 10; i++) {
    free(a[i]);
  }

	


  return 0;
}
