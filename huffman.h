#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "bitarray.h"

typedef struct HuffmanNode {
    struct HuffmanNode *c0;
    struct HuffmanNode *c1;
    short value;
    int weight;
} HuffmanNode;

typedef struct {
    HuffmanNode **nodes;
    char *nodeFresh;
} HuffmanNodeArray;

typedef struct {
    unsigned char **bits;
    int *len;
} MinimalHuffmanTree;

int *frequencies(char *data, int dataLen);
HuffmanNode *constructHuffmanEncoding(int *frequencies);
MinimalHuffmanTree exportHuffmanTree(HuffmanNode *node);
void freeHuffmanTree(HuffmanNode *node);
BitArray encodeBytes(MinimalHuffmanTree tree, char *bytes, int len);
unsigned char *decodeBits(MinimalHuffmanTree tree, BitArray bits, int len);
