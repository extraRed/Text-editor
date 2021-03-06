#include<iostream>
#include<fstream>
#include<ncurses.h>

using namespace std;

const char * FILENAME = "test.txt";

int main() {
	ifstream fin(FILENAME);
	WINDOW *lister;
	int maxy , maxx;
	char ch;
	initscr();
	getmaxyx(stdscr , maxy , maxx);
	lister = newwin(maxy , maxx/2 , 0 , maxx/4);
	if(lister == NULL) {
		addstr("unable to create window\n");
		refresh();
		getch();
		endwin();
		return 0;
	}
	
	if(!fin) {
		addstr("unable to open file\n");
		refresh();
		getch();
		endwin();
		return 0;
	}

/*	do{
		fin >> ch;
		waddch(lister , ch);
		wrefresh(lister);
	}while(ch != EOF);*/

	for(int i = 0 ; i < 12 ; ++i) {
		waddch(lister , 'f');
		wrefresh(lister);
	}

	fin.close();
	getch();
	endwin();
	return 0;
}
