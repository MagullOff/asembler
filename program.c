#include<stdio.h>
#include<string.h>
#include<stdlib.h>
const char* toBin(int a){
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

const char* toU2(int a){
    int b;
    char *tab;
    const char *wynik=toBin(a);
    tab = (char*)malloc(8);
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
    printf("%s\n", toU2(-127));
    return 0;
}