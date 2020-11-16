#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.h"
#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif

void getOrderHex(char c1, char c2) { // dla danego rozkazu zwracamy jego kod zapisany w systemie dziesiêtnym. Na system hexadecymalny zamieniamy go przy wypisywaniu
    if (c1 == 'A' && c2 == 'R') {
        tempRow.hex = 16;
        return;
    }
    if (c1 == 'A') {
        tempRow.hex = 209;
        return;
    }
    if (c1 == 'S' && c2 == 'R') {
        tempRow.hex = 18;
        return;
    }
    if (c1 == 'S' && c2 == 'T') {
        tempRow.hex = 243;
        return;
    }
    if (c1 == 'S') {
        tempRow.hex = 211;
        return;
    }
    if (c1 == 'M' && c2 == 'R') {
        tempRow.hex = 20;
        return;
    }
    if (c1 == 'M') {
        tempRow.hex = 213;
        return;
    }
    if (c1 == 'D' && c2 == 'R') {
        tempRow.hex = 22;
        return;
    }
    if (c1 == 'D') {
        tempRow.hex = 215;
        return;
    }
    if (c1 == 'C' && c2 == 'R') {
        tempRow.hex = 24;
        return;
    }
    if (c1 == 'C') {
        tempRow.hex = 217;
        return;
    }
    if (c1 == 'J' && c2 == 'Z') {
        tempRow.hex = 225;
        return;
    }
    if (c1 == 'J' && c2 == 'P') {
        tempRow.hex = 226;
        return;
    }
    if (c1 == 'J' && c2 == 'N') {
        tempRow.hex = 227;
        return;
    }
    if (c1 == 'J') {
        tempRow.hex = 224;
        return;
    }
    if (c1 == 'L' && c2 == 'R') {
        tempRow.hex = 49;
        return;
    }
    if (c1 == 'L' && c2 == 'A') {
        tempRow.hex = 242;
        return;
    }
    if (c1 == 'L') {
        tempRow.hex = 240;
        return;
    }
    return;
}