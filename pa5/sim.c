#include <stdio.h>
#include <stdlib.h>
#include "sim.h" /* structs and functions useful */

/*
 *	OPENING THE FILE (GLOBAL).
 */

FILE *ft; 
int readCount;
int writeCount;
int hits;
int misses;

/* CHECKING THE VALIDITY OF THE ARGUMENTS */

/*
 *	This function returns a boolean value after checking the validity of the arguments.
 */

bool Arguments(char *cachesize,char *associativity,char *blocksize,char *replacealg,char *writepolicy,Cache *cache) {
	int cacheSize = 0;
	int setSize = 0;
	int blockSize = 0;
	int numSets = 0;
	char replaceAlg;
	char writePolicy;
	/* valid cache size. */
	cacheSize = CSize(cachesize);
	if(cacheSize == 0)
		return false;
	cache->cacheSize = cacheSize;
	/* valid set size */
	setSize = Associativity(associativity);
	if(setSize == 0)
		return false;
	cache->setSize = setSize;
	/* valid block size */
	blockSize = BSize(blocksize);
	if(blockSize == 0)
		return false;
	cache->blockSize = blockSize;
	/* valid number of sets */
	numSets = cacheSize/(setSize*blockSize);
	if(numSets == 0)
		return false;
	cache->numSets = numSets;
	/* valid algorithm */
	replaceAlg = RepAlgorithm(replacealg);
	if((replaceAlg == '\0'))
		return false;
	cache->replaceAlg = replaceAlg;
	/* valid wp */
	writePolicy = WPolicy(writepolicy);
	if(writePolicy == '\0')
		return false;
	
	cache->writePolicy = writePolicy;
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
void process(Cache *cache,char* tf) {
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
	/* CACHE VARIABLES */
	int setSize = cache->setSize;
	int numSets = cache->numSets;
	int blockSize = cache->blockSize;
	int writePolicy = cache->writePolicy;
	/* VARIABLES OF BIT SECTIONS */
	int set; /* selector */
	int tag;
	int blockOffset; /* actual value */
	/* BITS REQUIRED */
	int setBits;
	int blockBits;
	/* ISOLATION MASKS */
	int blockMask; /* to isolate offset */
	int setMask; /* to isolate selector */
	int tagMask; /* to isolate tage */
	blockMask = blockSize - 1;
	blockBits = maskbits(blockSize);
	setBits = maskbits(numSets);
	setMask = (numSets - 1) << blockBits;
	tagMask = (-1^setMask)^blockMask;
	/* MEMORY ALLOCATION */
	baseSet = (Set *)malloc(numSets*sizeof(Set));
	/* ALLOCATION MEMEORY FOR EACH LINE OF THE CACHE */
	i = 0;
	while(i < numSets) {
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
		blockOffset = address&blockMask;
		/* FINDING SELECTOR */
		set = (address&setMask) >> blockBits;
		/* FINDING TAG */
		tag = ((address&tagMask) >> blockBits) >> setBits;
		currentSet = baseSet+set;
		i = 0;
		while(i< setSize) {
		currentLine = currentSet->base + i;
			if(currentLine->tag == tag) { /* TAG FOUND IN THE CACH: HIT */
				hits++;
				if(mode == 'W') {
					if(writePolicy == 't')
						writeCount++;
					else
						currentLine->modified = true;
				}else if(mode == 'R')
					readCount++;
			}else{ /* TAG MISSING FROM THE CACHE */
				misses++;
				if(currentLine->modified) { /* DIRTY */
					writeCount++;
					currentLine->modified = false;
				}
				currentLine->tag = tag;
				if(mode == 'W') {
					if(writePolicy == 't') {
						readCount++;
						writeCount++;
					}else if(writePolicy == 'b'){
						readCount++;
						currentLine->modified = false;
					}
				}
			}
		i++;
		}
	}
	i = 0;
	while(i < numSets) {
		currentSet = baseSet+i;
		free(currentSet->base);
		i++;
	}
	/* PRINT OUT RESULTS */
	printf("Number of lines in the trace file: %d\n", noOfLines);
	printf("Memory reads: %d\n", readCount);
	printf("Memory writes: %d\n", writeCount);
	printf("Cache hits: %d\n", hits);
	printf("Cache misses: %d\n", misses);
	free(baseSet);
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
		if(Arguments(argv[1],argv[2],argv[3],argv[4],argv[5],&cache)) {
			if(cache.setSize > 1) {
			  fprintf(stderr,"ERROR: not implemented:");
			  return -1;
			}
			process(&cache,argv[6]);
			return 0;
		}
	}
	/* ERROR REPORT */
	fprintf(stderr,"You have encountered an error with your arguments. Try again, or input [./sim -h] for help.\n");
	return 0;
}