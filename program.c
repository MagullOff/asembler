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
    FILE* input = NULL; //sprawdzenie poprawno�ci �cie�ki lub wczytanie jej
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
    parse(path); //funkcja dziel�ca kod na poszczeg�lne fragmenty
    getOffSets(); //funkcja obliczaj�ca przesuni�cia dla element�w kodu zawieraj�cych etykiety
    writeOutputToFile(); //funkcja wypisuj�ca kod maszynowy do pliku
    return 0;
}

