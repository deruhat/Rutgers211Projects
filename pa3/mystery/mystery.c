#include <stdio.h>
#include <stdlib.h>

/* SEE mystery_comments.s FOR FULL COMMENTARY EXPLAINING THE ALGORITHM IN THE ASSEMBLY CODE */

int num[200];
int add(int a, int b) {
	return a + b;
}
int dothething(int a) {
	if (num[a] == -1) {
		if (a == 0){
			num[a] = 0;
		}
		else if (a == 1){
			num[a] = 1;
		}
		else{
			num[a] = dothething(a-2) + dothething(a-1);
		}
	}
	return num[a];
}
int main(int argc, char** argv) {
	int i = 0;
	int x = atoi(argv[1]);
	while (i < 200) {
		num[i] = -1;
		i++;
	}
	printf("%i\n", dothething( x ));
	return 0;
}