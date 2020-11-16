#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.h"
#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif

void writeOutputToFile(); //funkcja wypisuj¹ca kod maszynowy do pliku
void getOffSets(); //funkcja obliczaj¹ca przesuniêcia dla elementów kodu zawieraj¹cych etykiety

int main(int argc, char *argv[]) {
    FILE* input = NULL; //sprawdzenie poprawnoœci œcie¿ki lub wczytanie jej
    char path[MAX_PATH_LEN];
    if (argc == 1) {
        printf("prosze podac sciezke sciezke:\n");
        scanf("%s", path);
    }
    else {
        strcpy(path, argv[1]);
    }

    while ((input = fopen(path, "r")) == NULL) {
        perror("Podany plik nie jest poprawny!\n");
        printf("sprobuj ponownie: \n");
        scanf("%s", path);
    }
    parse(path); //funkcja dziel¹ca kod na poszczególne fragmenty
    getOffSets(); //funkcja obliczaj¹ca przesuniêcia dla elementów kodu zawieraj¹cych etykiety
    writeOutputToFile(); //funkcja wypisuj¹ca kod maszynowy do pliku
    return 0;
}

