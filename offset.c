#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.h"
#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif


void getOffSets() { //funkcja obliczaj¹ca przesuniêcia dla elementów kodu zawieraj¹cych etykiety
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
