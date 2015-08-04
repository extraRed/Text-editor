#include<iostream>
#include<ncurses.h>

using namespace std;

int main() {
	char Ham1[] = "To be ot not to be is a question!\n";
	char Ham2[] = "Whether 'tis nobler in the mind to suffer\n";
	char Ham3[] = "The slings and arrows of outrageous fortuns.\n";
	char Ham4[] = "Or to take arms against a sea of troubles\n";
	char Ham5[] = "And by opposing end them?\n";

	initscr();

	addstr(Ham1);
	addstr(Ham3);
	addstr(Ham5);
	refresh();
	getch();

	move(1 , 0);
	insertln();
	addstr(Ham2);
	refresh();
	getch();

	move(3 , 0);
	insertln();
	addstr(Ham4);
	getch();

	endwin();
	return 0;
}
