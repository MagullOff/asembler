#pragma once
void initGUI(char memory[], char order[], int memory_amount, int order_amount, char* filename, char* filenameM, int registers[], int isPSA);
void actualizeGUI(char order[], char memory[], int registers[], int step, int memoryAmount, int state);
void endGUI();