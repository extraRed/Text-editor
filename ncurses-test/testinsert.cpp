#include<iostream>
#include<ncurses.h>

using namespace std;

int main() {
	char ch;
	initscr();
//	noecho();
	ch = getch();
//	echo();
	refresh();
	ch = getch();
	refresh();
	noecho();
	refresh();
    getch();
	endwin();
	return 0;
}
