#include <stdio.h>
#include <unistd.h>
#include "huffman.h"

int main() {
    char *someText = "Teeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeteeeeeeeeeeeering test";
    int inputLen = (int) strlen(someText) + 1;
    
    int *freq = frequencies(someText, inputLen);
    HuffmanNode *node = constructHuffmanEncoding(freq);
    MinimalHuffmanTree tree = exportHuffmanTree(node);
    BitArray textBA = encodeBytes(tree, someText, inputLen);
    
    printf("Compressed from %d bits to %d bits.\n", inputLen * 8, textBA.sizeBits);
    
    unsigned char *v = decodeBits(tree, textBA, inputLen);
    printf("%s\n", v);
    
    free(freq);
    freeHuffmanTree(node);
    for (int i = 0; i < 256; i++)
        free(tree.bits[i]);
    free(tree.bits);
    free(tree.len);
    free(textBA.bytes);
    free(v);
    
    return 0;
}
