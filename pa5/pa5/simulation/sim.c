#include <stdio.h>
#include <stdlib.h>
#include "sim.h" /* structs and functions useful */

/* CHECKING THE VALIDITY OF THE ARGUMENTS */

/*
 *	This function returns a boolean value after checking the validity of the arguments.
 */

bool Arguments(char *cachesize,char *associativity,char *blocksize,char *replacealg,char *writepolicy, Cache *cache) {
	int cacheSize = 0;
	int setSize = 0;
	int bsize = 0;
	int numOfSets = 0;
	char replaceAlg;
	char writePolicy;
	/* valid cache size. */
	cacheSize = CSize(cachesize);
	if(cacheSize == 0)
		return false;
	/* valid set size */
	setSize = Associativity(associativity);
	if(setSize == 0)
		return false;
	/* valid block size */
	bsize = BSize(blocksize);
	if(bsize == 0)
		return false;
	/* valid number of sets */
	numOfSets = cacheSize/(setSize*bsize);
	if(numOfSets < 1)
		return false;
	/* valid algorithm */
	replaceAlg = RepAlgorithm(replacealg);
	if((replaceAlg == '\0'))
		return false;
	/* valid wp */
	writePolicy = WPolicy(writepolicy);
	if(writePolicy == '\0')
		return false;
	cache->cacheSize = cacheSize;
	cache->setSize = setSize;
	cache->bsize = bsize;
	cache->numOfSets = numOfSets;
	cache->writePolicy = writePolicy;
	cache->replaceAlg = replaceAlg;
	return true;
}

/*
 *	checks if the argument is a power of two or not.
 */

bool isPowerOfTwo(int v) {
	int i = 0;
	while(i < 31) {
		if(v == 1 << i)
			return true;
	i++;
	}
	return false;
}

/*
*	PROCESSING ARGUMENTS:
		- Direct-mapped: done
		- Associative: not implemented
		- FIFO: done
		- LRU: not implemented 
*/
void process(char* tf, Cache *cache) {
	FILE *ft; 
	int i; /* counter */
	/* TO READ THE TRACE FILES */
	char line[80];
	char pointer[50];
	char mode;
	unsigned int address;
	int noOfLines;
	/* CACHE ELEMENTS */
	Set *currentSet;
	Set *baseSet;
	Line *currentLine;
	/* CALCULATE */
	int hits = 0;
	int misses = 0;
	int readCount = 0;
	int writeCount = 0;
	/* CACHE VARIABLES */
	int setSize = cache->setSize;
	int numOfSets = cache->numOfSets;
	int bsize = cache->bsize;
	int writePolicy = cache->writePolicy;
	/* VARIABLES OF BIT SECTIONS */
	int set; /* selector */
	int tag;
	int offset; /* actual value */
	/* BITS REQUIRED */
	int setBits;
	int blockBits;
	/* ISOLATION MASKS */
	int bmask; /* to isolate offset */
	int smask; /* to isolate selector */
	int tmask; /* to isolate tage */
	bmask = bsize - 1;
	blockBits = maskbits(bsize);
	setBits = maskbits(numOfSets);
	smask = (numOfSets - 1) << blockBits;
	tmask = (-1^smask)^bmask;
	/* MEMORY ALLOCATION */
	baseSet = (Set *)malloc(numOfSets*sizeof(Set));
	/* ALLOCATION MEMEORY FOR EACH LINE OF THE CACHE */
	i = 0;
	while(i < numOfSets) {
		currentSet = baseSet+i;
		currentSet->base = (Line *)malloc(setSize*sizeof(Line) + 5);
		i++;
	}
	currentSet = baseSet;
	/* OPENING THE FILE */
	ft = fopen(tf,"r");
	if(!ft) {
		fprintf(stderr,"ERROR: could not open tracefile.");
		return;
	}
	/* SIMULATION */
	for(noOfLines = 0;fgets(line,80,ft) != NULL; noOfLines++) {
		sscanf(line,"%s %c %x",pointer, &mode, &address);
		/* FINDING VALUE IN LEAST SIGNIFICANT BITS */
		offset = address&bmask;
		/* FINDING SELECTOR */
		set = (address&smask) >> blockBits;
		/* FINDING TAG */
		tag = ((address&tmask) >> blockBits) >> setBits;
		currentSet = baseSet+set;
		i = 0;
		while(i< setSize) {
		currentLine = currentSet->base + i;
			if(currentLine->tag == tag) { /* TAG FOUND IN THE CACH: HIT */
				hits++;
				switch(mode){
					case 'W':
						if(writePolicy == 't')
							writeCount++;
						else if(writePolicy == 'b')
							currentLine->modified = true;
						break;
					case 'R':
						readCount++;
						break;
				}
			}else{ /* TAG MISSING FROM THE CACHE */
				misses++;
				if(currentLine->modified) { /* DIRTY */
					writeCount++;
					currentLine->modified = false;
				}
				currentLine->tag = tag;
				switch(mode){
					case 'W':
						if(writePolicy == 't') {
						readCount++;
						writeCount++;
					}else if(writePolicy == 'b'){
						readCount++;
						currentLine->modified = false;
					}
						break;
					case 'R':
						break;
				}
			}
		i++;
		}
	}
	/* PRINT OUT RESULTS */
	printf("Number of lines in the trace file: %d\n", noOfLines);
	printf("Memory reads: %d\n", readCount);
	printf("Memory writes: %d\n", writeCount);
	printf("Cache hits: %d\n", hits);
	printf("Cache misses: %d\n", misses);
	free(baseSet);
	free(currentSet->base);
	fclose(ft);
	return;
}

int main(int argc, char** argv) {
	/* NEW CACHE */
	Cache cache;
	/* ARGUMENT CHECK */
    if(argc == 2){
		if(strcmp(argv[1],"-h") == 0) {
		printf("How to use:\n");
		printf("./sim [-h] <cache size> <associativity> <block size> <rep algorithm> <write policy> <tracefile.txt>\n");
		return 0;
		}
	}		
	/* PASSING ARGUMENTS TO VALIDITY CHECK */
    else if(argc == 7){
		if(Arguments(argv[1], argv[2], argv[3], argv[4], argv[5], &cache)) {
			if(cache.setSize > 1) {
			  fprintf(stderr,"ERROR: not implemented.\n");
			  return -1;
			}
			process(argv[6],&cache);
			return 0;
		}
	}
	/* ERROR REPORT */
	fprintf(stderr,"You have encountered an error with your arguments. Try again, or input [./sim -h] for help.\n");
	return 0;
}