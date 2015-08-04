#include<iostream>
#include<ncurses.h>
#include<string>
#include<fstream>

using namespace std;

ofstream fout("output.txt");

int main(){
	initscr();
	noecho();
	raw();
//	char endc = getch();
//	printw("%d\n" , endc);
//	printw("%d\n" , char(4) == endc);
//	addstr("BEGIN\n");
	refresh();
	char ch;
//	string buffer;
	while(true){
		ch = getch();
//		printw("%d\n" , ch);
//		printw("is %d\n" , ch == '^D');
//		fout << ch << '%';
		addch(ch);
		if(ch == char(4)) break;
		refresh();
	}
	refresh();
	endwin();
	fout.close();
	return 0;
}
