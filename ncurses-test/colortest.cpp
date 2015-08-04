#include<iostream>
#include<stdlib.h>
#include<string>
#include<ncurses.h>

using namespace std;

void bomb(char * msg);
int main() {
	initscr();
	if (!has_colors) bomb("Terminal cannot do colors\n");
	if(start_color() != OK) {
		bomb("Unable to start colors\ni");
	}
	printw("Colors have been proper;y initialized.\n");
	printw("Congratulations!\n");
	printw("Ncurses reports that you can use %d colors.\n" , 
				COLORS);
	refresh();
	getch();
	endwin();
	return 0;
}

void bomb(char * msg) {
	endwin();
	puts(msg);
	exit(1);
 }
