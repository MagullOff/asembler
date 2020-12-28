#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.h"
#include "interpreter.h"
#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif

void getOffSets(); //funkcja obliczaj¹ca przesuniêcia dla elementów kodu zawieraj¹cych etykiety
void getOrderHex(char c1, char c2);

void read_file(char* fileName) { //odczytuje plik i ka¿d¹ liniê wstawia do osobnego rzêdu w kwadratowej tablicy charów
    char temp_string[MAX_LEN_LINE];
    int i = 0;
    FILE* sourceFile;
    sourceFile = fopen(fileName, "r");
    while (fgets(temp_string, sizeof temp_string, sourceFile) != NULL) {
        if (temp_string[0] != '/') { //nie chcemy braæ pod uwagê komentarzy
            strcpy(row_string[i], temp_string);
            i += 1;
        }
        lineAmount = i;
    }
    fclose(sourceFile);
    return;
}
void parse(char* fileName) {
    int i = 0;
    int j;
    int z;
    read_file(fileName);
    while (row_string[i][0] != '\n') {
        ROW_ARRAY[i] = parse_memory_row(row_string[i]); //dzielimy ka¿d¹ linie a¿ do pierwszej pustej która oznacza koniec sekcji danych
        i++;
    }
    memRowAmount = i;
    orderRowAmount = lineAmount - memRowAmount - 1;
    j = i;
    z = i;
    i++;

    while (i < lineAmount) {
        ROW_ARRAY[i - 1] = parse_order_row(row_string[i]); //dzielimy kad¿d¹ linie kodu do jego koñca (jest to sekcja rozkazów)
        i++;
    }
    lineAmount--;
    getOffSets(); //funkcja obliczaj¹ca przesuniêcia dla elementów kodu zawieraj¹cych etykiety
    return;
}

void clear_temp() { //"czyœcimy" struct tempRow który jest zwracany w funkcjach parsuj¹cych linie. Bez tego w niektórych przypadkach pojawiaj¹ siê b³êdy
    memset(tempRow.label, 0, sizeof(tempRow.label));
    memset(tempRow.order, 0, sizeof(tempRow.order));
    memset(tempRow.line, 0, sizeof(tempRow.line));
    memset(tempRow.arg1, 0, sizeof(tempRow.arg1));
    memset(tempRow.arg2, 0, sizeof(tempRow.arg2));

    return;
}

struct ROW parse_memory_row(char* row) { //funkcja dziel¹ca linie sekcji danych na poszczególne elementy
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
    if (row[0] != ' ' && row[i] != '\t') { //jeœli etykieta istnieje, wczytujemy j¹
        while (row[i] != ' ' && row[i] != '\t') {
            tempRow.label[i] = row[i];
            i++;
        }
    }
    j = 0;
    while (row[i] == ' ' || row[i] == '\t') i++;

    tempRow.order[0] = row[i]; //wczytujemy derektywe
    tempRow.order[1] = row[++i];
    i++;
    while (row[i] == ' ' || row[i] == '\t') i++;
    j = 0;
    //sprawdzamy czy deklarujemy wiêcej ni¿ jedn¹ zmienn¹ oraz jakie s¹ jej ewentualne wartoœci w przypadku derektywy DC
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

struct ROW parse_order_row(char* row) { //funkcja dziel¹ca linie sekcji rozkazów na poszczególne elementy
    int row_len = strlen(row);
    int i = 0;
    int j = 0;
    char arg2C[10];
    char moveC[10];
    int q = 0;
    tempRow.type = 1;
    clear_temp();
    strcpy(tempRow.line, row);
    if (row[0] != ' ' && row[i] != '\t') {  //jeœli etykieta istnieje, wczytujemy j¹
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
    //poni¿ej przypisujemy argumenty w postaci ³añcuchów znaków do opowiednich pól
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
    //przypisujemy odpowiedznie wartoœci liczbowe do argumentów nie bêd¹cych etykietami
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