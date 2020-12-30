#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.h"
#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif
#define MACHINE_CODE_ROW_LEN 10
#define OFFSET_LEN 6
void writeOutputToFile() { //funkcja wypisuj¹ca kod maszynowy do pliku
    int i = 0;
    int j = 0;
    char t[MACHINE_CODE_ROW_LEN];
    char t2[OFFSET_LEN];
    FILE* outputFile;
    outputFile = fopen("output.txt", "w");
    for (; i < memRowAmount; i++) {

        for (j = 0; j < ROW_ARRAY[i].amount; j++) {
            if (ROW_ARRAY[i].order[1] == 'S') { // jeœli derektywa to DS wypisz "~~ ~~ ~~ ~~"
                fprintf(outputFile, "~~ ~~ ~~ ~~\n");
            }
            else { //jeœli derektywa to DC wypisz wartoœæ liczby
                sprintf(t, "%08X", ROW_ARRAY[i].value);
                fprintf(outputFile, "%c%c %c%c %c%c %c%c\n", t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7]);
            }
        }
    }
    fprintf(outputFile, "\n");//przerwa miêdzy sekcj¹ danych i rozkazów
    for (i = memRowAmount; i < lineAmount; i++) {
        if (ROW_ARRAY[i].byteAmount == 4) {//jeœli rozkaz zajmuje 4 bajty wypisz odpowiedni kod maszynowy
            if (ROW_ARRAY[i].arg1VAL > 16) ROW_ARRAY[i].arg1VAL %= 16;
            fprintf(outputFile, "%02X %1X%1X ", ROW_ARRAY[i].hex, ROW_ARRAY[i].arg1VAL, ROW_ARRAY[i].arg2VAL);
            sprintf(t2, "%04X", ROW_ARRAY[i].move);
            fprintf(outputFile, "%c%c %c%c\n", t2[0], t2[1], t2[2], t2[3]);
        }
        else fprintf(outputFile, "%02X %1X%1X\n", ROW_ARRAY[i].hex, ROW_ARRAY[i].arg1VAL, ROW_ARRAY[i].arg2VAL); //jeœli rozkaz zajmuje 2 bajty
    }
    fclose(outputFile);
    return;
}