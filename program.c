#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.h"
#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif

void writeOutput() {
    int i = 0;
    int j = 0;
    char t[15];
    char t2[6];
    for (; i < memRowAmount; i++) {
        j = 0;
        for (; j < ROW_ARRAY[i].amount; j++) {
            if (ROW_ARRAY[i].order[1] == 'S') {
                printf("~~ ~~ ~~ ~~\n");
            }
            else {
                sprintf(t, "%08X\n", ROW_ARRAY[i].value);
                printf("%c%c %c%c %c%c %c%c\n", t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7]);
            }
        }
    }
    puts("");
    for (i = memRowAmount; i < lineAmount; i++) {
            if (ROW_ARRAY[i].byteAmount == 4) {
                printf("%02X %01X%01X ", ROW_ARRAY[i].hex, ROW_ARRAY[i].arg1VAL, ROW_ARRAY[i].arg2VAL);
                sprintf(t2, "%04X", ROW_ARRAY[i].move);
                printf("%c%c %c%c\n", t2[0], t2[1], t2[2], t2[3]);
            }

            else
                printf("%02X %01X%01X\n", ROW_ARRAY[i].hex, ROW_ARRAY[i].arg1VAL, ROW_ARRAY[i].arg2VAL);
        }
        return;
}

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
        }else fprintf(outputFile, "%02X %01X%01X\n", ROW_ARRAY[i].hex, ROW_ARRAY[i].arg1VAL, ROW_ARRAY[i].arg2VAL);
    }
    return;
}

void getOffSets() {
    int i = memRowAmount;
    int j;
    for (; i < lineAmount; i++) {
        if (ROW_ARRAY[i].order[0] == 'J' && !isDigit(ROW_ARRAY[i].arg1[0])) {
            ROW_ARRAY[i].move = 0;
            for (j = memRowAmount; j < lineAmount; j++) {
                if (strcmp(ROW_ARRAY[j].label, ROW_ARRAY[i].arg1) == 0) {
                    break;
                }
                else {
                    ROW_ARRAY[i].move += ROW_ARRAY[j].byteAmount;
                }
            }
        }
        if (ROW_ARRAY[i].order[0] != 'J' && ROW_ARRAY[i].order[1] != 'R' && !isDigit(ROW_ARRAY[i].arg2[0])) {
            ROW_ARRAY[i].move = 0;
            for (j = 0; j < memRowAmount; j++) {
                if (strcmp(ROW_ARRAY[j].label, ROW_ARRAY[i].arg2) == 0) {
                    break;
                }
                else {
                    ROW_ARRAY[i].move += ROW_ARRAY[j].amount * 4;
                }
            }
        }

    }
    return;
}

int main(int argc, char** argv) {
    FILE* input = NULL;
    char path[MAX_PATH_LEN];
    char* outName = "output.txt";
    if (argc == 1) {
        printf("Brak sciezki!\nPodaj sciezke:\n");
        if (scanf("%s", &path, MAX_PATH_LEN) == 0)
        {
            perror("");
            exit(EXIT_FAILURE);
        }
    }
    else {
        if (strcpy(path, MAX_PATH_LEN * sizeof(char), argv[1]) != 0) {
            perror("");
            exit(EXIT_FAILURE);
        }
    }

    while (fopen(&input, path, "r") != 0) {
        printf("Plik %s nie istnieje!\nPodaj sciezke ponownie:\n", path);
        if (scanf("%s", &path, MAX_PATH_LEN) == 0) {
            perror("");
            exit(EXIT_FAILURE);
        }
    }
    parse(path);
    getOffSets();
    writeOutputToFile();
    puts("wypluwam kod maszynowy");
    writeOutput();
    
    puts("");
    i = 0;
    puts("wypluwam efekt parsera");
    for (; i < memRowAmount; i++) {
        printf("%2d || %20s || %2s || %4d || %4d ||\n", i, ROW_ARRAY[i].label, ROW_ARRAY[i].order, ROW_ARRAY[i].amount, ROW_ARRAY[i].value);
    }

    puts("");
    for (; i < lineAmount; i++) {
        if (ROW_ARRAY[i].byteAmount == 4)
            printf("%2d || %20s || %2s || %20s ||%20s||\n", i, ROW_ARRAY[i].label, ROW_ARRAY[i].order, ROW_ARRAY[i].arg1, ROW_ARRAY[i].arg2);
        else
            printf("%2d || %20s || %2s || %20s ||%20s||\n", i, ROW_ARRAY[i].label, ROW_ARRAY[i].order, ROW_ARRAY[i].arg1, ROW_ARRAY[i].arg2);
    }
    
    
    
    return 0;
}