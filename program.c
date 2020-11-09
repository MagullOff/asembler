#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.h"
#include "write.h"
#include "offset.h"
#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif


int main(int argc, char** argv) {
    FILE* input = NULL; //sprawdzenie poprawnoœci œcie¿ki lub wczytanie jej
    char path[MAX_PATH_LEN];
    char* outName = "output.txt";
    if (argc == 1) {
        printf("Brak sciezki!\nPodaj sciezke:\n");
        if (scanf("%s", &path, MAX_PATH_LEN) == 0)
        {
            perror("");
            exit(EXIT_FAILURE);
        }
    }
    else {
        if (strcpy(path, MAX_PATH_LEN * sizeof(char), argv[1]) != 0) {
            perror("");
            exit(EXIT_FAILURE);
        }
    }

    while (fopen(&input, path, "r") != 0) {
        printf("Plik %s nie istnieje!\nPodaj sciezke ponownie:\n", path);
        if (scanf("%s", &path, MAX_PATH_LEN) == 0) {
            perror("");
            exit(EXIT_FAILURE);
        }
    }
    parse(path); //funkcja dziel¹ca kod na poszczególne fragmenty
    getOffSets(); //funkcja obliczaj¹ca przesuniêcia dla elementów kodu zawieraj¹cych etykiety
    writeOutputToFile(); //funkcja wypisuj¹ca kod maszynowy do pliku
    return 0;
}

