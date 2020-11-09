#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.h";
#include "write.h"
#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif

void writeOutputToFile() {
    int i = 0;
    int j = 0;
    char t[15];
    char t2[6];
    FILE* outputFile;
    outputFile = fopen("output.txt", "w");
    for (; i < memRowAmount; i++) {
        j = 0;
        for (; j < ROW_ARRAY[i].amount; j++) {
            if (ROW_ARRAY[i].order[1] == 'S') {
                fprintf(outputFile, "~~ ~~ ~~ ~~\n");
            }
            else {
                sprintf(t, "%08X\n", ROW_ARRAY[i].value);
                fprintf(outputFile, "%c%c %c%c %c%c %c%c\n", t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7]);
            }
        }
    }
    fprintf(outputFile, "\n");
    for (i = memRowAmount; i < lineAmount; i++) {
        if (ROW_ARRAY[i].byteAmount == 4) {
            fprintf(outputFile, "%02X %01X%01X ", ROW_ARRAY[i].hex, ROW_ARRAY[i].arg1VAL, ROW_ARRAY[i].arg2VAL);
            sprintf(t2, "%04X", ROW_ARRAY[i].move);
            fprintf(outputFile, "%c%c %c%c\n", t2[0], t2[1], t2[2], t2[3]);
        }
        else fprintf(outputFile, "%02X %01X%01X\n", ROW_ARRAY[i].hex, ROW_ARRAY[i].arg1VAL, ROW_ARRAY[i].arg2VAL);
    }
    return;
}