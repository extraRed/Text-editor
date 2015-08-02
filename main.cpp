//文件名：main.cpp

//作者：史杨勍惟 （学号：1200012741）

// 我真诚地保证：
// 我自己独立地完成了整个程序从分析、设计到编码的所有工作。
// 如果在上述过程中，我遇到了什么困难而求教于人，那么，我将在程序实习报告中
// 详细地列举我所遇到的问题，以及别人给我的提示。
// 在此，我感谢我的队友对我的启发和帮助。下面的报告中，我还会具体地提到
// 他们在各个方法对我的帮助。
// 我的程序里中凡是引用到其他程序或文档之处，
// 例如教材、课堂笔记、网上的源代码以及其他参考书上的代码段,
// 我都已经在程序的注释里很清楚地注明了引用的出处。
// 我从未抄袭过别人的程序，也没有盗用别人的程序，
// 不管是修改式的抄袭还是原封不动的抄袭。
// 我编写这个程序，从来没有想过要去破坏或妨碍其他计算机系统的正常运转。

// <史杨勍惟、刘敏行、姜双、陈庆英>

#include"text.h"
#include"cursor.h"
#include"screen.h"
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<string>
#include<ncurses.h>

int main(int argc , char *argv[]){
	
	char * FILENAME = argv[argc-1];
	
	cout << "Welcome to using our simple editor!" << endl;//进入欢迎页面
	int choice;
	if(argc == 2){
		cout << "Choose a Mode(1 reading , 2 editting , 3 quit): ";
		cin >> choice;
		cin.get();
		while(choice != 1 && choice != 2){
			if(choice == 3) {
				cout << "Thank you for your using!" << endl;
				cin.get();
				return 0;
			} else {
				cout << "Invalid Input" << endl;
				cout << "Choose a Mode(1 reading , 2 editting , 3 quit): ";
				cin >> choice;
				cin.get();
			}
		}
	} else {
			char option = argv[1][1];
			if(option == 'r') choice = 1;
			else if(option == 'e') choice = 2;
			else{
				cout << "OPTION ERROR" << endl;
				return 0;
			}
	}
	if(choice == 1) cout << "You are in Reading " << FILENAME << endl;
	else if(choice == 2) cout << "You are in Editting " << FILENAME << endl;
	
	Text text(FILENAME);//初始化文本类
	
	cout << "Press Enter To Continue" << endl;
	cin.get();
	Screen scr(text , choice , "output.txt");//初始化窗口
	scr.init();
//	char ch = scr.get_key();
	scr.exit_scr();//退出窗口
	cout << "Thank you for your using!" << endl;//结束运行
	
	system("rm output.txt cur.txt");
	
	return 0;
}
