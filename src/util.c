
#include "util.h"

// Bug fixed. Remember to use unsigned char for your bytes, kids!
int load_bytes(byte* bytes, int len) {
    int x = 0;
    for (int i = 0; i < len; i++) {
        int offset = len - 1 - i;
        x += bytes[i] << (8 * offset);
    }
    return x;
}

void write_bytes(byte* bytes, int value, int len) {
    for (int i = 0; i < len; i++) {
        int offset = len - 1 - i;
        bytes[i] = value << (8 * offset);
    }
}

