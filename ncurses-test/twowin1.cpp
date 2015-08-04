#include<iostream>
#include<ncurses.h>

using namespace std;

int main() {
	WINDOW * two;
	initscr();
	addstr("This isthe original window , stdscr. \n");
	refresh();
	getch();

	two = newwin(0 , 0 , 0 , 0);
	if(two == NULL) {
		addstr("Unable to open window .\n");
		endwin();
		return 1;
	}
	addstr("Let's open!\n");
	refresh();
	waddstr(two , "This is the new window created.\n");

//	getch();
	wrefresh(two);
	getch();
	endwin();
	return 0;
}
