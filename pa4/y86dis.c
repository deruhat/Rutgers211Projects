#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "y86dis.h"
#include "stringmanip.h"

#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7

/*

PLEASE READ THE README FILE!

*/

 /*
	HEX CONVERSION STUFF:
 */
int one_hex_to_dec (char hex) {
	if(hex == '0')
		return 0;
	else if(hex == '1')
		return 1;
	else if(hex == '2')
		return 2;
	else if(hex == '3')
		return 3;
	else if(hex == '4')
		return 4;
	else if(hex == '5')
		return 5;
	else if(hex == '6')
		return 6;
	else if(hex == '7')
		return 7;
	else if(hex == '8')
		return 8;
	else if(hex == '9')
		return 9;
	else if(hex == 'A' || hex == 'a')
		return 10;
	else if(hex == 'B' || hex == 'b')
		return 11;
	else if(hex == 'C' || hex == 'c')
		return 12;
	else if(hex == 'D' || hex == 'd')
		return 13;
	else if(hex == 'E' || hex == 'e')
		return 14;
	else if(hex == 'F' || hex == 'f')
		return 15;
	else{
		fprintf(stderr, "%s", "ERROR: non-digit character.\n");
		return 0;
	}
}

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
		fprintf(stderr, "%s", "ERROR: non-digit character.\n");
		return "";
	}
}

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

int bin_to_dec (char * bin) {
	int i = 0;
	long v = 0;
	int power = (int)strlen(bin) - 1;
	while(i < (int)strlen(bin)) {
		if (bin[i] == '1') {
			v += (int)pow(2, power);
		}
        i++;
		power--;
	}
	return v;
}

int hex_to_dec(char * hex) { 
	char * bin = hex_to_bin(hex);
	return bin_to_dec(bin);
}

/*
	FETCHING THE DATA FROM THE FILE:
*/
char * fetch(const char * filename) {
	char * memory = 0;
	long length;
	FILE * f = fopen (filename, "rb");
	if (f) {
		fseek (f, 0, SEEK_END);
		length = ftell (f);
		fseek (f, 0, SEEK_SET);
		memory = malloc (length);
		if (memory) {
			fread (memory, 1, length, f);
		}
		fclose (f);
	} 
	else {
		printf("ERROR: Does not exist.\n");
	}
	
	if (memory) {
		return memory;
	} 
	else {
		printf("ERROR: Empty file.\n");
		return NULL;
	}
}

/*
	DECODING EVERY LINE AND ALLOCATING MEMORY:
*/
void decode(char * code) {
	char * duplicate = strdup(code);
	char * tokenizer = strtok(duplicate, "\t\n");
	int size = 0; 
	int i; 
	int j; 
	char * dir; 
	char * address;
	char * argument; 
	int long_argument;
	char * sub;
	processor * processor; 
	unsigned char * mem;
	int noOfTokens = 0;
	int textCount = 0;
	size = dirSize(tokenizer, size); /* HELPER METHOD */
	processor = malloc(sizeof *processor);
	processor->eip = -1;
	processor->size = size;
	mem = malloc((size + 1) * sizeof(unsigned char));
	sub = malloc(sizeof(size + 1));
	i = 0;
	while(i < size) {
		mem[i] = 0;
		i++;
	}
	i = 0;
	while(i < 9) {
		processor->registers[i] = 0;
		i++;
	}
	duplicate = strdup(code);
	tokenizer = strtok(duplicate, "\n\t");
	i = 0;
	for(noOfTokens = 0; tokenizer; noOfTokens++) {
		if (strcmp(tokenizer, ".size") == 0) {
			tokenizer = strtok(NULL, "\n\t"); 
			tokenizer = strtok(NULL, "\n\t"); 
		} 
		else if (strcmp(tokenizer, ".string") == 0) { 
			dir = strdup(tokenizer); 
			tokenizer = strtok(NULL, "\n\t"); 
			address = strdup(tokenizer); 
			tokenizer = strtok(NULL, "\n\t"); 
			argument = strdup(tokenizer); 
			tokenizer = strtok(NULL, "\n\t"); 
			i = hex_to_dec(address);
			j = 1;
			while (j <= strlen(argument)) {
				mem[i] = (unsigned char) argument[j];
				i++;
				j++;
			}
		} 
		else if (strcmp(tokenizer, ".long") == 0) {
			dir = strdup(tokenizer); 
			tokenizer = strtok(NULL, "\n\t"); 
			address = strdup(tokenizer); 
			tokenizer = strtok(NULL, "\n\t"); 
			argument = strdup(tokenizer); 
			tokenizer = strtok(NULL, "\n\t"); 
			long_argument = atoi(argument);
			j = hex_to_dec(address);
			*(unsigned int*)(mem + j) = long_argument;
			free(argument);
			free(dir);
			free(address);
			address = 0; 
		}
		else if (strcmp(tokenizer, ".byte") == 0) {
			dir = strdup(tokenizer); 
			tokenizer = strtok(NULL, "\n\t"); 
			address = strdup(tokenizer); 
			tokenizer = strtok(NULL, "\n\t"); 
			argument = strdup(tokenizer); 
			tokenizer = strtok(NULL, "\n\t"); 
			mem[hex_to_dec(address)] = (unsigned char)hex_to_dec(argument);
			free(argument);
			free(dir);
			free(address);
			address = 0; 
		}
		else if (strcmp(tokenizer, ".text") == 0) {
			dir = strdup(tokenizer); 
			tokenizer = strtok(NULL, "\n\t"); 
			address = strdup(tokenizer); 
			tokenizer = strtok(NULL, "\n\t"); 
			argument = strdup(tokenizer); 
			tokenizer = strtok(NULL, "\n\t"); 
			i = hex_to_dec(address);
			j = 0;
			if (processor->eip == -1) { 
				processor->eip = i; 
			} else if (processor->eip != -1) { /* more than 1 text directive */
				fprintf(stderr, "ERROR: Invalid directive.\n");
				return;
			}
			i = 0;
			while (j < strlen(argument)) {
				sub = substr(argument, sub, j, 2);
				mem[i] = (unsigned char) hex_to_dec(sub);
				i++;
				j = j + 2;
				textCount++;
			}
			free(argument);
			free(dir);
			free(address);
			address = 0; 
		}
		else {
			fprintf(stderr, "ERROR: Invalid instruction.\n"); 
			return;
		}
	}
	free(duplicate);
	free(tokenizer);
	execute(mem, textCount);
	return;
}

/*
	PRINT ASSEMBLY:
*/
void execute(unsigned char * mem, int textCount){
	unsigned char regA; /*rA*/
	unsigned char regB; /*rB*/
	char *reg1;
	char *reg2;
	int i = 0;
	int v = 0;
	/*
	EBP\0
	*/
	reg1 = malloc(4);
	reg2 = malloc(4); 

	for(; i < textCount;) {
		regA = (mem[i + 1] & 0xf0) >> 4;
		regB = (mem[i + 1] & 0x0f);
		if(regB == 0){
			reg2 = "eax";
		}
		else if(regB == 1){
			reg2 = "ecx";
		}
		else if(regB == 2){
			reg2 = "edx";
		}
		else if(regB == 3){
			reg2 = "ebx";
		}
		else if(regB == 4){
			reg2 = "esp";
		}
		else if(regB == 5){
			reg2 = "ebp\0";
		}
		else if(regB == 6){
			reg2 = "esi";
		}
		else if(regB == 7){
			reg2 = "edi";
		}
		if(regA == 0){
			reg1 = "eax";
		}
		else if(regA == 1){
			reg1 = "ecx";
		}
		else if(regA == 2){
			reg1 = "edx";
		}
		else if(regA == 3){
			reg1 = "ebx";
		}
		else if(regA == 4){
			reg1 = "esp";
		}
		else if(regA == 5){
			strcpy(reg1,"ebp");
		}
		else if(regA == 6){
			reg1 = "esi";
		}
		else if(regA == 7){
			reg1 = "edi";
		}
		if(mem[i] == 0x00){ /* nop: do nothing, go to next byte */
			printf("\tnop\n");
			i++;
		}
		else if(mem[i] == 0x10){ /* halt: stop the program */
			printf("\thlt\n");
			i++;
		}
		else if(mem[i] == 0x20){ /* rrmovl: register to register */
			printf("\trrmovl\t%s, %s\n", reg1, reg2);
			i = i + 2;
		}
		else if(mem[i] == 0x30){ /* irmovl: immediate to register */
			v = mem[i + 2] + (mem[i + 3] << 8) + (mem[i + 4] << 16) + (mem[i + 5] << 24);
			printf("\tirmovl\t%d, %s\n", v, reg2);
			i = i + 6;
		}
		else if(mem[i] == 0x40){ /* rmmovl: register to memory */
			v = mem[i + 2] + (mem[i + 3] << 8) + (mem[i + 4] << 16) + (mem[i + 5] << 24);
			printf("\trmmovl\t%s, %x\n", reg2, v);
			i = i + 6;
		}
		else if(mem[i] == 0x50){ /* mrmovl: memory to register */
			v = mem[i + 2] + (mem[i + 3] << 8) + (mem[i + 4] << 16) + (mem[i + 5] << 24);
			printf("\tmrmovl\t%x, %s\n", v, reg1);
			i = i + 6;
		}
		else if(mem[i] == 0x60){ /* addl: add two registers, save it in register B */
			printf("\taddl\t%s, %s\n", reg1, reg2);
			i = i + 2;
		}
		else if(mem[i] == 0x61){ /* subl: subtract two registers, save it in register B */
			printf("\tsubl\t%s, %s\n", reg1, reg2);
			i = i + 2;
		}
		else if(mem[i] == 0x62){ /* andl: AND operand */
			printf("\tandl\t%s, %s\n", reg1, reg2);
			i = i + 2;
		}
		else if(mem[i] == 0x63){ /* xorl: EXCLUSIVE OR operand */
			printf("\txorl\t%s, %s\n", reg1, reg2);
			i = i + 2;
		}
		else if(mem[i] == 0x64){ /* imull: multiply two registers, save value in register B */
			printf("\timull\t%s, %s\n", reg1, reg2);
			i = i + 2;
		}
		else if(mem[i] == 0x65){ /* cmpl: compares two registers logically (like subl but discards the result and sets the flags) */
			printf("\tcmpl\t%s, %s\n", reg1, reg2);
			i = i + 2;
		}
		else if(mem[i] == 0x70){ /* jmp: jump to destination with no condition */
			v = mem[i + 1] + (mem[i + 2] << 8) + (mem[i + 3] << 16) + (mem[i + 4] << 24);
			printf("\tjmp\t%x\n", v);
			i = i + 4;
		}
		else if(mem[i] == 0x71){ /* jle: jump to destination if less or equal */
			v = mem[i + 1] + (mem[i + 2] << 8) + (mem[i + 3] << 16) + (mem[i + 4] << 24);
			printf("\tjle\t%x\n", v);
			i = i + 5;
		}
		else if(mem[i] == 0x72){ /* jl: jump to destination if less */
			v = mem[i + 1] + (mem[i + 2] << 8) + (mem[i + 3] << 16) + (mem[i + 4] << 24);
			printf("\tjl\t%x\n", v);
			i = i + 5;
		}
		else if(mem[i] == 0x73){ /* je: jump to destination if equal */
			v = mem[i + 1] + (mem[i + 2] << 8) + (mem[i + 3] << 16) + (mem[i + 4] << 24);
			printf("\tje\t%x\n", v);
			i = i + 5;
		}
		else if(mem[i] == 0x74){ /* jne: jump to destination if not equal */
			v = mem[i + 1] + (mem[i + 2] << 8) + (mem[i + 3] << 16) + (mem[i + 4] << 24);
			printf("\tjne\t%x\n", v);
			i = i + 5;
		}
		else if(mem[i] == 0x75){ /* jge: jump to destination if greater than or equal */
			v = mem[i + 1] + (mem[i + 2] << 8) + (mem[i + 3] << 16) + (mem[i + 4] << 24);
			printf("\tjge\t%x\n", v);
			i = i + 5;
		}
		else if(mem[i] == 0x76){ /* jg: jump to destination if greater */
			v = mem[i + 1] + (mem[i + 2] << 8) + (mem[i + 3] << 16) + (mem[i + 4] << 24);
			printf("\tjg\t%x\n", v);
			i = i + 5;
		}
		else if(mem[i] == 0x80){ /* call: call a function and do the instructions there */
			v = mem[i + 1] + (mem[i + 2] << 8) + (mem[i + 3] << 16) + (mem[i + 4] << 24);
			printf("\tcall\t%x\n", v);
			i = i + 4;
		}
		else if(mem[i] == 0x90){ /* ret: return the value in this function and go to previous function */
			printf("\tret\n");
			i++;
		}
		else if(mem[i] == 0xa0){ /* pushl: push to the stack */
			printf("\tpush\t%s\n", reg1);
			i = i + 2;
		}
		else if(mem[i] == 0xb0){ /* popl: pop from the stack */
			printf("\tpop\t%s\n", reg1);
			i = i + 2;
		}
		else if(mem[i] == 0xc0){ /* readb: read a byte from the user */
			v = mem[i + 2] + (mem[i + 3] << 8) + (mem[i + 4] << 16) + (mem[i + 5] << 24);
			printf("\treadb\t%x\n", v);
			i = i + 6;
		}
		else if(mem[i] == 0xc1){ /* readl: read 4 bytes from the user */
			v = mem[i + 2] + (mem[i + 3] << 8) + (mem[i + 4] << 16) + (mem[i + 5] << 24);
			printf("\treadl\t%x\n", v);
			i = i + 6;
		}
		else if(mem[i] == 0xd0){ /* writeb: print a byte to the user */
			v = mem[i + 2] + (mem[i + 3] << 8) + (mem[i + 4] << 16) + (mem[i + 5] << 24);
			printf("\twriteb\t%x\n", v);
			i = i + 6;
		}
		else if(mem[i] == 0xd1){ /* writel: print 4 bytes to the user */
			v = mem[i + 2] + (mem[i + 3] << 8) + (mem[i + 4] << 16) + (mem[i + 5] << 24);
			printf("\twritel\t%x\n", v);
			i = i + 6;
		}
		else if(mem[i] == 0xe0){ /* movsbl: move to destination with sign extension from 1 byte to 4 bytes */
			v = mem[i + 2] + (mem[i + 3] << 8) + (mem[i + 4] << 16) + (mem[i + 5] << 24);
			printf("\tmovsbl\t%x, %s\n", v, reg1);
			i = i + 6;
		}
	} 
	
	return;
} 

int main(int argc, const char * argv[]) {
	decode(fetch(argv[1]));
	return 0;
}