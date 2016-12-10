#include <stdio.h> /* standard i/o */
#include <stdlib.h> /* */
#include <string.h> /* */
#include <math.h> /* to use pow */

#include "format.h" /* personal header file with declarations inside */

/*
	changes the zero bits into one bits and vice versa:
*/

char * bitflip(char * flip) {
	int i = 0;
	while(i < (int) strlen(flip)) {
		if (flip[i] == '1')
			flip[i] = '0';
		else if(flip[i] == '0')
			flip[i] = '1';
		i++;
	}
	return flip;
}


void reverse(char *string)
{
	int length;
	int x;
	char *start; 
	char *stop;
	char temp;
    
	length = (int)strlen(string);
    
	x = 0;
	
	start = string;
	stop = string;
    
	while(x < ( length - 1 )){
		stop++;
		x++;
	}
    
	x = 0;
	
	while(x < length/2)
	{
		temp = *stop;
		*stop = *start;
		*start = temp;
		start++;
		stop--;
		x++;
	}
}

char * add(char * bin1, char * bin2) {
	int i;
	int toBeAdded;
    
	char * binary_result = malloc(strlen(bin2) + 1 + 1);
	binary_result[strlen(bin2)] = '\0';
	i = (int)strlen(bin2) - 1;
	toBeAdded = 0;
	memset(binary_result,0,strlen(bin2)+1+1);

	while (i >= 0) {
		if (toBeAdded != 1) {
			if (bin1[i] == '0' && bin2[i] == '0') {
				binary_result = strcat(binary_result, "0");
				toBeAdded = 0;
			} else if ((bin1[i] == '0' && bin2[i] == '1') || (bin1[i] == '1' && bin2[i] == '0')) {
				binary_result = strcat(binary_result, "1");
				toBeAdded = 0;
			} else if (bin1[i] == '1' && bin2[i] == '1') {
				binary_result = strcat(binary_result, "0");
				toBeAdded = 1;
			}
		} else {
			if (bin1[i] == '0' && bin2[i] == '0') {
				binary_result = strcat(binary_result, "1");
				toBeAdded = 0;
			} else if ((bin1[i] == '0' && bin2[i] == '1') || (bin1[i] == '1' && bin2[i] == '0')) {
				binary_result = strcat(binary_result, "0");
				toBeAdded = 1;
			} else if (bin1[i] == '1' && bin2[i] == '1') {
				binary_result = strcat(binary_result, "1");
				toBeAdded = 1;
			}
		}
		i--;
	}
    
	if (toBeAdded == 1)
		binary_result = strcat(binary_result, "1");
    
	reverse(binary_result);
    
	return binary_result;
}

int bin_to_dec (char * bin, int flip) {
	int i = 0;
	int power = (int)strlen(bin) - 1;
	int value = 0;
	int flipped = 0;
    
	if (bin[0] == '1' && flip) {
		bitflip(bin);
		flipped = 1;
	}
    
	for (i = 0; i < (int)strlen(bin); i++) {
		
		if (bin[i] == '1') {
			value += (int) pow(2, power);
		}
		
		power--;
	}
	
	if (flipped){
		value++;
	}
	
	return value;
}

double after_point(char * bin) {
	int i = 0;
	int power = -1;
	double value = 0;
    
	for (i = 0; i < (int)strlen(bin); i++) {
		
		if (bin[i] == '1') {
			value += pow(2, power);
		}
		
		power--;
	}
	
	return value;
}

void floatx(char * floatstr) {
	int i = 0;
	int flipped = 0;
	char * exponent_str = malloc(9);
	char * fraction = malloc(24);
	int sign = 0;
	double initial_exp = 0;
	double exponent_value = 0;
	double fraction_double = 0;
	double value = 0;
	int power = 0;

	reverse(floatstr);
	
	memset(exponent_str, 0, 8);
	exponent_str[8] = '\0';
	
	memset(fraction, 0, 23);
	fraction[23] = '\0';
	
	while(i < 32) {
		if (i == 0) {
			if (floatstr[i] == '1')
				flipped = 1;
		} else if (i >= 1 && i <= 8) {
			exponent_str[i - 1] = floatstr[i];
		} else {
			fraction[i-9] = floatstr[i];
		}
		i++;
	}
	
	if(floatstr[0] == '0'){
		sign = 1;
	}else{
		sign = -1;
	}
	
	initial_exp = bin_to_dec(exponent_str, 0) - 127;
		
	exponent_value = pow(2, initial_exp);
	
	fraction_double = 1 + after_point(fraction);
	
	value = sign * exponent_value * fraction_double;
	
	if (value < 1) {
		while (value < 1) {
			printf("here1.\n");
			value = value * 10;
			power--;
		}
	} else if (value > 9) {
		while (value > 9) {
			printf("here2.\n");
			value = value / 10;
			power++;
		}
	}
	
	printf("%fe%d", value, power);
}



int main (int argc, char *argv[]) {
	char * tmp = malloc(strlen(argv[2] + 1));
	tmp[(int)strlen(argv[2])] = '\0';
	if (strcmp(argv[2], "int") == 0) {
		printf("%d\n", bin_to_dec(argv[1], 1));
	} else if (strcmp(argv[2], "float") == 0) {
		
		if (strcasecmp(argv[1], "10000000000000000000000000000000") == 0) {
			tmp = "-0.0e0";
			printf("%s\n", tmp);
		} else {
			tmp = strcpy(tmp, argv[1]);
			reverse(tmp);
			floatx(tmp);
		}
	} else {
		printf("ERROR: second argument not valid.");
	}
	
	return 0;
}