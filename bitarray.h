#include <stdlib.h>

typedef struct BitArray {
    unsigned char *bytes;
    int sizeBits;
    int lastBitOffset;
    int capacity;
} BitArray;

BitArray newBitArray(void);
void appendToBitArray(BitArray *arr, unsigned char *bits, int length);
