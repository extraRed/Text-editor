#include<iostream>
#include<curses.h>

int main() {
	char ch;
	initscr();
	addstr("Type a few lines of test\n");
	addstr("Press - to quit");
	refresh();
	while(ch = getch() != '-') {
		;
	}
	endwin();
	return 0;
}
