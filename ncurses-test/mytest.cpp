#include<iostream>
#include<fstream>
#include<ncurses.h>

using namespace std;

const char * FILENAME = "test.txt";

int main() {
	FILE * text;
	WINDOW *lister;
	int maxy , maxx;
	int ch;
	initscr();
	getmaxyx(stdscr , maxy , maxx);
	lister = newwin(0 , 0 , 0 , 0);
	if(lister == NULL) {
		addstr("unable to create window\n");
		refresh();
		getch();
		endwin();
		return 0;
	}

	text = fopen(FILENAME , "r");
	
	if(text == NULL) {
		addstr("unable to open file\n");
		refresh();
		getch();
		endwin();
		return 0;
	}

	do{
		ch = fgetc(text);
		waddch(lister , ch);
		//wrefresh(lister);
		//getch();
	}while(ch != EOF);
	getch();
	wrefresh(lister);
//	getch();
//	addstr("HEY\n");
	fclose(text);
	getch();
	endwin();
	return 0;
}
