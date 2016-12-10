#include <string.h> /* string functions */
#include <stdbool.h> /* boolean values */
bool isPowerOfTwo(int v);
int maskbits(int x);

/* THE CACHE STRUCT */
typedef struct {
	int cacheSize;
	int setSize;
	int numSets;
	int blockSize;
	char replaceAlg;
	char writePolicy;
}Cache;

/* THE LINE STRUCT */
typedef struct {
	int tag;
	bool modified;
}Line;

/* THE SET STRUCT */
typedef struct {
	Line* base;
}Set;

/* WRITING POLICY DETECTOR */
char WPolicy(char *w) {
	char policy;
	if(strcmp(w,"-wt") == 0)
		policy = 't';
	else if(strcmp(w,"-wb") == 0)
		policy = 'b';
	return policy;
}

/* REPLACEMENT ALGORITHM DETECTOR */
char RepAlgorithm(char *a) {
	char alg;
	if(strcmp(a,"-FIFO") == 0){
		alg = 'f';
	}
	else if(strcmp(a,"-LRU") == 0){
		fprintf(stderr, "ERROR: LRU NOT IMPLEMENTED.\n");
	}
	return alg;
}

/* BLOCK SIZE DETECTOR */
int BSize(char *blocksize) {
	int blockSize;
	int scan = sscanf(blocksize, "%d", &blockSize);
	if(scan == 0)
		return 0;
	if(isPowerOfTwo(blockSize) == true)
		return blockSize;
	return 0;
}

/* CACHE SIZE DETECTOR */
int CSize(char *cachesize) { 
	int cacheSize;
	int scan = sscanf(cachesize, "%d", &cacheSize);
	if((scan = 0))
		return 0;
	if(isPowerOfTwo(cacheSize) == true)
		return cacheSize;
	return 0;
}

/* ASSOCIATIVITY DETECTOR (Associative is not yet implemented) */
int Associativity(char *assoc) {
	int setSize = 0;
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