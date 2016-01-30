#include <stdio.h>

int main(int argc, char *argv[])
{
	char buf[80];
	char buf1[80];
	if (argv[1]) {
		printf("%s\n", engfmt(buf, sizeof(buf), argv[1]));
		printf("%s\n", insert_commas(buf1, buf, ','));
	}
	return 0;
}
