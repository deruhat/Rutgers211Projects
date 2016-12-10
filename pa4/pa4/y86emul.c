#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "y86emul.h"
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
	char * dup = strdup(code);
	char * tokenizer = strtok(dup, "\t\n");
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
	dup = strdup(code);
	tokenizer = strtok(dup, "\n\t");
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
	
	
	
	
	printf("========================================================================\n");
	printf("* USEFUL INFORMATION ABOUT THE PROGRAM *\n");
	printf("  - After decoding the y86 file:\n");
	printf("     Total # of lines: %d\n", noOfTokens);
	printf("     Contents of memory (1st clmn is location in array, 2nd clmn is value or ID: \n");
	for (i = 0; i < textCount; i++) {
		printf("\t\t%i\t%x\n", i, mem[i]);
	}
	printf("========================================================================\n");
	printf("* EXECUTION OF THE PROGRAM (BELOW IS WHAT THE PROGRAM SHOULD LOOK LIKE): \n");
	printf("========================================================================\n");

	free(dup);
	free(tokenizer);
	execute(mem, processor);
	return;
}

/*
	EXECUTING THE INSTRUCTIONS AND MATHEMATICAL OPERATIONS WITH THE BYTES PROVIDED AND PROCESSOR:
*/
void execute(unsigned char * mem, processor * processor){
	flags * flags; /* value flags */
	status * status; /* status flags */
	int loops; /* number of instructions */
	unsigned char regA; /*rA*/
	unsigned char regB; /*rB*/
	int v; /* immediate value or result of calculation */
	unsigned int dst; /* destination for mov */
	char inputB; /* inout for 1 byte */
	int inputL; /* input for 4 bytes */
	processor->cont = 1; /* continue flag */
	/* FLAGS */
	flags = malloc(sizeof *flags);
	status = malloc(sizeof *status);
	status->AOK = 1;
	status->HLT = 0;
	status->ADR = 0;
	status->INS = 0;
	/* SHIFTING:
		Every F masking operation (one digit) in hex from the right is equivelent to right shifting 4 bits in binary.
		e.g. :
			0x0f (no shift needed)
			0xf0 ( >> 4 ): one digit from the right 
			0x000000ff (no shift needed)
			0x0000ff00 ( >> 8 ): two digits from the right
	*/
	for(loops = 0; processor->cont != 0; loops++) {
		if(mem[processor->eip] == 0x00){ /* nop: do nothing, go to next byte */
			processor->eip++;
		}
		else if(mem[processor->eip] == 0x10){ /* halt: stop the program */
			status->HLT = 1;
			processor->cont = 0;
			return;
		}
		else if(mem[processor->eip] == 0x20){ /* rrmovl: register to register */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			regB = (mem[processor->eip + 1] & 0x0f);
			processor->registers[regB] = processor->registers[regA];
			processor->eip = processor->eip + 2;
		}
		else if(mem[processor->eip] == 0x30){ /* irmovl: immediate to register */
			regB = (mem[processor->eip + 1] & 0x0f);
			v = mem[processor->eip + 2] + (mem[processor->eip + 3] << 8) + (mem[processor->eip + 4] << 16) + (mem[processor->eip + 5] << 24);
			processor->registers[regB] = v;
			processor->eip = processor->eip + 6;
		}
		else if(mem[processor->eip] == 0x40){ /* rmmovl: register to memory */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			regB = (mem[processor->eip + 1] & 0x0f);
			dst = mem[processor->eip + 2] + (mem[processor->eip + 3] << 8) + (mem[processor->eip + 4] << 16) + (mem[processor->eip + 5] << 24);
			mem[processor->registers[regB]] = (processor->registers[regA]);
			mem[processor->registers[regB] + 1] = (processor->registers[regA] & 0x0000ff00) >> 8;
			mem[processor->registers[regB] + 2] = (processor->registers[regA] & 0x00ff0000) >> 16;
			mem[processor->registers[regB] + 3] = (processor->registers[regA] & 0xff000000) >> 24;
			processor->eip = processor->eip + 6;
		}
		else if(mem[processor->eip] == 0x50){ /* mrmovl: memory to register */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			regB = (mem[processor->eip + 1] & 0x0f);
			dst = mem[processor->eip + 2] + (mem[processor->eip + 3] << 8) + (mem[processor->eip + 4] << 16) + (mem[processor->eip + 5] << 24);
			processor->registers[regA] = (mem[processor->registers[regB] + dst]) + (mem[processor->registers[regB] + dst + 1]<<8) + (mem[processor->registers[regB] + dst + 2]<<16) + (mem[processor->registers[regB] + dst + 3]<<24);
			processor->eip = processor->eip + 6;
		}
		else if(mem[processor->eip] == 0x60){ /* addl: add two registers, save it in register B */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			regB = (mem[processor->eip + 1] & 0x0f);
			flags->OF = 0;
			flags->SF = 0;
			flags->ZF = 0;
			v = processor->registers[regA] + processor->registers[regB];
			if ((processor->registers[regA] > 0 && processor->registers[regB] > 0 && v < 0) || (processor->registers[regA] < 0 && processor->registers[regB] < 0 && v > 0)) {
				flags->OF = 1;
			}
			if (v == 0) {
				flags->ZF = 0;
			}
			if (v < 0) {
				flags->SF = 1;
			}
			processor->registers[regB] = v;
			processor->eip = processor->eip + 2;
		}
		else if(mem[processor->eip] == 0x61){ /* subl: subtract two registers, save it in register B */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			regB = (mem[processor->eip + 1] & 0x0f);
			flags->OF = 0;
			flags->SF = 0;
			flags->ZF = 0;
			v = processor->registers[regB] - processor->registers[regA];
			if ((processor->registers[regA] > 0 && processor->registers[regB] > 0 && v < 0) || (processor->registers[regA] < 0 && processor->registers[regB] < 0 && v > 0)) {
				flags->OF = 1;
			}
			if (v == 0) {
				flags->ZF = 1;
			}
			if (v < 0) {
				flags->SF = 1;
			}
			processor->registers[regB] = v;
			processor->eip = processor->eip + 2;
		}
		else if(mem[processor->eip] == 0x62){ /* andl: AND operand */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			regB = (mem[processor->eip + 1] & 0x0f);
			flags->SF = 0;
			flags->ZF = 0;
			v = regA & regB;
			if (v == 0) {
				flags->ZF = 1;
			}
			if (v < 0) {
				flags->SF = 1;
			}
			processor->registers[regB] = v;
			processor->eip = processor->eip + 2;
		}
		else if(mem[processor->eip] == 0x63){ /* xorl: EXCLUSIVE OR operand */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			regB = (mem[processor->eip + 1] & 0x0f);
			flags->SF = 0;
			flags->ZF = 0;
			v = regA ^ regB;
			if (v == 0) {
				flags->ZF = 1;
			}
			if (v < 0) {
				flags->SF = 1;
			}
			processor->registers[regB] = v;
			processor->eip = processor->eip + 2;
		}
		else if(mem[processor->eip] == 0x64){ /* imull: multiply two registers, save value in register B */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			regB = (mem[processor->eip + 1] & 0x0f);
			flags->OF = 0;
			flags->SF = 0;
			flags->ZF = 0;
			v = processor->registers[regA] * processor->registers[regB];
			if ((processor->registers[regA] > 0 && processor->registers[regB] > 0 && v < 0) || (processor->registers[regA] < 0 && processor->registers[regB] < 0 && v < 0) || (processor->registers[regA] < 0 && processor->registers[regB] > 0 && v > 0) || (processor->registers[regA] > 0 && processor->registers[regB] < 0 && v > 0)) { 
			flags->OF = 1;
			}
			processor->registers[regB] = v;
			if(v == 0)
				flags->ZF = 1;
			if(v < 0)
				flags->SF = 1;
			processor->eip = processor->eip + 2;
		}
		else if(mem[processor->eip] == 0x65){ /* cmpl: compares two registers logically (like subl but discards the result and sets the flags) */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			regB = (mem[processor->eip + 1] & 0x0f);
			flags->OF = 0;
			flags->SF = 0;
			flags->ZF = 0;
			v = processor->registers[regB] - processor->registers[regA];
			if ((processor->registers[regA] > 0 && processor->registers[regB] > 0 && v < 0) || (processor->registers[regA] < 0 && processor->registers[regB] < 0 && v > 0)) {
				flags->OF = 1;
			}
			if (v == 0) {
				flags->ZF = 1;
			}
			if (v < 0) {
				flags->SF = 1;
			}
			processor->eip = processor->eip + 2;
		}
		else if(mem[processor->eip] == 0x70){ /* jmp: jump to destination with no condition */
			dst = mem[processor->eip + 1] + (mem[processor->eip + 2] << 8) + (mem[processor->eip + 3] << 16) + (mem[processor->eip + 4] << 24);
			processor->eip = dst;
		}
		else if(mem[processor->eip] == 0x71){ /* jle: jump to destination if less or equal */
			dst = mem[processor->eip + 1] + (mem[processor->eip + 2] << 8) + (mem[processor->eip + 3] << 16) + (mem[processor->eip + 4] << 24);
			if ((flags->SF ^ flags->OF) || flags->ZF)
				processor->eip = dst;
			else
				processor->eip = processor->eip + 5;
		}
		else if(mem[processor->eip] == 0x72){ /* jl: jump to destination if less */
			dst = mem[processor->eip + 1] + (mem[processor->eip + 2] << 8) + (mem[processor->eip + 3] << 16) + (mem[processor->eip + 4] << 24);
			if ((flags->SF ^ flags->OF) == 1)
				processor->eip = dst;
			else
				processor->eip = processor->eip + 5;
		}
		else if(mem[processor->eip] == 0x73){ /* je: jump to destination if equal */
			dst = mem[processor->eip + 1] + (mem[processor->eip + 2] << 8) + (mem[processor->eip + 3] << 16) + (mem[processor->eip + 4] << 24);
			if (flags->ZF == 1)
				processor->eip = dst;
			else
				processor->eip = processor->eip + 5;
		}
		else if(mem[processor->eip] == 0x74){ /* jne: jump to destination if not equal */
			dst = mem[processor->eip + 1] + (mem[processor->eip + 2] << 8) + (mem[processor->eip + 3] << 16) + (mem[processor->eip + 4] << 24);
			if (flags->ZF == 0)
				processor->eip = dst;
			else
				processor->eip = processor->eip + 5;
		}
		else if(mem[processor->eip] == 0x75){ /* jge: jump to destination if greater than or equal */
			dst = mem[processor->eip + 1] + (mem[processor->eip + 2] << 8) + (mem[processor->eip + 3] << 16) + (mem[processor->eip + 4] << 24);
			if ((flags->SF ^ flags->OF) == 0)
				processor->eip = dst;
			else
				processor->eip = processor->eip + 5;
		}
		else if(mem[processor->eip] == 0x76){ /* jg: jump to destination if greater */
			dst = mem[processor->eip + 1] + (mem[processor->eip + 2] << 8) + (mem[processor->eip + 3] << 16) + (mem[processor->eip + 4] << 24);
			if (((flags->SF ^ flags->OF) == 0) && flags->ZF == 0)
				processor->eip = dst;
			else
				processor->eip = processor->eip + 5;
		}
		else if(mem[processor->eip] == 0x80){ /* call: call a function and do the instructions there */
			printf("Program counter: %d\n", processor->eip);
			dst = mem[processor->eip + 1] + (mem[processor->eip + 2] << 8) + (mem[processor->eip + 3] << 16) + (mem[processor->eip + 4] << 24);
			processor->registers[ESP] = processor->registers[ESP] - 4;
			inputL = processor->eip + 5; 
			v = 0;
			mem[processor->registers[ESP]] = (inputL & 0x000000ff);
			mem[processor->registers[ESP] + 1] = (inputL & 0x0000ff00) >> 8;
			mem[processor->registers[ESP] + 2] = (inputL & 0x00ff0000) >> 16;
			mem[processor->registers[ESP] + 3] = (inputL & 0xff000000) >> 24;
			processor->eip = dst;
			printf("Program counter: %d\n", processor->eip);
		}
		else if(mem[processor->eip] == 0x90){ /* ret: return the value in this function and go to previous function */
			dst = (mem[processor->registers[ESP]]) + (mem[processor->registers[ESP] + 1] << 8) + (mem[processor->registers[ESP] + 2] << 16) + (mem[processor->registers[ESP] + 3] << 24);
			processor->eip = dst;
			processor->registers[ESP] = processor->registers[ESP] + 4;
		}
		else if(mem[processor->eip] == 0xa0){ /* pushl: push to the stack */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			processor->registers[ESP] = processor->registers[ESP] - 4;
			mem[processor->registers[ESP]] = (processor->registers[regA]);
			mem[processor->registers[ESP] + 1] = (processor->registers[regA] & 0x0000ff00) >> 8;
			mem[processor->registers[ESP] + 2] = (processor->registers[regA] & 0x00ff0000) >> 16;
			mem[processor->registers[ESP] + 3] = (processor->registers[regA] & 0xff000000) >> 24;
			processor->eip = processor->eip + 2;
		}
		else if(mem[processor->eip] == 0xb0){ /* popl: pop from the stack */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			processor->registers[regA] = (mem[processor->registers[ESP]]) + (mem[processor->registers[ESP] + 1] << 8) + (mem[processor->registers[ESP] + 2] << 16) + (mem[processor->registers[ESP] + 3] << 24);
			processor->registers[ESP] = processor->registers[ESP] + 4;
			processor->eip = processor->eip + 2;
		}
		else if(mem[processor->eip] == 0xc0){ /* readb: read a byte from the user */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			dst = mem[processor->eip + 2] + (mem[processor->eip + 3] << 8) + (mem[processor->eip + 4] << 16) + (mem[processor->eip + 5] << 24);
			scanf("%c", &inputB);
			/* 1 byte */
			mem[processor->registers[regA] + dst] = inputB;
			processor->eip = processor->eip + 6;
		}
		else if(mem[processor->eip] == 0xc1){ /* readl: read 4 bytes from the user */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			dst = mem[processor->eip + 2] + (mem[processor->eip + 3] << 8) + (mem[processor->eip + 4] << 16) + (mem[processor->eip + 5] << 24);
			scanf("%i", &inputL);
			/* 4 bytes */
			mem[processor->registers[regA] + dst] = (inputL & 0x000000ff);
			mem[processor->registers[regA] + dst + 1] = (inputL & 0x0000ff00) >> 8;
			mem[processor->registers[regA] + dst + 2] = (inputL & 0x00ff0000) >> 16;
			mem[processor->registers[regA] + dst + 3] = (inputL & 0xff000000) >> 24;
			processor->eip = processor->eip + 6;
		}
		else if(mem[processor->eip] == 0xd0){ /* writeb: print a byte to the user */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			dst = mem[processor->eip + 2] + (mem[processor->eip + 3] << 8) + (mem[processor->eip + 4] << 16) + (mem[processor->eip + 5] << 24);
			printf("%c", mem[processor->registers[regA] + dst]);
			processor->eip = processor->eip + 6;
		}
		else if(mem[processor->eip] == 0xd1){ /* writel: print 4 bytes to the user */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			regB = (mem[processor->eip + 1] & 0x0f);
			dst = mem[processor->eip + 2] + (mem[processor->eip + 3] << 8) + (mem[processor->eip + 4] << 16) + (mem[processor->eip + 5] << 24);
			v =  (mem[processor->registers[regB] + dst]) + (mem[processor->registers[regB] + dst + 1] << 8) + (mem[processor->registers[regB] + dst + 2] << 16) + (mem[processor->registers[regB] + dst + 3] << 24);	
			printf("%i", v);
			processor->eip = processor->eip + 6;
		}
		/* STILL WORKING ON IT: MOVSBL */
		else if(mem[processor->eip] == 0xe0){ /* movsbl: move to destination with sign extension from 1 byte to 4 bytes */
			regA = (mem[processor->eip + 1] & 0xf0) >> 4;
			regB = (mem[processor->eip + 1] & 0x0f);
			dst = mem[processor->eip + 2] + (mem[processor->eip + 3] << 8) + (mem[processor->eip + 4] << 16) + (mem[processor->eip + 5] << 24);
			v = (0x0000ffff & processor->registers[regA]);
			if(0x00008000 & processor->registers[regA]){
				v = v + 0xffff0000;
			}
			processor->registers[regA] = (mem[processor->registers[regB] + dst]) + (mem[processor->registers[regB] + dst + 1]<<8) + (mem[processor->registers[regB] + dst + 2]<<16) + (mem[processor->registers[regB] + dst + 3]<<24);
			processor->eip = processor->eip + 6;
		}
		else{ /* invalid instruction */
			printf("ERROR: at pointer = %i\n", processor->eip);
			printf("\tValue in memory: %x\n", mem[processor->eip]);
			fprintf(stderr, "Might be an invalid instruction.\n");
			status->INS = 1;
			status->AOK = 0;
			return;
		}
		regA = 0;
		regB = 0;
		dst = 0;
		v = 0;
	} 
	/* if it reaches here, unknown error */
	fprintf(stderr, "ERROR.\n");
	return;
} 

int main(int argc, const char * argv[]) {
	decode(fetch(argv[1]));
	return 0;
}