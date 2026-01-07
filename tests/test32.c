// gcc -m32 test32.c -o test32
#include <stdio.h>

int global_var = 42;
static int static_var = 10;

void my_function(void)
{
	printf("Hello from 32-bit!\n");
}

int main(void)
{
	my_function();
	return 0;
}
