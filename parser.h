#pragma once
#define MAX_LEN_ETYKIETA 20
#define MAX_LEN_LINE 100
#define MAX_ROW_AMOUNT 1000
#define MAX_PATH_LEN 30

struct ROW { //struct ten reprezentuje ka¿d¹ linie programu i przechowywane s¹ w nim jej czêœci
    int type; //0-dane 1-rozkazy
    char label[MAX_LEN_ETYKIETA];
    char order[5];
    char line[MAX_LEN_LINE];

    //rozkazy
    char arg1[MAX_LEN_ETYKIETA]; //³añcuch pierwszego argumentu
    char arg2[MAX_LEN_ETYKIETA]; //³añcuch drugiego argumentu
    int byteAmount; //jak du¿o miejsca zajmuje kod maszynowy rozkazu
    int hex; //kod rozkazu
    int arg1VAL; //wartoœæ pierwszego argumentu jako liczba
    int arg2VAL; //wartoœæ drugiego argumentu jako liczba
    int move; //przesuniêcie

    //dane
    int amount; //iloœæ zadeklarowanych zmiennych
    int value; // wartoœæ w przypadku derektywy DC
    int offset; //iloœæ pamiêciu zabranej przez element

}ROW_ARRAY[MAX_ROW_AMOUNT];

struct ROW tempRow; 
char row_string[MAX_ROW_AMOUNT][MAX_LEN_LINE]; //tablica przechowuj¹ca w jednym rzêdzie jedn¹ linie kodu jako ³añcuch znaków
int lineAmount; //iloœæ linii
int memRowAmount; //iloœæ linii w sekcji danych
int orderRowAmount; //iloœæ linii w sekcji rozkazów
void clear_temp();  //"czyœcimy" struct tempRow który jest zwracany w funkcjach parsuj¹cych linie. Bez tego w niektórych przypadkach pojawiaj¹ siê b³êdy
struct ROW parse_memory_row(char* row); //funkcja dziel¹ca linie sekcji danych na poszczególne elementy
struct ROW parse_order_row(char* row); //funkcja dziel¹ca linie sekcji rozkazów na poszczególne elementy
void read_file(char* fileName); //funkcja wczytuj¹ca zawartoœæ pliku do tablicy 2-wymiarowej
void parse(char* fileName); 