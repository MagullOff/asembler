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
WINDOW* top_border,*top_content;
WINDOW* psa_border, * psa_content, *psa_title;
WINDOW* msck_border, * msck_content , *msck_title;
WINDOW* memory_border, * memory_content, *memory_title;
WINDOW* register_border, * register_content, *register_title;
WINDOW* sign_border, * sign_content,*sign_title;
WINDOW* counter_border, * counter_content,*counter_title;



int PSA;
void refreshRegisters(int registers[]) {
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
void createWinWithBorder(WINDOW** window, int h, int w, int y, int x)
{
	*window = newwin(h, w, y, x);
	box(*window, 0, 0);
	return;
}
void writeMemory(char memory[],int memoryAmount) {
	int i = 0,j=0;
	for (; i < min(30, memoryAmount / 8); i++) {
		mvwprintw(memory_content, i, 0, "                             ");
		mvwprintw(memory_content, i, 1, "%3d: %s", j / 2, ROW_ARRAY[i].label);
		mvwprintw(memory_content, i, 15, " %c%c %c%c %c%c %c%c", memory[j], memory[j + 1], memory[j + 2], memory[j + 3], memory[j + 4], memory[j + 5], memory[j + 6], memory[j + 7]);
		j += 8;
	}
}
void highlightStep(int step) {
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
void writePSAandMSCK(int step, char order[]) {
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
void writeState(int state) {
	if(state==0) mvwprintw(sign_content, 0, 1, "00");
	if (state == 1) mvwprintw(sign_content, 0, 1, "01");
	if (state == -1) mvwprintw(sign_content, 0, 1, "10");
	if (state >1||state<-1) mvwprintw(sign_content, 0, 1, "11");
}
void highlightRegister(int n, int option) {
	mvwchgat(register_content, n, 0, 18, COLOR_PAIR(option), 0, NULL);
	return;
}
void highlightMemory(int n, int option) {
	mvwchgat(memory_content, n, 0, 28, COLOR_PAIR(option), 0, NULL);
	return;
}
void highlight(char order[], int step, int registers[]) {
	int i = 0,j=0;
	for (i = 0; i < step; i++) {
		if (isDigit(order[j])) j += 4;
		else j += 8;
	}
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
				highlightMemory(hexToDec4(order[j + 4], order[j + 5], order[j + 6], order[j + 7])/4,2);
			}
			else {
				highlightRegister(hexToDec1(order[j + 3]), 2);
				highlightMemory((registers[hexToDec1(order[j + 3])]+hexToDec4(order[j + 4], order[j + 5], order[j + 6], order[j + 7])) / 4, 2);
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
					highlightMemory(hexToDec4(order[j + 4], order[j + 5], order[j + 6], order[j + 7]) / 4, 2);
				}
				else {
					highlightRegister(hexToDec1(order[j + 3]), 2);
					highlightMemory((registers[hexToDec1(order[j + 3])] + hexToDec4(order[j + 4], order[j + 5], order[j + 6], order[j + 7])) / 4, 2);
				}
			}
			else {
				highlightRegister(hexToDec1(order[j + 2]), 2);
				if (order[j + 3] == 'F') {
					highlightRegister(15, 1);
					highlightMemory(hexToDec4(order[j + 4], order[j + 5], order[j + 6], order[j + 7]) / 4, 1);
				}
				else {
					highlightRegister(hexToDec1(order[j + 3]), 1);
					highlightMemory((registers[hexToDec1(order[j + 3])] + hexToDec4(order[j + 4], order[j + 5], order[j + 6], order[j + 7])) / 4, 1);
				}
			}
			break;
		}
	}
}
	
void actualizeGUI(char order[], char memory[], int registers[], int step, int memoryAmount, int state) {
	int c;
	writeMemory(memory, memoryAmount);
	writePSAandMSCK(step, order);
	refreshRegisters(registers);
	writeState(state);
	highlight(order, step, registers);
	wrefresh(register_content);
	wrefresh(psa_content);
	wrefresh(msck_content);
	wrefresh(memory_content);
	wrefresh(sign_content);
	while (1) {
		c = getchar();
		if (c == (int)('n')) break;
		if (c == (int)('q')) exit(NULL);
	}
}
void initGUI(char memory[], char order[], int memory_amount, int order_amount, char* filename,char *filenameM, int registers[], int isPSA) {
	if (isPSA == 1) PSA = 0;
	else PSA = 1;
	initscr();
	cbreak();
	noecho();
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLUE);
	init_pair(2, COLOR_YELLOW, COLOR_MAGENTA);
	init_color(COLOR_WHITE, 850, 850, 850);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);
	resize_term(40,total_width);
	createWinWithBorder(&top_border, 6, total_width-40, 1, 1);
	createWinWithBorder(&counter_border, 5, 28, 2, total_width - 40 + 2);
	createWinWithBorder(&sign_border, 5, 8, 2, total_width - 40 + 2 + 29);
	createWinWithBorder(&psa_border, 32, 44, 8, 1);
	createWinWithBorder(&msck_border, 32, 20, 8, 46);
	createWinWithBorder(&memory_border, 32, 30, 8, 46+16+1+2+2);
	createWinWithBorder(&register_border, 32, 20 , 8, 46 + 16 + 1+30+2+1+2);

	psa_title = newwin(1, 44, 7, 1);
	msck_title = newwin(1, 16, 7, 46);
	memory_title = newwin(1, 30, 7, 46 + 16 + 1+2+2);
	register_title = newwin(1, 20, 7, 46 + 16 + 1 + 30 + 1+2+2);
	sign_title = newwin(1, 8, 1, total_width - 40 + 2 + 29);
	counter_title = newwin(1, 28, 1, total_width - 40 + 2);
	top_content = newwin(4, total_width - 42, 2, 2);
	register_content = newwin(30, 18, 9, 46 + 16 + 1 + 30 + 2+2+2);
	psa_content = newwin(30, 42, 9, 2);
	msck_content = newwin(30, 18, 9, 47);
	memory_content = newwin(30, 28, 9, 68);
	sign_content = newwin(1, 4, 4, total_width - 40 + 2 + 29 + 2);
	mvwprintw(psa_title, 0, (44 - strlen("KOD W PSEUDOASEMBLERZE")) / 2, "KOD W PSEUDOASEMBLERZE");
	mvwprintw(msck_title, 0, (20 - strlen("KOD MASZYNOWY")) / 2, "KOD MASZYNOWY");
	mvwprintw(memory_title, 0, (30 - strlen("PAMIEC")) / 2, "PAMIEC");
	mvwprintw(register_title, 0, (20 - strlen("REJESTRY")) / 2, "REJESTRY");
	mvwprintw(sign_title, 0,0, "  ZNAK");
	mvwprintw(counter_title, 0, (28 - strlen("STAN PROGRAMU")) / 2, "STAN PROGRAMU");
	mvwprintw(top_content, 0, 0, "INTERPRETER PSEUDOASEMBLERA!");
	if(PSA) mvwprintw(top_content, 1, 0, "interpretuje program: %s", filename);
	else mvwprintw(top_content, 1, 0, "interpretuje program: %s", filenameM);
	mvwprintw(top_content, 2, 0, "'q'-wyjdz z programu, 'n'-nastepny krok, 't'-wierszowany tutorial");
	refresh();
	refreshRegisters(registers);
	writePSAandMSCK(0,order);
	writeMemory(order,memory_amount);
	writeState(0);
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
void endGUI() {
	endwin();
}