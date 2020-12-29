#pragma once

void parse_machine_code(char* fileName, int isDebug,int psa_or_msc,char *filename1);

void count_memory(int *mem_pointer, int *order_pointer, char *fileName);

void write_to_string(char memory[], char order[], int memory_amount, int order_amount, char* fileName);

void interpret(char memory[], char order[], int memory_amount, int order_amount, int registers[], int isDebug);

int analyze4order(char c1, char c2, char c3, char c4, int registers[], int state);
int analyze8order(char c1, char c2, char c3, char c4, char c5, char c6, char c7, char c8, int registers[], char memory[], int state);

void writeInterpretedOutput(char memory[], char order[], int memory_amount, int order_amount);

int hexToDec1(char c);
int hexToDec4(char c1, char c2, char c3, char c4);
int hexToDec8(char c1, char c2, char c3, char c4, char c5, char c6, char c7, char c8);

int getStep(int i, char order[]);

int isDigit(char c);
int isDigitOrLetter(char c);

void printMemory(char memory[]);