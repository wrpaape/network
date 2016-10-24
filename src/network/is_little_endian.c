#include <stdio.h>

int
main(void)
{
	int x = 1;

	printf("%d", (int) (*((char *) &x)));

	return 0;
}
