#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* to use compare */
#include <time.h> /* gettimeofday */
#include <sys/time.h> /* gettimeofday */
#include "nCr.h" /* assembly code */

int main(int argc, const char * argv[])
{
	int i = 0;
	int n, c;
	struct timeval START;
	struct timeval END;
	const char * a = argv[1];
	
	if (strcmp(a,"-h") == 0) {
		printf("How to use: ./formula <positive integer>\n");
		return 0;
	} else {
		n = atoi(argv[1]);
		gettimeofday(&START, NULL);
		printf("(1 + x)^%i = 1 + ", n);
		if (n == 0)
			printf("0");
		while(i <= n) {
			c = nCr(n, i);
			if (c == -1) {
				printf("Error: overflow!\n");
				return 0;
			}
			if (i != 0){
				printf("%i*x^%i ",c ,i);
			}
			if (i != n && i != 0){
				printf("+ ");
			} 
			i++;
		}
		gettimeofday(&END, NULL);
	}
	printf("\n%ld microsecond(s)\n", ((END.tv_sec * 1000000 + END.tv_usec) - (START.tv_sec * 1000000 + START.tv_usec)));
	return 0;
}