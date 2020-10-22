#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX_LEN_ETYKIETA 30
#define MAX_LEN_LINE 400
#define MAX_ROW_AMOUNT 100
int line_amount;
int mem_line_amount;
char out[10];
struct MEMORY_ROW{
    char etykieta[MAX_LEN_ETYKIETA];
    char rozkaz[3];
    int amount;
    char amountC[10];
    int value;
    char valueC[10];
    char line[MAX_LEN_ETYKIETA];
    char valueOut[20];
}MEMORY_ARRAY[MAX_ROW_AMOUNT];

struct ORDER_ROW{
    char etykieta[MAX_LEN_ETYKIETA];
    char rozkaz[3];
    char argument1[MAX_LEN_ETYKIETA];
    char argument2[MAX_LEN_ETYKIETA];
    char line[MAX_LEN_LINE];
}ORDER_ARRAY[MAX_ROW_AMOUNT];

struct MEMORY_ROW mem_temp;
struct ORDER_ROW or_temp;
void clear_mem_temp(){
    strncpy(mem_temp.etykieta,"",strlen(mem_temp.etykieta));
    strncpy(mem_temp.rozkaz,"",strlen(mem_temp.rozkaz));
    strncpy(mem_temp.line,"",strlen(mem_temp.line));
    strncpy(mem_temp.valueC,"",strlen(mem_temp.valueC));
    strncpy(mem_temp.amountC,"",strlen(mem_temp.amountC));
    return;
}
void clear_or_temp(){
    strncpy(or_temp.etykieta,"",strlen(or_temp.etykieta));
    strncpy(or_temp.rozkaz,"",strlen(or_temp.rozkaz));
    strncpy(or_temp.line,"",strlen(or_temp.line));
    strncpy(or_temp.argument1,"",strlen(or_temp.argument1));
    strncpy(or_temp.argument2,"",strlen(or_temp.argument2));
    return;
}
struct ORDER_ROW divide_order_row(char *row){
    
    int row_len = strlen(row);
    int i=0;
    int j=0;
    clear_or_temp();
    strcpy(or_temp.line,row);
    if(row[0]!=' '){
        while(row[i]!=' '){
            or_temp.etykieta[i]=row[i];
            i++;
        }
    }
    while(row[i]==' ') i++;
    while(row[i]!=' '){
        or_temp.rozkaz[j]=row[i];
        j++;
        i++;
    }
    while(row[i]==' ') i++;
    j=0;
    

    return or_temp;
}
struct MEMORY_ROW divide_memory_row(char *row){
    
    int row_len = strlen(row);
    int i=0;
    int j=0;
    int q=0;
    clear_mem_temp();
    strcpy(mem_temp.line,row);
    if(row[0]!=' '){
        while(row[i]!=' '){
            mem_temp.etykieta[i]=row[i];
            i++;
        }
    }
    while(row[i]==' ') i++;
    while(row[i]!=' '){
        mem_temp.rozkaz[j]=row[i];
        j++;
        i++;
    }
    while(row[i]==' ') i++;
    j=0;
    if(row[i]=='I'){
        mem_temp.amount=1;
        mem_temp.amountC[0]='1';
    } 
    else{
        while(row[i]!='*'){
            mem_temp.amountC[j]=row[i];
            i++;
            j++;
        }
    }
    mem_temp.amount=atoi(mem_temp.amountC);
    while(row[i]!='R') i++;
    if(mem_temp.rozkaz[1]=='C'){
        i+=2;
        while(row[i]!=')'){
            mem_temp.valueC[q]=row[i];
            i++;
            q++;
        }
        mem_temp.value=atoi(mem_temp.valueC);
    }else mem_temp.value=128;
    return mem_temp;
}
void wczytaj(char *fileName){
    FILE * sourceFile;
    sourceFile = fopen(fileName,"r");
    char line[MAX_LEN_LINE];
        //printf("dupa");
    for(int i=0;!feof(sourceFile);i++){
        strncpy(line, "",strlen(line));
        fgets(line,MAX_LEN_LINE,sourceFile);
        if(line[0]=='\n') break;
        MEMORY_ARRAY[i] = divide_memory_row(line);
        line_amount=i+1;
        mem_line_amount=i+1;
    }
    //printf("dupa");
    /*
    line_amount++;
    //printf("%d\n",line_amount);
    for(int i=0;!feof(sourceFile);i++){
        strncpy(line, "",strlen(line));
        fgets(line,MAX_LEN_LINE,sourceFile);
        ORDER_ARRAY[i] = divide_order_row(line);
        line_amount++;
    }*/
    return;
}
char *toBin(int a){
    char *tab;
    int b;
    int pom;
    tab = (char*)malloc(8);
    for(int i=0;a>0;i++){
        tab[i]=a%2+'0';
        a/=2;
        b=i;
    }
    for(int i=0;i<b/2;i++){
        pom=tab[i];
        tab[i]=tab[b-i];
        tab[b-i]=pom;
    }
    return tab;
}

char *toU2(int a){
    int b;
    char *tab;
    const char *wynik=toBin(a);
    tab = (char*)malloc(8);
    if(a==128) return "~~~~~~~~";
    if(a==-128) return "10000000";
    if(a>=0){
        b=strlen(toBin(a));
        for(int i=0;i<8-b;i++){
            tab[i]='0';
        }
        tab = strcat(tab,wynik);
        return tab;
    }else{
        tab[0]='1';
        b=strlen(toBin(a+128));
        wynik=toBin(a+128);
        for(int i=1;i<8-b;i++){
            tab[i]='0';
        }
        tab = strcat(tab,wynik);
        return tab;
    }
}
int main(){
    
    wczytaj("asembler.txt");
    
    for(int i=0;i<mem_line_amount;i++){
        printf("etykieta:%s rozkaz:%s ilosc:%d out:%s\n",MEMORY_ARRAY[i].etykieta,MEMORY_ARRAY[i].rozkaz,MEMORY_ARRAY[i].amount,toU2(MEMORY_ARRAY[i].value));
    }
    
    printf("============================\n");
    /*
    for(int i=0;i<line_amount-mem_line_amount;i++){
        printf("%s///%s///\n",ORDER_ARRAY[i].etykieta,ORDER_ARRAY[i].rozkaz);
    }
    */

    return 0;
}