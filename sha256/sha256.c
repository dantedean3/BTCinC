#include <stdio.h>
#include <string.h>

void sha256(const char *str, char outputBuffer[65]) {
    /* Dummy hash: just fill the output with '0's and terminate with null char */
    int i;
    for (i = 0; i < 64; i++) {
        outputBuffer[i] = '0';
    }
    outputBuffer[64] = '\0';
}
