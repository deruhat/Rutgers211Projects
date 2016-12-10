#include "declarations.h"

typedef struct processor {
	int registers[9];
	int size;
	int eip;
	int cont;
} processor;

typedef struct flags {
	unsigned int OF;
	unsigned int ZF;
	unsigned int SF;
} flags;

typedef struct status {
	unsigned int AOK;
	unsigned int HLT;
	unsigned int ADR;
	unsigned int INS;
} status;

int dirSize(char* tok, int size){
	while (tok != NULL) {
		if (strcmp(tok, ".size") == 0) {
			tok = strtok(NULL, "\t\n");
			size = hex_to_dec(tok);
		} else {
			tok = strtok(NULL, "\t\n");
		}
	}
	return size;
}

char * fetch(const char * filename);
void decode(char * code);
void execute(unsigned char * bytes, processor * processor);

