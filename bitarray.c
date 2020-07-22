#include "bitarray.h"

BitArray newBitArray() {
    BitArray array;
    array.bytes = calloc(1, sizeof(char));
    array.sizeBits = 0;
    array.lastBitOffset = 0;
    array.capacity = 1;
    return array;
}

int bytesInBitArray(BitArray *arr) {
    int bytes = arr->sizeBits / 8;
    if (arr->sizeBits % 8 > 0)
        bytes++;
    return bytes;
}

void appendToBitArray(BitArray *arr, unsigned char *bits, int length) {
    int bytesNeeded = length / 8;
    if (length % 8 > 0)
        bytesNeeded++;
    int bitsAvailable = arr->capacity * 8 - arr->sizeBits;
    if (bitsAvailable < length) {
        int expSize = arr->capacity * 2;
        int expAvailable = expSize * 8 - arr->sizeBits;
        if (expAvailable < length) { // Enough for the length
            int lenSize = arr->capacity + bytesNeeded;
            arr->bytes = realloc(arr->bytes, lenSize);
            for (int i = arr->capacity; i < lenSize; i++)
                arr->bytes[i] = 0;
            arr->capacity = lenSize;
        } else { // Exponential growth
            arr->bytes = realloc(arr->bytes, expSize);
            for (int i = arr->capacity; i < expSize; i++)
                arr->bytes[i] = 0;
            arr->capacity = expSize;
        }
    }
    
    unsigned char *currentByte = arr->bytes + bytesInBitArray(arr);
    int currentOffset = arr->lastBitOffset;
    if (currentOffset != 0)
        currentByte--;
    for (int i = 0; i < length; i++) {
        *currentByte |= bits[i] << (7 - currentOffset);
        currentOffset++;
        arr->sizeBits++;
        if (currentOffset == 8) {
            currentOffset = 0;
            currentByte++;
        }
    }
    arr->lastBitOffset = currentOffset;
}
