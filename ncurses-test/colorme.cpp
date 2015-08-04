#include<iostream>
#include<ncurses.h>

using namespace std;

int main() {
	initscr();
	start_color();

	init_pair(1 , COLOR_BLACK , COLOR_RED);
	init_pair(2 , COLOR_BLUE , COLOR_BLACK);
	attrset(COLOR_PAIR(1));
	addstr("My name is Mr. Black!\n");
	attrset(COLOR_PAIR(2));
	addstr("My name is Mr. Blue!\n");
	attrset(COLOR_PAIR(1));
	addstr("How do you do?\n");
	attron(A_BOLD);
	addstr("What");
	attroff(A_BOLD);
	addch('?');
	refresh();
	getch();
	endwin();
	return 0;
}
