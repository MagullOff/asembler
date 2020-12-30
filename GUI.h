#pragma once
//funkcje potrzebne do rozpoczecia pracy srodowiska graficzego
void initGUI(char memory[], char order[], int memory_amount, int order_amount, char* filename, char* filenameM, int registers[], int isPSA);
void refreshALL();
void initWindows();
void printStaticText(char* filename, char* filenameM);
void printFrames();
void initCOLORS();
//funkcje potrzebne do zmiany obrazu w zaleznosci od interpretowanej linii
void actualizeGUI(char order[], char memory[], int registers[], int step, int memoryAmount, int state);
void refreshRegisters(int registers[]);
void writeMemory(char memory[], int memoryAmount);
void highlightStep(int step);
void writePSAandMSCK(int step, char order[]);
void writeState(int state);
void highlightRegister(int n, int option);
void highlightMemory(int n, int option);
void highlight(char order[], int step, int registers[], int state);
//funkcja integrujace plik interpreter.c z GUI.c
void endGUI();
void refreshMem();
void changeMemStart(int n);