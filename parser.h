#pragma once
#define MAX_LEN_ETYKIETA 20
#define MAX_LEN_LINE 100
#define MAX_ROW_AMOUNT 1000
#define MAX_PATH_LEN 30
char row_string[MAX_ROW_AMOUNT][MAX_LEN_LINE]; //tablica przechowuj�ca w jednym rz�dzie jedn� linie kodu jako �a�cuch znak�w
int lineAmount; //ilo�� linii
int memRowAmount; //ilo�� linii w sekcji danych
int memRowAmount1;
int orderRowAmount; //ilo�� linii w sekcji rozkaz�w
int PSARowAmount;
struct ROW { //struct ten reprezentuje ka�d� linie programu i przechowywane s� w nim jej cz�ci
    int type; //0-dane 1-rozkazy
    char label[MAX_LEN_ETYKIETA];
    char order[5];
    char line[MAX_LEN_LINE];

    //rozkazy
    char arg1[MAX_LEN_ETYKIETA]; //�a�cuch pierwszego argumentu
    char arg2[MAX_LEN_ETYKIETA]; //�a�cuch drugiego argumentu
    int byteAmount; //jak du�o miejsca zajmuje kod maszynowy rozkazu
    int hex; //kod rozkazu
    int arg1VAL; //warto�� pierwszego argumentu jako liczba
    int arg2VAL; //warto�� drugiego argumentu jako liczba
    int move; //przesuni�cie

    //dane
    int amount; //ilo�� zadeklarowanych zmiennych
    int value; // warto�� w przypadku derektywy DC
    int offset; //ilo�� pami�ciu zabranej przez element

}ROW_ARRAY[MAX_ROW_AMOUNT];
struct ROW tempRow;
void clear_temp();  //"czy�cimy" struct tempRow kt�ry jest zwracany w funkcjach parsuj�cych linie. Bez tego w niekt�rych przypadkach pojawiaj� si� b��dy
struct ROW parse_memory_row(char* row); //funkcja dziel�ca linie sekcji danych na poszczeg�lne elementy
struct ROW parse_order_row(char* row); //funkcja dziel�ca linie sekcji rozkaz�w na poszczeg�lne elementy
void read_file(char* fileName); //funkcja wczytuj�ca zawarto�� pliku do tablicy 2-wymiarowej
void parse(char* fileName);

