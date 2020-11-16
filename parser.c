#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.h"
#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif

void getOrderHex(char c1, char c2);
int isDigit(char c) { //sprawdza czy dany char to cyfra
    if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9') return 1;
    return 0;
}
void read_file(char* fileName) { //odczytuje plik i każdą linię wstawia do osobnego rzędu w kwadratowej tablicy charów
    char temp_string[MAX_LEN_LINE];
    int i = 0;
    FILE* sourceFile;
    sourceFile = fopen(fileName, "r");
    while (fgets(temp_string, sizeof temp_string, sourceFile) != NULL) {
        if (temp_string[0] != '/') { //nie chcemy brać pod uwagę komentarzy
            strcpy(row_string[i], temp_string);
            i += 1;
        }
        lineAmount = i;
    }
    return;
}
void parse(char* fileName) {
    int i = 0;
    int j;
    int z;
    read_file(fileName);
    while (row_string[i][0] != '\n') {
        ROW_ARRAY[i] = parse_memory_row(row_string[i]); //dzielimy każdą linie aż do pierwszej pustej która oznacza koniec sekcji danych
        i++;
    }
    memRowAmount = i;
    orderRowAmount = lineAmount - memRowAmount - 1;
    j = i;
    z = i;
    i++;
    
    while (i < lineAmount) {
        ROW_ARRAY[i - 1] = parse_order_row(row_string[i]); //dzielimy kadżdą linie kodu do jego końca (jest to sekcja rozkazów)
        i++;
    }
    lineAmount--;
    
    return;
}

void clear_temp() { //"czyścimy" struct tempRow który jest zwracany w funkcjach parsujących linie. Bez tego w niektórych przypadkach pojawiają się błędy
    strncpy(tempRow.label, "", strlen(tempRow.label));
    strncpy(tempRow.order, "", strlen(tempRow.order));
    strncpy(tempRow.line, "", strlen(tempRow.line));
    strncpy(tempRow.arg1, "", strlen(tempRow.arg1));
    strncpy(tempRow.arg2, "", strlen(tempRow.arg2));
    return;
}

struct ROW parse_memory_row(char* row) { //funkcja dzieląca linie sekcji danych na poszczególne elementy
    int row_len = strlen(row);
    int i = 0;
    int j = 0;
    int q = 0;
    char amountC[10];
    char valueC[10];
    tempRow.type = 0;
    memset(amountC, 0, sizeof(amountC));
    memset(valueC, 0, sizeof(valueC));
    clear_temp();
    strcpy(tempRow.line, row);
    if (row[0] != ' ' && row[i] != '\t') { //jeśli etykieta istnieje, wczytujemy ją
        while (row[i] != ' ' && row[i] != '\t') {
            tempRow.label[i] = row[i];
            i++;
        }
    }
    j = 0;
    while (row[i] == ' ' ||  row[i] == '\t') i++;
 
    tempRow.order[0] = row[i]; //wczytujemy derektywe
    tempRow.order[1] = row[++i];
    i++;
    while (row[i] == ' ' || row[i] == '\t') i++;
    j = 0;
    //sprawdzamy czy deklarujemy więcej niż jedną zmienną oraz jakie są jej ewentualne wartości w przypadku derektywy DC
    if (row[i] == 'I') {
        tempRow.amount = 1;
        amountC[0] = '1';
    }
    else {
        while (row[i] != '*') {
            amountC[j] = row[i];
            i++;
            j++;
        }
    }
    tempRow.amount = atoi(amountC);
    while (row[i] != 'R') i++;
    if (tempRow.order[1] == 'C') {
        i += 2;
        while (row[i] != ')') {
            valueC[q] = row[i];
            i++;
            q++;
        }
        tempRow.value = atoi(valueC);
    }
    else tempRow.value = 0;
    tempRow.offset = 4 * tempRow.amount;
    return tempRow;
}

struct ROW parse_order_row(char* row) { //funkcja dzieląca linie sekcji rozkazów na poszczególne elementy
    int row_len = strlen(row);
    int i = 0;
    int j = 0;
    char arg2C[10];
    char moveC[10];
    int q = 0;
    tempRow.type = 1;
    clear_temp();
    strcpy(tempRow.line, row);
    if (row[0] != ' ' && row[i] != '\t') {  //jeśli etykieta istnieje, wczytujemy ją
        while (row[i] != ' ' && row[i] != '\t') {
            tempRow.label[i] = row[i];
            i++;
        }
    }
    while (row[i] == ' ' || row[i] == '\t') i++;
    tempRow.order[0] = row[i]; //wczytujemy rozkaz
    tempRow.order[1] = row[++i];
    i++;
    while (row[i] == ' ' || row[i] == '\t') i++;
    //poniżej przypisujemy argumenty w postaci łańcuchów znaków do opowiednich pól
    if (tempRow.order[0] == 'J') {
        j = 0;
        while (row[i] != ' ' && row[i] != '\n' && row[i] != '\0' && row[i] != '\t' && row[i] != '\\') {
            tempRow.arg1[j] = row[i];
            j++;
            i++;
        }

    }
    else {
        j = 0;
        while (row[i] != ',' && row[i] != ' ' && row[i] != '\n' && row[i] != '\0' && row[i] != '\t') {
            tempRow.arg1[j] = row[i];
            j++;
            i++;
        }

        j = 0;
        while (row[i] == ',' || row[i] == ' ') i++;
        while (row[i] != ' ' && row[i] != '\n' && row[i] != '\0' && row[i] != '\t' && row[i] != '\\') {
            tempRow.arg2[j] = row[i];
            j++;
            i++;
        }
    }
    i = 0;
    getOrderHex(tempRow.order[0], tempRow.order[1]); //przypisujemy odpowieni kod rozkazu
    if (tempRow.order[0] != 'J') tempRow.arg1VAL = atoi(tempRow.arg1);
    else  tempRow.arg1VAL = 0;
    //przypisujemy odpowiedznie wartości liczbowe do argumentów nie będących etykietami
    if (tempRow.order[1] == 'R') tempRow.arg2VAL = atoi(tempRow.arg2);
    if (tempRow.order[0] == 'J') {
        tempRow.arg2VAL = 14;
        if (isDigit(tempRow.arg1[0])) {
            for (j = 0; tempRow.arg1[j] != ' ' && tempRow.arg1[j] != '('; j++) {
                arg2C[j] = tempRow.arg1[j];
            }
            tempRow.move = atoi(arg2C);
           

        }
        else {
            tempRow.move = 0;
        }
    }
    if (tempRow.order[1] != 'R' && tempRow.order[0] != 'J') {
        tempRow.arg2VAL = 15;
        if (isDigit(tempRow.arg2[0])) {
            for (j = 0; tempRow.arg2[j] != ' ' && tempRow.arg2[j] != '('; j++) {
                arg2C[j] = tempRow.arg2[j];
            }
            tempRow.move = atoi(arg2C);
            while (tempRow.arg2[j] == ' ' || tempRow.arg2[j] == '(') j++;
            q = 0;
            for (; tempRow.arg2[j] != ' ' && tempRow.arg2[j] != ')'; j++) {
                moveC[q] = tempRow.arg2[j];
                q++;
            }
            tempRow.arg2VAL = atoi(moveC);
        }
        else {
            tempRow.move = 0; 
        }
        
    }

    if (tempRow.order[1] == 'R') tempRow.byteAmount = 2;
    else tempRow.byteAmount = 4;
    return tempRow;
}