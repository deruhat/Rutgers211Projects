#include <string.h> /* string functions */
#include <stdbool.h> /* boolean values */
bool isPowerOfTwo(int v);
int maskbits(int x);

/* THE CACHE STRUCT */
typedef struct {
	int cacheSize; /* cache size */
	int setSize; /* set size */
	int numOfSets; /* number of sets */
	int bsize; /* block size */
	char replaceAlg; /* FIFO-LRU */
	char writePolicy; /* wt-wb */
}Cache;

/* THE LINE STRUCT */
typedef struct {
	int tag; /* tag holder */
	bool modified; /* dirty indicator */
}Line;

/* THE SET STRUCT */
typedef struct {
	Line* base; /* base of the line within the set */
}Set;

/* WRITING POLICY DETECTOR */
unsigned char WPolicy(char *w) {
	unsigned char policy;
	if(strcmp(w,"-wt") == 0)
		policy = 't';
	else if(strcmp(w,"-wb") == 0)
		policy = 'b';
	return policy;
}

/* REPLACEMENT ALGORITHM DETECTOR */
unsigned char RepAlgorithm(char *a) {
	unsigned char alg;
	if(strcmp(a,"-FIFO") == 0){
		alg = 'f';
	}
	else if(strcmp(a,"-LRU") == 0){
		fprintf(stderr, "ERROR: LRU NOT IMPLEMENTED.\n");
	}
	return alg;
}

/* BLOCK SIZE DETECTOR */
unsigned int BSize(char *blocksize) {
	unsigned int blockSize;
	unsigned int scan = sscanf(blocksize, "%u", &blockSize);
	if(scan == 0)
		return 0;
	if(isPowerOfTwo(blockSize) == true)
		return blockSize;
	return 0;
}

/* CACHE SIZE DETECTOR */
unsigned int CSize(char *cachesize) { 
	unsigned int cacheSize;
	unsigned int scan = sscanf(cachesize, "%u", &cacheSize);
	if((scan = 0))
		return 0;
	if(isPowerOfTwo(cacheSize) == true)
		return cacheSize;
	return 0;
}

/* ASSOCIATIVITY DETECTOR (Associative is not yet implemented) */
unsigned int Associativity(char *assoc) {
	unsigned int setSize = 0;
	if(strcmp(assoc,"-direct") == 0)
		setSize = 1;
	else if(strcmp(assoc,"-assoc") == 0){
		fprintf(stderr, "ERROR: ASSOCIATIVE FUNCTION NOT IMPLIMENTED.\n");
    }
	return setSize;
}

/* MASK */
int maskbits(int x) {
	int y = 0;
	while(y < 31) { /* 32 bits */
		if((x >> y) == 1){
			y++;
			break;
		}
	y++;
	}
	return y;
}