#include<iostream>
#include<ncurses.h>

using namespace std;

int main() {
	char first[24];
	char last[32];
	initscr();
	addstr("What is your first name? ");
	refresh();
	getstr(first);
	addstr("What is your last name? ");
	refresh();
	getstr(last);
	printw("Nice to meet you %s %s!" , first , last);
	refresh();
	getch();
	endwin();
	return 0;
}
