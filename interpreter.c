#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <limits.h>
#include "GUI.h"
#include "interpreter.h"
#include "parser.h"

#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif
void parse_machine_code(char* fileName, int isDebug, int psa_or_msc,char *filename1) { //spaja cly proces interpretacji, odczytania z pliku i wypisania do pliku
	int memory_amount = 0; //ilosc zarezerwowanej pamieci
	int order_amount = 0; //ilosc pamieci na rozkazy
    int registers[16];
    int c;
    int i = 0;
    for (i = 0; i < 16; i++) {
        registers[i] = 0;
    }
    i = 0;
    count_memory(&memory_amount, &order_amount, fileName);
    
    char *memory = (char*)malloc(memory_amount * sizeof(char)+10);   
    char *order = (char*)malloc(order_amount * sizeof(char)+1);
    memory[memory_amount+8] = '\0';
    order[order_amount] = '\0';
    memRowAmount1 = memory_amount / 8;
    write_to_string(memory, order, memory_amount, order_amount, fileName);
    if (isDebug == 1) initGUI(memory, order, memory_amount, order_amount, filename1,fileName, registers,psa_or_msc);
    i = 0;
    interpret(memory, order, memory_amount, order_amount, registers, isDebug);
    
    if (isDebug) {
        while (1) {
            c = getchar();
            if (c == (int)('q') || c == (int)('Q')) {
                endGUI();
                break;
            }
            if (c == (int)('1')) {
                changeMemStart(1);
                writeMemory(memory, memory_amount);
                refreshMem();
            }
            if (c == (int)('2')) {
                changeMemStart(2);
                writeMemory(memory, memory_amount);
                refreshMem();
            }
	
        }
    }
    free(memory);
    free(order);
    return;
}
void handle_error(int n) {
    if (n == 0) {
        printf("Nastapilo dzielenie przez 0!\n");
        exit(0);
    }
    if (n == 1) {
        printf("Nastapil blad pamieci!\n");
        exit(0);
    }
    if (n == 2) {
        printf("Nastapil blad overflow!\n");
        exit(0);
    }
}
int checkAddOF(int x, int y) {
    if (x > 0 && INT_MAX - x < y || x<0 && INT_MIN - x>y) return 1;
    return 0;
}
int checkSubOF(int x, int y) {
    if ((x > 0 && y < INT_MIN + x) || (x < 0 && y - 1 > INT_MAX + x)) return 1;
    return 0;
}
int checkMulOF(int x, int y) {
    if ((x == -1 && y == INT_MIN) || (y == -1 && x == INT_MIN) || (y != 0 && (x > INT_MAX / y || x < INT_MIN / y)) ) return 1;
    return 0;
}
int checkDivOF(int x, int y) {
    if ((x == -1 && y == INT_MIN) || (y == -1 && x == INT_MIN)) return 1;
    return 0;
}
int getStep(int i, char order[]) { // sprawdza na ktorym rozkazie jestesmy
    int step = 0;
    int j = 0;
    for (; j < i; step++) {
        if (isDigit(order[j])) j += 4;
        else j += 8;
    }
    return step;
}
int isDigit(char c) { //sprawdza czy dany char to cyfra
    if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9') return 1;
    return 0;
}
int isDigitOrLetter(char c) { //sprawdza czy dany char to cyfra lub litera
    if (((int)c >= (int)('0') && (int)c <= (int)('9')) || ((int)c >= (int)('A') && (int)c <= (int)('Z')) || c == '~') return 1;
    else return 0;
}
void interpret(char memory[], char order[], int memory_amount, int order_amount, int registers[],int isDebug) { //funkcja interpretujaca kod
    int i = 0;
    int step = 0;
    int x = 0,y=0;
    int state = 0;
    while (i < order_amount-3) {
        x++;
        if(isDebug==1) actualizeGUI(order,memory,registers,getStep(i,order),memory_amount,state);
        if (order[i] == '1'||(order[i]=='3'&&order[i+1]=='1')) {
            state=analyze4order(order[i], order[i + 1], order[i + 2], order[i + 3],registers,state);
            i += 4;
        }
        else {
            y=analyze8order(order[i], order[i + 1], order[i + 2], order[i + 3], order[i + 4], order[i + 5], order[i + 6], order[i + 7],registers,memory,state);
            if (y == -1) i += 8;
            if (y < (-3)) {
                i += 8;
                state = y + 5;
            }
            if (y >= 0) i = y;
        }

    }
    if (isDebug == 1) actualizeGUI(order, memory, registers, getStep(i, order), memory_amount,state);
    writeInterpretedOutput(memory, order, memory_amount, order_amount);
    
    
}

int hexToDec1(char c) { //zamiana liczby jednocyfrowej w systemie 16 na system 10
    if ((int)(c) <= '9') return (int)(c)-(int)('0');
    else return 10 + ((int)(c)-(int)('A'));
}
int hexToDec4(char c1, char c2, char c3, char c4) {//zamiana z U2 na system 10 liczby 4cyfrowej
    int w = 0;
    w += hexToDec1(c4) + 16 * hexToDec1(c3) + 16 * 16 * hexToDec1(c2);
    if (hexToDec1(c1) >= 8) {
        
        w += (hexToDec1(c1) - 8) * 16 * 16 * 16;
        w -= 32768;
    }else w += (hexToDec1(c1)) * 16 * 16 * 16;
    return w;
}
int hexToDec8(char c1, char c2, char c3, char c4, char c5, char c6, char c7, char c8) { //zamiana z U2 na system 10 liczby 8cyfrowej
    int w = 0;
    w += hexToDec1(c8) + 16 * hexToDec1(c7) + 16 * 16 * hexToDec1(c6) + 16 * 16 * 16 * hexToDec1(c5) + 16 * 16 * 16 * 16 * hexToDec1(c4) + 16 * 16 * 16 * 16 * 16 * hexToDec1(c3)+16 * 16 * 16 * 16 * 16 * 16 * hexToDec1(c2);
    if (hexToDec1(c1) >= 8) {
        w += (hexToDec1(c1) - 8) * 16 * 16 * 16 * 16 * 16 * 16 * 16;   
        w -= 2147483648; 
    }
    else w += (hexToDec1(c1)) * 16 * 16 * 16 * 16 * 16 * 16 * 16;
    return w;
}
int analyze4order(char c1, char c2, char c3, char c4, int registers[], int state) { //funkcja analizujaca rozkaz 2-bajtowy
    int g = state;
    if (c1 == '3') { //LR
        registers[hexToDec1(c3)] = registers[hexToDec1(c4)];
    }
    else {
        switch (c2) {
            case '0': //AR
                registers[hexToDec1(c3)] += registers[hexToDec1(c4)];
                if (checkAddOF(registers[hexToDec1(c3)], registers[hexToDec1(c4)])) handle_error(2);
                g = registers[hexToDec1(c3)];
                if (g == 0) return 0;
                if (g > 0) return 1;
                if (g < 0) return -1;
                break;
            case '2': //SR
                if (checkSubOF(registers[hexToDec1(c3)], registers[hexToDec1(c4)])) handle_error(2);
                registers[hexToDec1(c3)] -= registers[hexToDec1(c4)];
                g = registers[hexToDec1(c3)];
                if (g == 0) return 0;
                if (g > 0) return 1;
                if (g < 0) return -1;
                break;
            case '4': //MR
                if (checkMulOF(registers[hexToDec1(c3)], registers[hexToDec1(c4)])) handle_error(2);
                registers[hexToDec1(c3)] *= registers[hexToDec1(c4)];
                g = registers[hexToDec1(c3)];
                if (g == 0) return 0;
                if (g > 0) return 1;
                if (g < 0) return -1;
                break;
            case '6': //DR
                if (checkDivOF(registers[hexToDec1(c3)], registers[hexToDec1(c4)])) handle_error(2);
                if (registers[hexToDec1(c4)] == 0) handle_error(0);
                registers[hexToDec1(c3)] /= registers[hexToDec1(c4)];
                g = registers[hexToDec1(c3)];
                if (g == 0) return 0;
                if (g > 0) return 1;
                if (g < 0) return -1;
                break;
            case '8': //CR
                g = registers[hexToDec1(c3)] - registers[hexToDec1(c4)];
                if (g == 0) return 0;
                if (g > 0) return 1;
                if (g < 0) return -1;
                break;
            }
    }
    
    return g;
}
int analyze8order(char c1, char c2, char c3, char c4, char c5, char c6, char c7, char c8, int registers[],char memory[],int state) { //funkcja analizujaca rozkaz 4-bajtowy
    int real_offset = hexToDec4(c5, c6, c7, c8);
    int offset = 2 * real_offset;
    int offset1 = 2 * registers[hexToDec1(c4)] + offset;
    char tab[10];
    int s1 = 0;
    int j = 0;
    int g = -1;
    
    switch (c1) {
    case 'D':
        if (c4 == 'F' && offset > memRowAmount1 * 8-7) handle_error(1);
        if (c4 != 'F' && offset1 > memRowAmount1 * 8-7) handle_error(1);
        switch (c2) {
        case '1': //A
            if (c4 == 'F') {
                if (checkAddOF(registers[hexToDec1(c3)], hexToDec8(memory[offset], memory[offset + 1], memory[offset + 2], memory[offset + 3], memory[offset + 4], memory[offset + 5], memory[offset + 6], memory[offset + 7]))) handle_error(2);
                registers[hexToDec1(c3)] += hexToDec8(memory[offset], memory[offset + 1], memory[offset + 2], memory[offset + 3], memory[offset + 4], memory[offset + 5], memory[offset + 6], memory[offset + 7]);
            }
            else {
                if (checkAddOF(registers[hexToDec1(c3)], hexToDec8(memory[offset1], memory[offset1 + 1], memory[offset1 + 2], memory[offset1 + 3], memory[offset1 + 4], memory[offset1 + 5], memory[offset1 + 6], memory[offset1 + 7]))) handle_error(2);
                registers[hexToDec1(c3)] += hexToDec8(memory[offset1], memory[offset1 + 1], memory[offset1 + 2], memory[offset1 + 3], memory[offset1 + 4], memory[offset1 + 5], memory[offset1 + 6], memory[offset1 + 7]);
            }
            break;
        case '3': //S
            if (c4 == 'F') {
                if (checkSubOF(registers[hexToDec1(c3)], hexToDec8(memory[offset], memory[offset + 1], memory[offset + 2], memory[offset + 3], memory[offset + 4], memory[offset + 5], memory[offset + 6], memory[offset + 7]))) handle_error(2);
                registers[hexToDec1(c3)] -= hexToDec8(memory[offset], memory[offset + 1], memory[offset + 2], memory[offset + 3], memory[offset + 4], memory[offset + 5], memory[offset + 6], memory[offset + 7]);
            }
            else {
                if (checkSubOF(registers[hexToDec1(c3)], hexToDec8(memory[offset1], memory[offset1 + 1], memory[offset1 + 2], memory[offset1 + 3], memory[offset1 + 4], memory[offset1 + 5], memory[offset1 + 6], memory[offset1 + 7]))) handle_error(2);
                registers[hexToDec1(c3)] -= hexToDec8(memory[offset1], memory[offset1 + 1], memory[offset1 + 2], memory[offset1 + 3], memory[offset1 + 4], memory[offset1 + 5], memory[offset1 + 6], memory[offset1 + 7]);
            }
            break;
        case '5': //M
            if (c4 == 'F') {
                if (checkMulOF(registers[hexToDec1(c3)], hexToDec8(memory[offset], memory[offset + 1], memory[offset + 2], memory[offset + 3], memory[offset + 4], memory[offset + 5], memory[offset + 6], memory[offset + 7]))) handle_error(2);
                registers[hexToDec1(c3)] *= hexToDec8(memory[offset], memory[offset + 1], memory[offset + 2], memory[offset + 3], memory[offset + 4], memory[offset + 5], memory[offset + 6], memory[offset + 7]);
            }
            else {
                if (checkMulOF(registers[hexToDec1(c3)], hexToDec8(memory[offset1], memory[offset1 + 1], memory[offset1 + 2], memory[offset1 + 3], memory[offset1 + 4], memory[offset1 + 5], memory[offset1 + 6], memory[offset1 + 7]))) handle_error(2);
                registers[hexToDec1(c3)] *= hexToDec8(memory[offset1], memory[offset1 + 1], memory[offset1 + 2], memory[offset1 + 3], memory[offset1 + 4], memory[offset1 + 5], memory[offset1 + 6], memory[offset1 + 7]);
            }
            break;
        case '7': //D
            if (c4 == 'F') {
                if (hexToDec8(memory[offset], memory[offset + 1], memory[offset + 2], memory[offset + 3], memory[offset + 4], memory[offset + 5], memory[offset + 6], memory[offset + 7]) == 0) handle_error(0);
                if (checkDivOF(registers[hexToDec1(c3)], hexToDec8(memory[offset], memory[offset + 1], memory[offset + 2], memory[offset + 3], memory[offset + 4], memory[offset + 5], memory[offset + 6], memory[offset + 7]))) handle_error(2);
                registers[hexToDec1(c3)] /= hexToDec8(memory[offset], memory[offset + 1], memory[offset + 2], memory[offset + 3], memory[offset + 4], memory[offset + 5], memory[offset + 6], memory[offset + 7]);
            }
            else {
                if(hexToDec8(memory[offset1], memory[offset1 + 1], memory[offset1 + 2], memory[offset1 + 3], memory[offset1 + 4], memory[offset1 + 5], memory[offset1 + 6], memory[offset1 + 7])==0) handle_error(0);
                if (checkDivOF(registers[hexToDec1(c3)], hexToDec8(memory[offset1], memory[offset1 + 1], memory[offset1 + 2], memory[offset1 + 3], memory[offset1 + 4], memory[offset1 + 5], memory[offset1 + 6], memory[offset1 + 7]))) handle_error(2);
                else registers[hexToDec1(c3)] /= hexToDec8(memory[offset1], memory[offset1 + 1], memory[offset1 + 2], memory[offset1 + 3], memory[offset1 + 4], memory[offset1 + 5], memory[offset1 + 6], memory[offset1 + 7]);
            }
            break;
        case '9': //C
            if (c4 == 'F') s1 = registers[hexToDec1(c3)] - hexToDec8(memory[offset], memory[offset + 1], memory[offset + 2], memory[offset + 3], memory[offset + 4], memory[offset + 5], memory[offset + 6], memory[offset + 7]);
            else s1=registers[hexToDec1(c3)] - hexToDec8(memory[offset1], memory[offset1 + 1], memory[offset1 + 2], memory[offset1 + 3], memory[offset1 + 4], memory[offset1 + 5], memory[offset1 + 6], memory[offset1 + 7]);
            if (s1 == 0) return -5;
            if (s1 > 0) return -4;
            if (s1 < 0) return -6;
            break;
        }
        if (c2 == '9') {
            if (registers[hexToDec1(c3)] == 0) return -5;
            if (registers[hexToDec1(c3)] > 0) return -4;
            if (registers[hexToDec1(c3)] < 0) return -6;
        }
        break;
    case 'E':
        switch (c2) {
        case '0': //J
            return offset;
            break;
        case '1': //JZ
            if (state == 0) return offset;
            break;
        case '2': //JP
            if (state > 0) return offset;
            break;
        case '3': //JN
            if (state < 0) return offset;
            break;
        }
        break;
    case 'F':
        switch (c2) {
        case '0': //L
            if (c4 == 'F' && offset > memRowAmount1 * 8-7) handle_error(1);
            if (c4 != 'F' && offset1 > memRowAmount1 * 8-7) handle_error(1);
            if (c4 == 'F') registers[hexToDec1(c3)] = hexToDec8(memory[offset], memory[offset + 1], memory[offset + 2], memory[offset + 3], memory[offset + 4], memory[offset + 5], memory[offset + 6], memory[offset + 7]);
            else registers[hexToDec1(c3)] = hexToDec8(memory[offset1], memory[offset1 + 1], memory[offset1 + 2], memory[offset1 + 3], memory[offset1 + 4], memory[offset1 + 5], memory[offset1 + 6], memory[offset1 + 7]);
            break;
        case '2': //LA
            registers[hexToDec1(c3)] = real_offset;
            break;
        case '3': //ST
            if (c4 == 'F' && offset > memRowAmount1 * 8-7) handle_error(1);
            if (c4 != 'F' && offset1 > memRowAmount1 * 8-7) handle_error(1);
            sprintf(tab, "%08X", registers[hexToDec1(c3)]);
            if (c4 == 'F') {
                for (j = 0; j < 8; j++) {
                    memory[offset + j] = tab[j];
                }
            }
            else {
                for (j = 0; j < 8; j++) {
                    memory[offset1 + j] = tab[j];
                }
            }
            
            break;
        }
        break;
    }
    return g;
}

void count_memory(int* mem_pointer, int* order_pointer, char* fileName) {//policzenie ilosci pamieci potrzebnej do zarezerwowania na dane i rozkazy
    int memory_amount = 0; 
    int order_amount = 0; 
    char temp_string[15];
    int i = 0;
    FILE* sourceFile;
    sourceFile = fopen(fileName, "r");
    while (fgets(temp_string, sizeof temp_string, sourceFile) != NULL) {
        if (i == 0) {
            if (temp_string[0] == ' ' || temp_string[0] == '\0' || temp_string[0] == '\n') i++;
            else memory_amount+=8;
        }
        else {
            if (strlen(temp_string) < 8) order_amount += 4;
            else order_amount += 8;
        }
    }
    fclose(sourceFile);
    *order_pointer = order_amount;
    *mem_pointer = memory_amount;
    return;
}
void write_to_string(char memory[], char order[], int memory_amount, int order_amount, char* fileName) { //przepisanie ciagow danych i rozkazow na osobne ciagi znakow
    FILE* sourceFile;
    char temp_string[15];
    sourceFile = fopen(fileName, "r");
    int i = 0, j = 0, a = 0, b = 0, x = 0;
    while (fgets(temp_string, sizeof temp_string, sourceFile) != NULL) {
        if (i == 0) {
            if (temp_string[0] == ' ' || temp_string[0] == '\0' || temp_string[0] == '\n') i++;
            else {
                for (j = 0; j < strlen(temp_string); j++) {
                    if (isDigitOrLetter(temp_string[j])) {
                        memory[a] = temp_string[j];
                        a++;
                    }
                }
            }
        }
        else {
            x++;
            for (j = 0; j < strlen(temp_string); j++) {
                if (isDigitOrLetter(temp_string[j])) {
                    order[b] = temp_string[j];
                    b++;
                }
            }
        }
    }
    orderRowAmount = x;
    fclose(sourceFile);
}
void writeInterpretedOutput(char memory[], char order[], int memory_amount, int order_amount) { //zapisanie wyniku zinterpretowanego programu
    int h = 0;
    int i = 0;
    FILE* outputFile;
    outputFile = fopen("result.txt", "w");
    for (h = 0; h < memory_amount; h += 8) {
        fprintf(outputFile, "%c%c %c%c %c%c %c%c\n", memory[h], memory[h + 1], memory[h + 2], memory[h + 3], memory[h + 4], memory[h + 5], memory[h + 6], memory[h + 7]);
    }
    fprintf(outputFile, "\n");

    while (i < order_amount - 1) {
        if (order[i] == '1' || (order[i] == '3' && order[i + 1] == '1')) {
            fprintf(outputFile, "%c%c %c%c\n", order[i], order[i + 1], order[i + 2], order[i + 3]);
            i += 4;
        }
        else {
            fprintf(outputFile, "%c%c %c%c %c%c %c%c\n", order[i], order[i + 1], order[i + 2], order[i + 3], order[i + 4], order[i + 5], order[i + 6], order[i + 7]);
            i += 8;
        }
    }
    fclose(outputFile);
}
