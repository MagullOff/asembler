#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.h"
#include "interpreter.h"
#include "GUI.h"
#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif

void writeOutputToFile(); //funkcja wypisuj¹ca kod maszynowy do pliku
void getOffSets(); //funkcja obliczaj¹ca przesuniêcia dla elementów kodu zawieraj¹cych etykiety

int main(int argc, char* argv[]) {
    FILE* input = NULL; //sprawdzenie poprawnoœci œcie¿ki lub wczytanie jej
    char path[MAX_PATH_LEN];
    char path1[MAX_PATH_LEN];
    int psa_or_msc=0; //0-psa 1-msc
    int isDebug = 0;
    if (argc == 1) {
        printf("prosze podac sciezke sciezke:\n");
        scanf("%s", path);
    }
    else {
        strcpy(path, argv[1]);
        if (argc > 2) {
            if (strcmp("psa_code", argv[2]) == 0) psa_or_msc = 0;
            if (strcmp("msck_code", argv[2]) == 0) psa_or_msc = 1;
        }
        if (argc > 3 && strcmp("debug", argv[3]) == 0) isDebug = 1;
    }

    while ((input = fopen(path, "r")) == NULL) {
        perror("Podany plik nie jest poprawny!\n");
        printf("sprobuj ponownie: \n");
        scanf("%s", path);
        
    }
    fclose(input);

    if (psa_or_msc == 0) {
        parse(path);
        getOffSets();
        writeOutputToFile();
        strcpy(path1, path);
        memset(path, 0, sizeof(path));
        strcpy(path, "output.txt");
    }
    parse_machine_code(path,isDebug,psa_or_msc,path1); 
    

    printf("Program zakonczyl dzialanie sukcesem!\n");
    
    return 0;
}

