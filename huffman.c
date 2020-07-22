#include "huffman.h"

int *frequencies(char *data, int dataLen) {
    int *freq = calloc(256, sizeof(int));
    for (int i = 0; i < dataLen; i++) {
        freq[data[i]]++;
    }
    return freq;
}

HuffmanNodeArray nodes(int *freq) {
    HuffmanNodeArray n;
    n.nodes = malloc(sizeof(HuffmanNode*) * 256);
    n.nodeFresh = malloc(sizeof(char) * 256);
    for (int i = 0; i < 256; i++) {
        n.nodes[i] = malloc(sizeof(HuffmanNode));
        n.nodes[i]->c0 = NULL;
        n.nodes[i]->c1 = NULL;
        n.nodes[i]->value = (short) i;
        n.nodes[i]->weight = freq[i];
        n.nodeFresh[i] = 1;
    }
    return n;
}

int nodeWeight(HuffmanNode *node) {
    if (node->value != -1) {
        return node->weight;
    } else {
        return nodeWeight(node->c0) + nodeWeight(node->c1);
    }
}

short nodeCount(HuffmanNodeArray arr) {
    short nodes = 0;
    for (int i = 0; i < 256; i++) {
        nodes += (short) arr.nodeFresh[i];
    }
    return nodes;
}

HuffmanNode **smallestNode(HuffmanNodeArray arr) {
    int lowestWeight = INT_MAX;
    HuffmanNode **node = NULL;
    for (int i = 0; i < 256; i++) {
        if (arr.nodeFresh[i] == 0)
            continue;
        int weight = nodeWeight(arr.nodes[i]);
        if (weight < lowestWeight) {
            lowestWeight = weight;
            node = arr.nodes + i;
        }
    }
    return node;
}

HuffmanNode *constructHuffmanEncoding(int *frequencies) {
    HuffmanNodeArray nodeArray = nodes(frequencies);
    while (nodeCount(nodeArray) > 1) {
        HuffmanNode **a = smallestNode(nodeArray);
        int aWeight = nodeWeight(*a);
        long aIdx = a - nodeArray.nodes;
        nodeArray.nodeFresh[aIdx] = 0;
        
        HuffmanNode **b = smallestNode(nodeArray);
        int bWeight = nodeWeight(*b);
        long bIdx = b - nodeArray.nodes;
        nodeArray.nodeFresh[bIdx] = 0;
        
        HuffmanNode *newNode = malloc(sizeof(HuffmanNode));
        if (aWeight < bWeight) {
            newNode->c0 = *a;
            newNode->c1 = *b;
        } else {
            newNode->c0 = *b;
            newNode->c1 = *a;
        }
        newNode->value = -1;
        newNode->weight = 0;
        
        nodeArray.nodes[aIdx] = newNode;
        nodeArray.nodeFresh[aIdx] = 1;
    }
    HuffmanNode *finalNode = NULL;
    for (int i = 0; i < 256; i++) {
        if (nodeArray.nodeFresh[i] == 1) {
            finalNode = nodeArray.nodes[i];
        }
    }
    free(nodeArray.nodeFresh);
    free(nodeArray.nodes);
    return finalNode;
}

void _exportHuffmanTree(HuffmanNode *node, unsigned char **bitStrings, int *bitLengths, unsigned char *bitStringRoot, unsigned char *bitString) {
    if (node->value == -1) {
        *bitString = 0;
        _exportHuffmanTree(node->c0, bitStrings, bitLengths, bitStringRoot, bitString + 1);
        *bitString = 1;
        _exportHuffmanTree(node->c1, bitStrings, bitLengths, bitStringRoot, bitString + 1);
    } else {
        int bsSize = (int) (bitString - bitStringRoot);
        unsigned char *bsCopy = malloc(sizeof(unsigned char) * bsSize);
        memcpy(bsCopy, bitStringRoot, bsSize);
        bitStrings[node->value] = bsCopy;
        bitLengths[node->value] = bsSize;
    }
}

MinimalHuffmanTree exportHuffmanTree(HuffmanNode *node) {
    MinimalHuffmanTree hft;
    unsigned char *bs = malloc(sizeof(char) * 300);
    hft.bits = malloc(sizeof(unsigned char *) * 256);
    hft.len = malloc(sizeof(int) * 256);
    _exportHuffmanTree(node, hft.bits, hft.len, bs, bs);
    free(bs);
    return hft;
}

void freeHuffmanTree(HuffmanNode *node) {
    if (node->value == -1) {
        freeHuffmanTree(node->c0);
        freeHuffmanTree(node->c1);
    }
    free(node);
}

BitArray encodeBytes(MinimalHuffmanTree tree, char *bytes, int len) {
    BitArray ba = newBitArray();
    for (int i = 0; i < len; i++) {
        appendToBitArray(&ba, tree.bits[((unsigned char *) bytes)[i]], tree.len[((unsigned char *) bytes)[i]]);
    }
    return ba;
}

char compareBytesWithLength(unsigned char *a, unsigned char *b, int len) {
    for (int i = 0; i < len; i++) {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

unsigned char *decodeBits(MinimalHuffmanTree tree, BitArray bits, int len) {
    unsigned char *bs = malloc(sizeof(unsigned char) * 300);
    int bc = 0;
    int j = 0;
    unsigned char *currentByte = bits.bytes;
    unsigned char *values = malloc(sizeof(unsigned char) * len);
    int y = 0;
    for (int i = 0; i < bits.sizeBits; i++) {
        int b = (*currentByte >> (7 - bc++)) & 0x1;
        bs[j++] = (char) b;
        for (int k = 0; k < 256; k++) {
            if (tree.len[k] != j)
                continue;
            if (compareBytesWithLength(tree.bits[k], bs, j) == 1) {
                values[y++] = (char) k;
                j = 0;
                break;
            }
        }
        if (bc == 8) {
            bc = 0;
            currentByte++;
        }
    }
    free(bs);
    return values;
}
