#include <stdio.h> /* standard i/o */
#include <stdlib.h> /* to use atoi */
#include <string.h> /* to use string commands */
#include <math.h> /* to use pow */
#include "calc.h" /* personal header file with declarations inside */

/*
	Reverses the string in the argument and returns it:
*/

char *strrev(char *str){
	char *p1, *p2;
	if (! str || ! *str)
		return str;
	for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2){
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	return str;
}

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

/*
	ADDING AND SUBTRACTING OPERATIONS ARE DONE BELOW (returns final value):
*/

char * binary_operation(char * bin1, char * bin2) {
	int i;
	int toBeAdded;
	char * binary_result = malloc(strlen(bin2) + 2);
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
    
	return strrev(binary_result);
}

/*
	MULTIPLICATION OPERATIONS ARE DONE BELOW:
*/

/* char * binary_multiplication(char * bin1, char * bin2) {


} */

/*
	The function below pads a binary value (adds suitable zeroes to match the other binary value's n places) anreturns the result:
*/

char * padding(char * bin, int n) {
	int length = (int)strlen(bin);
	int difference = n - length;
	char * result = malloc(n + 1);
	int i = 0;
	int k = 0;
	memset(result, 0, n + 1);
	
	if (n == 0) {
		return bin;
	} else {
		while(i <= n){
			if (i < difference) {
				result[i] = '0';
			} else {
				result[i] = bin[k];
				k++;
			}
			i++;
        }
	}
	return result;
}

/*
	checks if the first argument in the function is bigger than the second argument and returns truth value:
*/

int bin1_is_bigger(char * bin1, char * bin2) {
	int i = (int) strlen(bin1) - 1;
	if (strlen(bin1) > strlen(bin2)) {
		return 1;
	} else if (strlen(bin2) > strlen(bin1)) {
		return 0;
	} else {
		while(i >= 0) {
			if (bin1[i] == '1' && bin2[i] == '0') {
				return 1;
			} else if (bin1[i] == '0' && bin2[i] == '1') {
				return 0;
			}
			i--;
		}
	}
	return 0;
}

/*
	Converts an octal string to a binary string:
*/

char * oct_to_bin (char * oct) {
	char * bin = malloc(strlen(oct) * 4);
	int i = 0;
	memset(bin,0,strlen(oct)*4);
	while(i < strlen(oct)) {
		bin = strcat(bin, one_oct_to_bin(oct[i]));
		i++;
	}
	return bin;
}

/*
	Converts a hexidecimal string to a binary string:
*/

char * hex_to_bin (char * hex) {
	char * bin = malloc(strlen(hex) * 5);
	int i = 0;
	bin[strlen(hex)*4] = '\0';
	while(i < strlen(hex)) {
		bin = strcat(bin, one_hex_to_bin(hex[i]));
		i++;
	}
	return bin;
}

/*
	Converts a decimal string to a binary string:
*/

char * dec_to_bin (char * dec) {
	int i;
	int k;
	int j = 0;
	int n = atoi(dec);
	char * bin = malloc(sizeof(int)+1);
	int zeroes = 1;
	bin[32] = '\0';
	i = 31;
	while(i >= 0) {
		k = n >> i;
		if (k & 1) {
			bin[j] = '1';
			j++;
			zeroes = 0;
		} else if (zeroes == 0) {
			bin[j] = '0';
			j++;
		}
		i--;
	}
	bin[j] = '\0';
	return bin;
}

/*
	Converts a binary string to a decimal string and prints it:
*/

void bin_to_dec (char * bin) {
	int i = 0;
	long v = 0;
	int power = (int)strlen(bin) - 1;
	while(i < (int)strlen(bin)) {
		if (bin[i] == '1') {
			v += pow(2, power);
		}
        i++;
		power--;
	}
	printf("%ld\n", v);
}

/*
	Converts a binary string to a single octal char:
*/

char one_bin_to_oct (char * bin) {
	if (strcmp(bin, "000") == 0) {
		return '0';
	} else if (strcmp(bin, "001") == 0) {
		return '1';
	} else if (strcmp(bin, "010") == 0) {
		return '2';
	} else if (strcmp(bin, "011") == 0) {
		return '3';
	} else if (strcmp(bin, "100") == 0) {
		return '4';
	} else if (strcmp(bin, "101") == 0) {
		return '5';
	} else if (strcmp(bin, "110") == 0) {
		return '6';
	} else {
		return '7';
	}
}

/*
	Converts a binary string to a single hexidecimal char:
*/

char one_bin_to_hex (char * bin) {
	if (strcmp(bin, "0000") == 0) {
		return '0';
	} else if (strcmp(bin, "0001") == 0) {
		return '1';
	} else if (strcmp(bin, "0010") == 0) {
		return '2';
	} else if (strcmp(bin, "0011") == 0) {
		return '3';
	} else if (strcmp(bin, "0100") == 0) {
		return '4';
	} else if (strcmp(bin, "0101") == 0) {
		return '5';
	} else if (strcmp(bin, "0110") == 0) {
		return '6';
	} else if (strcmp(bin, "0111") == 0) {
		return '7';
	} else if (strcmp(bin, "1000") == 0) {
		return '8';
	} else if (strcmp(bin, "1001") == 0) {
		return '9';
	} else if (strcmp(bin, "1010") == 0) {
		return 'A';
	} else if (strcmp(bin, "1011") == 0) {
		return 'B';
	} else if (strcmp(bin, "1100") == 0) {
		return 'C';
	} else if (strcmp(bin, "1101") == 0) {
		return 'D';
	} else if (strcmp(bin, "1110") == 0) {
		return 'E';
	} else if (strcmp(bin, "1111") == 0){
		return 'F';
	} else {
		fprintf(stderr, "%s", "error: non-hex digit");
		return 'X';
	}
}

/*
	Converts a single octal char to a binary string:
*/

char * one_oct_to_bin (char oct) {
	if(oct == '0')
		return "000";
	else if(oct == '1')
		return "001";
	else if(oct == '2')
		return "010";
	else if(oct == '3')
		return "011";
	else if(oct == '4')
		return "100";
	else if(oct == '5')
		return "101";
	else if(oct == '6')
		return "110";
	else if(oct == '7')
		return "111";
	else {
		fprintf(stderr, "%s", "error: non-digit character");
		return "";
	}
}

/*
	Converts a single hexidecimal char to a binary string:
*/

char * one_hex_to_bin (char hex) {
	if(hex == '0')
		return "0000";
	else if(hex == '1')
		return "0001";
	else if(hex == '2')
		return "0010";
	else if(hex == '3')
		return "0011";
	else if(hex == '4')
		return "0100";
	else if(hex == '5')
		return "0101";
	else if(hex == '6')
		return "0110";
	else if(hex == '7')
		return "0111";
	else if(hex == '8')
		return "1000";
	else if(hex == '9')
		return "1001";
	else if(hex == 'A' || hex == 'a')
		return "1010";
	else if(hex == 'B' || hex == 'b')
		return "1011";
	else if(hex == 'C' || hex == 'c')
		return "1100";
	else if(hex == 'D' || hex == 'd')
		return "1101";
	else if(hex == 'E' || hex == 'e')
		return "1110";
	else if(hex == 'F' || hex == 'f')
		return "1111";
	else{
		fprintf(stderr, "%s", "error: non-digit character");
		return "";
	}
}

/*
	Adjusts a binary string to have an even base:
*/

char * base_adjust (char * bin, int size_of_digit) {
	int distance = 0;
	int i = 0;
	int j = 4;
	int k = 0;
	int number_of_digits = 0;
	char * res;
	char* substr;
	int x = 0;
    
	while(((strlen(bin) + distance) % size_of_digit) != 0) {
		distance++;
	}
    
	res = malloc((strlen(bin) + distance) / size_of_digit+1);
	res[(strlen(bin)+distance)/size_of_digit] = '\0';
    
	x = (int)strlen(bin) + distance;
	
	bin = padding(bin, x);
	number_of_digits = (int)strlen(bin)/size_of_digit;
 
    
	while (k < number_of_digits) {
		substr = strndup(bin+i, j);
		substr[size_of_digit] = '\0';
        
		i = i + size_of_digit;
		j = j + size_of_digit;
        
		if (size_of_digit == 4) {
			res[k] = one_bin_to_hex(substr);
		} else if (size_of_digit == 3) {
			res[k] = one_bin_to_oct(substr);
		}
		free(substr);
		k++;
	}
	free(bin);
	res[k] = '\0';
	return res;
}

void final_conversion(char* output, char output_base, int output_is_negative){
	/* first, check if it's an octal or a hex and convert them using the base_adjust function */
	/* Also, if output base is d, check if negative and print accordingly */
	
	if(output_base == 'x'){
		output = base_adjust(output, 4);
		if (output_is_negative == 1)
			printf("%c", '-');
        
		printf("%c", output_base);
        
		printf("%s\n", output);
	}else if(output_base == 'o'){
		output = base_adjust(output, 3);
		if (output_is_negative == 1)
			printf("%c", '-');
        
		printf("%c", output_base);
        
		printf("%s\n", output);
	}else if(output_base == 'd'){
		if (output_is_negative == 1)
			printf("%c", '-');
		printf("%c", output_base);
		bin_to_dec(output);
	}else if(output_base == 'b'){
		if (output_is_negative == 1)
			printf("%c", '-');
        
		printf("%c", output_base);
        
		printf("%s\n", output);
	}else{
		fprintf(stderr, "%s", "error: base invalid.\n");
	}
}

int main(int argc, char *argv[]){
    /* first number data */
   	char * n1;
	int n1_is_bigger = 0;
	int n1_is_negative = 0;
	char n1_base = 'x';
	int n1_space = 0;
    /* second number data */
	char * n2;
	int n2_is_bigger = 0;
	int n2_is_negative = 0;
	char n2_base = 'x';
	int n2_space = 0; 
	/* result to be returned */
	char * output;
	int output_is_negative;
	char output_base;
	char output_operation = ' ';
    /* counters */
	int i,j,k;
	i=0; j=0; k=0;
    
	/* if too few arguments */
	if (argc < 5) {
		fprintf(stderr, "%s", "error:too few arguments.\n");
		return 1;
	}

	/* processing the arguments to find the base, values and operations */
	while(i < argc) {
		char const *arg =  argv[i];
		if(i == 0){
			i++;
			continue;
		}else if(i == 1){
			output_operation = arg[0];
		}else if(i == 2){
			if(arg[0] == '-'){
					n1_is_negative = 1;
				}
				else {
					n1_is_negative = 0;
				}
				if(n1_is_negative == 1){
					n1_base = arg[1];
				}
				else {
					n1_base = arg[0];
				}
				if(n1_is_negative == 1){
					n1_space = (int)(strlen(arg) - 2);
				}else{
					n1_space = (int)(strlen(arg) - 1);
				}
				if(n1_is_negative == 1){
					k = 2;
				}else{
					k = 1;
				}
				n1 = malloc(n1_space+1);
				n1[n1_space] = '\0';
				j = 0;
				while(k < strlen(arg)) {
					char ptr = arg[k];
					n1[j] = ptr;
					j++;
					k++;
				}
				n1[j] = '\0';
		}else if(i == 3){
			if(arg[0] == '-'){
					n2_is_negative = 1;
				}
				else {
					n2_is_negative = 0;
				}
				if(n2_is_negative == 1){
					n2_base = arg[1];
				}
				else {
					n2_base = arg[0];
				}
				if(n1_is_negative == 1){
					n2_space = (int)(strlen(arg) - 2);
				}else{
					n2_space = (int)(strlen(arg) - 1);
				}
				if(n2_is_negative == 1){
					k = 2;
				}else{
					k = 1;
				}
				n2 = malloc(n2_space+1);
				n2[n1_space] = '\0';
				j = 0;
				while(k < strlen(arg)) {
					char ptr = arg[k];
					n2[j] = ptr;
					j++;
					k++;
				}
				n2[j] = '\0';
		}else if(i == 4){
			output_base = arg[0];
		}else {
			fprintf(stderr, "%s", "error: too many arguments");
			return 0;
		}
		i++;
	}
    
	/* begin processing */	
	if(n1_base == 'b'){
		/* already binary */
	}else if(n1_base == 'o'){
		/* octal */
		n1 = oct_to_bin(n1);
	}else if(n1_base == 'd'){
		/* decimal */
		n1 = dec_to_bin(n1);
	}else if(n1_base == 'x'){
		/* hexidecimal */
		n1 = hex_to_bin(n1);
	}else{
		/* not valid base */
		fprintf(stderr, "%s", "error: base invalid.\n");
		return 0;
	}
	if(n2_base == 'b'){
		/* already binary */
	}else if(n2_base == 'o'){
		/* octal */
		n2 = oct_to_bin(n2);
	}else if(n2_base == 'd'){
		/* decimal */
		n2 = dec_to_bin(n2);
	}else if(n2_base == 'x'){
		/* hexidecimal */
		n2 = hex_to_bin(n2);
	}else{
		/* not valid base */
		fprintf(stderr, "%s","error: base invalid.\n");
		return 0;
	}
	
	/* prepare for padding (step after this) */
    n1_is_bigger = bin1_is_bigger(n1, n2);
	n2_is_bigger = bin1_is_bigger(n2, n1);
    
	
	/* This is for adding zeroes and making the two binary values have the same length (padding): */
	if (strlen(n1) > strlen(n2)) {
		n2 = padding(n2, (int)strlen(n1));
	} else if (strlen(n2) > strlen(n1)) {
		n1 = padding(n1, (int)strlen(n2));
	}
    
	/* ADDING OPERATION */
	if (output_operation == '+') {
        if (n1_is_negative == 1 && n2_is_negative == 1) {
            output = binary_operation(n1, n2);
            output_is_negative = 1;
        } else if (n1_is_negative != 1 && n2_is_negative != 1) {
			output = binary_operation(n1, n2);
			output_is_negative = 0;
		} else if (n1_is_negative == 1) {
            if (n1_is_bigger != 1 && !n2_is_bigger != 1) {
				output = strdup("0");
				output_is_negative = 0;
			} else if (n2_is_bigger == 1) {
                n1 = bitflip(n1);
				n1 = binary_operation(padding("1", (int)strlen(n1)), n1);
				output = binary_operation(n1, n2);
				output[0] = '0';
				output_is_negative = 0;
            } else if (n1_is_bigger == 1) {
                n2 = bitflip(n2);
                n2 = binary_operation(padding("1", (int)strlen(n2)), n2);
                output = binary_operation(n1, n2);
                output[0] = '0';
                output_is_negative = 1;
            }
		} else if (n2_is_negative == 1) {
			if (n1_is_bigger != 1 && n2_is_bigger != 1) {
				output = strdup("0");
				output_is_negative = 0;
			} else if (n1_is_bigger == 1) {
				n2 = bitflip(n2);
                n2 = binary_operation(padding("1", (int)strlen(n2)), n2);
                output = binary_operation(n1, n2);
                output[0] = '0';
                output_is_negative = 0;
			} else if (n2_is_bigger == 1) {
				n1 = bitflip(n1);
				n1 = binary_operation(padding("1", (int)strlen(n1)), n1);
				output = binary_operation(n2, n1);
				output[0] = '0';
				output_is_negative = 1;
			}
		} 
	} 
	
	/* SUBTRACTING OPERATION */
	else if (output_operation == '-') {
        if (n1_is_negative == 1 && n2_is_negative == 1) {
            if (n1_is_bigger != 1 && n2_is_bigger != 1) {
                output = strdup("0");
                output_is_negative = 0;
            } else if (n1_is_bigger == 1) {
                n2 = bitflip(n2);
                n2 = binary_operation(padding("1", (int)strlen(n2)), n2);
                output = binary_operation(n1, n2);
                output[0] = '0';
                output_is_negative = 1;
            } else if (n2_is_bigger == 1) {
                n1 = bitflip(n1);
				n1 = binary_operation(padding("1", (int)strlen(n1)), n1);
				output = binary_operation(n2, n1);
				output[0] = '0';
				output_is_negative = 0;
            }
        } else if (n1_is_negative != 1 && n2_is_negative != 1) {
			if (n1_is_bigger != 1 && n2_is_bigger != 1) {
				output = strdup("0");
				output_is_negative = 0;
			} else if (n1_is_bigger == 1) {
				n2 = bitflip(n2);
				n2 = binary_operation(padding("1", (int)strlen(n2)), n2);
				output = binary_operation(n1, n2);
				output[0] = '0';
				output_is_negative = 0;
			} else if (n2_is_bigger == 1) {
				n1 = bitflip(n1);
				n1 = binary_operation(padding("1", (int)strlen(n1)), n1);
				output = binary_operation(n2, n1);
				output[0] = '0';
				output_is_negative = 1;
			}
		} else if (n1_is_negative == 1) {
			output = binary_operation(n1, n2);
			output_is_negative = 1;
		} else if (n2_is_negative == 1) {
			output = binary_operation(n1, n2);
			output_is_negative = 0;
		}
	}
	
	/* MULTIPLICATION OPERATION */
	/* else if (output_operation == '*') {
		if (n1_is_negative == 1 && n2_is_negative == 1) {
            output = n1 * n2;
            output_is_negative = 0;
        } else if (n1_is_negative != 1 && n2_is_negative != 1) {
			output = n1 * n2;
			output_is_negative = 0;
		} else if (n1_is_negative == 1 || n2_is_negative == 1) {
			output = n1 * n2;
			output_is_negative = 1;
		}
	} */
    
	/* CONVERSION TO OUTPUT BASE */
	final_conversion(output, output_base, output_is_negative);
	return 0;
}