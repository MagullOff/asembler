#pragma once
void parse_machine_code(char* fileName);
void count_memory(int *mem_pointer, int *order_pointer, char *fileName);
void write_to_string(char memory[], char order[], int memory_amount, int order_amount, char* fileName);
void interpret(char memory[], char order[], int memory_amount, int order_amount, int registers[]);
int analyze4order(char c1, char c2, char c3, char c4, int registers[], int *s1);
int analyze8order(char c1, char c2, char c3, char c4, char c5, char c6, char c7, char c8, int registers[], char memory[], int state);
int hexToDec8(char c1, char c2, char c3, char c4, char c5, char c6, char c7, char c8);