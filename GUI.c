#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <curses.h>
#include "parser.h"
#include "interpreter.h"
#include "GUI.h"
#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif
#define total_width 119
#define HEIGHT 30
#define PSA_WIDTH 42
#define MSCK_WIDTH 18
#define REGISTER_WIDTH 18
#define MEMORY_WIDTH 28
#define LEVEL 9
WINDOW* top_border,*top_content;
WINDOW* psa_border, * psa_content, *psa_title;
WINDOW* msck_border, * msck_content , *msck_title;
WINDOW* memory_border, * memory_content, *memory_title;
WINDOW* register_border, * register_content, *register_title;
WINDOW* sign_border, * sign_content,*sign_title;
WINDOW* counter_border, * counter_content,*counter_title;



int PSA;
int currentMemStart = 0;

void createWinWithBorder(WINDOW** window, int h, int w, int y, int x) //tworzy ramke
{
	*window = newwin(h, w, y, x);
	box(*window, 0, 0);
	return;
}

void actualizeGUI(char order[], char memory[], int registers[], int step, int memoryAmount, int state) { //aktualizuje strone wizualna po kazdym  ruchu
	int c;
	writeMemory(memory, memoryAmount);
	writePSAandMSCK(step, order);
	refreshRegisters(registers);
	writeState(state);
	highlight(order, step, registers, state);
	wrefresh(register_content);
	wrefresh(psa_content);
	wrefresh(msck_content);
	wrefresh(memory_content);
	wrefresh(sign_content);
	wrefresh(counter_content);
	while (1) {
		c = getchar();
		if (c == (int)('n') || c == (int)('N')) break;
		if (c == (int)('q') || c == (int)('Q')) exit(NULL);
		if (c == (int)('m') || c == (int)('M')) printMemory(memory);
		if (c == (int)('1')) {
			currentMemStart = max(0, currentMemStart - 1);
			writeMemory(memory, memoryAmount);
			highlight(order, step, registers, state);
			wrefresh(memory_content);
		}
		if (c == (int)('2')) {
			currentMemStart++;
			writeMemory(memory, memoryAmount);
			highlight(order, step, registers, state);
			wrefresh(memory_content);
		}

	}
}
void writeMemory(char memory[],int memoryAmount) { //aktualizuje pamiec
	int i = 0,j=0;
	for(i = 0; i < 30; i++) {
		mvwprintw(memory_content, i, 0, "                             ");
	}
	for (i=currentMemStart; i < min(30+currentMemStart, memoryAmount / 8); i++) {
		
		mvwprintw(memory_content, i-currentMemStart, 1, "%3d: %s", j / 2, ROW_ARRAY[i].label);
		mvwprintw(memory_content, i-currentMemStart, 15, " %c%c %c%c %c%c %c%c", memory[j], memory[j + 1], memory[j + 2], memory[j + 3], memory[j + 4], memory[j + 5], memory[j + 6], memory[j + 7]);
		j += 8;
	}
}
void writePSAandMSCK(int step, char order[]) { //aktualizuje kroki kodu maszynowego i pseudoasemblera
	int i = 0;
	int j = 0;
	int q = 0;
	if (step < 15 || orderRowAmount<=30) {
		if (PSA) {
			for (i = 0; i < min(30, orderRowAmount); i++) {
				mvwprintw(psa_content, i, 0, "                                          ");
			
				mvwprintw(psa_content, i, 0, "%s", ROW_ARRAY[memRowAmount+i].label);
				mvwprintw(psa_content, i, 10, "%s", ROW_ARRAY[memRowAmount + i].order);
				mvwprintw(psa_content, i, 13, "%s", ROW_ARRAY[memRowAmount + i].arg1);
				if(ROW_ARRAY[memRowAmount + i].order[0]!='J') mvwprintw(psa_content, i, 15, ",%s", ROW_ARRAY[memRowAmount + i].arg2);
			
			}
		}
		
		for (i = 0; i < min(30, orderRowAmount); i++) {
			mvwprintw(msck_content, i, 0, "                 ");
			
			if (isDigit(order[j])) {
				mvwprintw(msck_content, i, 1, "%3d: %c%c %c%c\n",j/2, order[j], order[j + 1], order[j + 2], order[j + 3]);
				j += 4;
			}
			else {
				mvwprintw(msck_content, i, 1, "%3d: %c%c %c%c %c%c %c%c\n",j/2, order[j], order[j + 1], order[j + 2], order[j + 3], order[j + 4], order[j + 5], order[j + 6], order[j + 7]);
				j += 8;
			}
			
		}
	}
	else {
		if (orderRowAmount - step < 15) {
			if (PSA) {
				for (i = orderRowAmount-30; i < orderRowAmount; i++, q++) {
					mvwprintw(psa_content, q, 0, "                                          ");
				
					mvwprintw(psa_content, q, 0, "%s", ROW_ARRAY[memRowAmount + i].label);
					mvwprintw(psa_content, q, 10, "%s", ROW_ARRAY[memRowAmount + i].order);
					mvwprintw(psa_content, q, 13, "%s", ROW_ARRAY[memRowAmount + i].arg1);
					if (ROW_ARRAY[memRowAmount + i].order[0] != 'J') mvwprintw(psa_content, q, 15, ",%s", ROW_ARRAY[memRowAmount + i].arg2);
				
				}
			}
			
			for (i = 0; i < orderRowAmount-30; i++) {
				if (isDigit(order[j])) j += 4;
				else j += 8;
			}
			q = 0;
			for (i = orderRowAmount-30; i < orderRowAmount; i++) {
				mvwprintw(msck_content, q, 0, "                 ");
				
				if (isDigit(order[j])) {
					mvwprintw(msck_content, q, 1, "%3d: %c%c %c%c\n", j / 2, order[j], order[j + 1], order[j + 2], order[j + 3]);
					j += 4;
				}
				else {
					mvwprintw(msck_content, q, 1, "%3d: %c%c %c%c %c%c %c%c\n", j / 2, order[j], order[j + 1], order[j + 2], order[j + 3], order[j + 4], order[j + 5], order[j + 6], order[j + 7]);
					j += 8;
				}
				
				q++;
			}
		}
		else
		{
			if (PSA) {
				for (i = step-15; i < step+15; i++, q++) {
					mvwprintw(psa_content, q, 0, "                                          ");
				
					mvwprintw(psa_content, q, 0, "%s", ROW_ARRAY[memRowAmount + i].label);
					mvwprintw(psa_content, q, 10, "%s", ROW_ARRAY[memRowAmount + i].order);
					mvwprintw(psa_content, q, 13, "%s", ROW_ARRAY[memRowAmount + i].arg1);
					if (ROW_ARRAY[memRowAmount + i].order[0] != 'J') mvwprintw(psa_content, q, 15, ",%s", ROW_ARRAY[memRowAmount + i].arg2);
				
				}
			}
			
			for (i = 0; i < step - 15; i++) {
				
				if (isDigit(order[j])) j += 4;
				else j += 8;
			}
			q = 0;
			for (i = step - 15; i < step + 15; i++) {
				mvwprintw(msck_content, q, 0, "                 ");
				
				if (isDigit(order[j])) {
					mvwprintw(msck_content, q, 1, "%3d: %c%c %c%c\n", j / 2, order[j], order[j + 1], order[j + 2], order[j + 3]);
					j += 4;
				}
				else {
					mvwprintw(msck_content, q, 1, "%3d: %c%c %c%c %c%c %c%c\n", j / 2, order[j], order[j + 1], order[j + 2], order[j + 3], order[j + 4], order[j + 5], order[j + 6], order[j + 7]);
					j += 8;
				}
				
				q++;
			}
		}
	}
	highlightStep(step);
}
void highlightStep(int step) { //podswietla odpowienni wiersz w kodzie maszynowym i pseudoasemblerze
	if (step < 15 || orderRowAmount <= 30) {
		if(PSA) mvwchgat(psa_content,step, 0, 42, COLOR_PAIR(3), 0, NULL);
		else mvwchgat(psa_content, step, 0, 42, A_NORMAL, 0, NULL);
		mvwchgat(msck_content,step, 0, 18, COLOR_PAIR(3), 0, NULL);
	}
	else {
		if (orderRowAmount - step < 15) {
			if(PSA) mvwchgat(psa_content,30-(orderRowAmount-step), 0, 42, COLOR_PAIR(3), 0, NULL);
			else mvwchgat(psa_content, 30 - (orderRowAmount - step), 0, 42, A_NORMAL, 0, NULL);
			mvwchgat(msck_content,30 - (orderRowAmount - step), 0, 18, COLOR_PAIR(3), 0, NULL);
		}
		else {
			if(PSA) mvwchgat(psa_content,15, 0, 42, COLOR_PAIR(3), 0, NULL);
			else mvwchgat(psa_content, 15, 0, 42, A_NORMAL, 0, NULL);
			mvwchgat(msck_content,15, 0, 18, COLOR_PAIR(3), 0, NULL);
		}
	}
	return;
}
void refreshRegisters(int registers[]) { //aktualizuje rejestry
	int i = 0;
	char c[12];
	for (i = 0; i <= 29; i++) {
		mvwprintw(register_content, i, 0, "                  ");
	}
	for (i = 0; i <= 13; i++) {
		sprintf(c, "%08X", registers[i]);
		mvwprintw(register_content, i, 0, "                  ");
		mvwprintw(register_content, i, 1, "%2d: %c%c %c%c %c%c %c%c",i,c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
	}

	mvwprintw(register_content, 14, 1, "14: ORDERS");
	mvwprintw(register_content, 15, 1, "15: MEMORY");
	
}
void writeState(int state) { //aktualizuje znak ostatniej operacji
	if(state==0) mvwprintw(sign_content, 0, 1, "00");
	if (state == 1) mvwprintw(sign_content, 0, 1, "01");
	if (state == -1) mvwprintw(sign_content, 0, 1, "10");
	if (state >1||state<-1) mvwprintw(sign_content, 0, 1, "11");
}
void actualizeStateRegister(char order[], int index, int state) {
	int offset=4+index/2;
	int x;
	char c[14];
	if (state == 0) mvwprintw(counter_content, 0, 1, "00 00 00 00");
	if (state == 1) mvwprintw(counter_content, 0, 1, "00 00 40 00");
	if (state == -1) mvwprintw(counter_content, 0, 1, "00 00 80 00");
	if (state > 1 || state < -1) mvwprintw(counter_content, 0, 1, "00 00 C0 00");
	if (order[index] == 'E') {
		x= hexToDec4(order[index+4], order[index+5], order[index+6], order[index+7]);
		switch (order[index+1]) {
		case '0': //J
			offset = x;
			break;
		case '1': //JZ
			if (state == 0) offset = x;
			break;
		case '2': //JP
			if (state > 0) offset = x;
			break;
		case '3': //JN
			if (state < 0) offset = x;
			break;
		}
	}
	else {
		if (isDigit(order[index])) offset = 2+index/2;
		else offset = 4+index/2;
	}
	offset += memRowAmount * 4;
	sprintf(c, "%08X", offset);
	mvwprintw(counter_content,0,13, "%c%c %c%c %c%c %c%c\n", c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
}
void highlight(char order[], int step, int registers[], int state) { //podswietla odpowiednie miejsca w rejestrach i pamieci
	int i = 0,j=0;
	for (i = 0; i < step; i++) {
		if (isDigit(order[j])) j += 4;
		else j += 8;
	}
	actualizeStateRegister(order, j, state);
	if (isDigit(order[j]) == 1) {
		highlightRegister(hexToDec1(order[j+2]), 1);
		highlightRegister(hexToDec1(order[j + 3]), 2);
	}
	else {
		switch (order[j]) {
		case 'D':
			highlightRegister(hexToDec1(order[j + 2]), 1);
			if (order[j+3] == 'F') {
				highlightRegister(15, 2);
				highlightMemory(hexToDec4(order[j + 4], order[j + 5], order[j + 6], order[j + 7])/4 - currentMemStart,2);
			}
			else {
				highlightRegister(hexToDec1(order[j + 3]), 2);
				highlightMemory((registers[hexToDec1(order[j + 3])]+hexToDec4(order[j + 4], order[j + 5], order[j + 6], order[j + 7])) / 4-currentMemStart, 2);
			}
			break;
		case 'E':
			highlightRegister(14, 2);
			break;
		case 'F':
			if (order[j + 1] != '3') {
				highlightRegister(hexToDec1(order[j + 2]), 1);
				if (order[j + 3] == 'F') {
					highlightRegister(15, 2);
					highlightMemory(hexToDec4(order[j + 4], order[j + 5], order[j + 6], order[j + 7]) / 4-currentMemStart, 2);
				}
				else {
					highlightRegister(hexToDec1(order[j + 3]), 2);
					highlightMemory((registers[hexToDec1(order[j + 3])] + hexToDec4(order[j + 4], order[j + 5], order[j + 6], order[j + 7])) / 4-currentMemStart, 2);
				}
			}
			else {
				highlightRegister(hexToDec1(order[j + 2]), 2);
				if (order[j + 3] == 'F') {
					highlightRegister(15, 1);
					highlightMemory(hexToDec4(order[j + 4], order[j + 5], order[j + 6], order[j + 7]) / 4-currentMemStart, 1);
				}
				else {
					highlightRegister(hexToDec1(order[j + 3]), 1);
					highlightMemory((registers[hexToDec1(order[j + 3])] + hexToDec4(order[j + 4], order[j + 5], order[j + 6], order[j + 7])) / 4-currentMemStart, 1);
				}
			}
			break;
		}
	}
}
void highlightRegister(int n, int option) { // podswietla odpowiedni rejestr
	mvwchgat(register_content, n, 0, 18, COLOR_PAIR(option), 0, NULL);
	return;
}
void highlightMemory(int n, int option) { // podswietla odpowiednia komorke pamieci
	mvwchgat(memory_content, n, 0, 28, COLOR_PAIR(option), 0, NULL);
	return;
}

void initGUI(char memory[], char order[], int memory_amount, int order_amount, char* filename, char* filenameM, int registers[], int isPSA) { //rozpoczyna oprawe graficzna
	if (isPSA == 1) PSA = 0;
	else PSA = 1;
	initscr();
	cbreak();
	noecho();
	initCOLORS();
	resize_term(40, total_width);	
	printFrames();
	initWindows();
	
	printStaticText(filename, filenameM);
	
	refreshRegisters(registers);
	writePSAandMSCK(0, order);
	writeMemory(order, memory_amount);
	writeState(0);
	
	refreshALL();
}
void initCOLORS() { //inicjalizuje kolory
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLUE);
	init_pair(2, COLOR_YELLOW, COLOR_MAGENTA);
	init_color(COLOR_WHITE, 850, 850, 850);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);
}
void printFrames() { //drukuje ramki na ekranie
	createWinWithBorder(&top_border, 6, total_width - 40, 1, 1);
	createWinWithBorder(&counter_border, 5, 28, 2, total_width - 38);
	createWinWithBorder(&sign_border, 5, 8, 2, total_width - 9);
	createWinWithBorder(&psa_border, HEIGHT+2, PSA_WIDTH+2, 8, 1);
	createWinWithBorder(&msck_border, HEIGHT + 2, MSCK_WIDTH + 2, 8, PSA_WIDTH+4);
	createWinWithBorder(&memory_border, HEIGHT + 2, MEMORY_WIDTH + 2, 8, PSA_WIDTH+4 + MSCK_WIDTH+ 3);
	createWinWithBorder(&register_border, HEIGHT + 2, REGISTER_WIDTH + 2, 8, total_width - REGISTER_WIDTH -3 );
}
void printStaticText(char* filename, char* filenameM) { //drukuje caly tekst ktory sie nie bedzie zmienial
	mvwprintw(psa_title, 0, (PSA_WIDTH + 2 - strlen("KOD W PSEUDOASEMBLERZE")) / 2, "KOD W PSEUDOASEMBLERZE");
	mvwprintw(msck_title, 0, (MSCK_WIDTH + 2 - strlen("KOD MASZYNOWY")) / 2, "KOD MASZYNOWY");
	mvwprintw(memory_title, 0, (MEMORY_WIDTH + 2 - strlen("PAMIEC")) / 2, "PAMIEC");
	mvwprintw(register_title, 0, (REGISTER_WIDTH + 2 - strlen("REJESTRY")) / 2, "REJESTRY");
	mvwprintw(sign_title, 0, 0, "  ZNAK");
	mvwprintw(counter_title, 0, (28 - strlen("STAN PROGRAMU")) / 2, "STAN PROGRAMU");
	mvwprintw(top_content, 0, 0, "INTERPRETER PSEUDOASEMBLERA!");
	if (PSA) mvwprintw(top_content, 1, 0, "interpretuje program: %s", filename);
	else mvwprintw(top_content, 1, 0, "interpretuje program: %s", filenameM);
	mvwprintw(top_content, 2, 0, "'Q'-wyjdz z programu, 'N'-nastepny krok, 'M'-druk pamieci");
	mvwprintw(top_content, 3, 0, "'1','2'-przewijanie sekcji pamieci");
}
void initWindows() { //inicjalizuje obiekty typu WINDOW
	psa_title = newwin(1, PSA_WIDTH + 2, 7, 1);
	msck_title = newwin(1, MSCK_WIDTH + 2, 7, PSA_WIDTH+4);
	memory_title = newwin(1, MEMORY_WIDTH + 2, 7, PSA_WIDTH+ MSCK_WIDTH +7);
	register_title = newwin(1, REGISTER_WIDTH + 2, 7, total_width- REGISTER_WIDTH -3);
	sign_title = newwin(1, 8, 1, total_width - 9);
	counter_title = newwin(1, 28, 1, total_width - 38);
	top_content = newwin(4, total_width - 42, 2, 2);
	register_content = newwin(HEIGHT, REGISTER_WIDTH, LEVEL, total_width-REGISTER_WIDTH-2);
	psa_content = newwin(HEIGHT, PSA_WIDTH, LEVEL, 2);
	msck_content = newwin(HEIGHT, MSCK_WIDTH, LEVEL, PSA_WIDTH+5);
	memory_content = newwin(HEIGHT, MEMORY_WIDTH, LEVEL, PSA_WIDTH+ MSCK_WIDTH+8);
	sign_content = newwin(1, 4, 4, total_width - 7);
	counter_content = newwin(1, 25, 4, total_width - 37);
}
void refreshALL() { //odswierza potrzebne elementy
	refresh();
	wrefresh(top_border);
	wrefresh(counter_border);
	wrefresh(sign_border);
	wrefresh(psa_border);
	wrefresh(msck_border);
	wrefresh(memory_border);
	wrefresh(register_border);
	wrefresh(psa_title);
	wrefresh(msck_title);
	wrefresh(memory_title);
	wrefresh(register_title);
	wrefresh(sign_title);
	wrefresh(counter_title);
	wrefresh(top_content);
	wrefresh(register_content);
	wrefresh(psa_content);
	wrefresh(msck_content);
	wrefresh(memory_content);
	wrefresh(sign_content);
}

void endGUI() { //konczy dzalanie strony graficznej
	endwin();
}