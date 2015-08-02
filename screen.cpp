//文件名：screen.cpp

//作者: 史杨勍惟 （学号：1200012741）

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

#include"screen.h"
#include<iostream>
#include<cstdlib>
#include<ncurses.h>
#include<cstdio>

Screen::Screen(Text _tx , int choice , string out):cur(0,0,0,0,0, "cur.txt") , tx(_tx) , mode(choice) , fout(out.c_str()){}

Screen::~Screen(){
	fout.close();
}

void Screen::init(){
	
	//模式初始化
	k_count = 0;
	initscr();
	raw();
	noecho();
	scrollok(stdscr , TRUE);
	keypad(stdscr , TRUE);
	mousemask(ALL_MOUSE_EVENTS,NULL);	
	string_register.first = 0;
	
	//窗口初始化
	getmaxyx(stdscr , sy , sx);
	cur.change_size(sy-1 , sx);
	
	//颜色初始化
	start_color();
	init_pair(2 , COLOR_WHITE , COLOR_BLUE);//文本颜色
	init_pair(1 , COLOR_WHITE , COLOR_BLACK);//高亮颜色
	init_pair(3 , COLOR_RED , COLOR_BLACK);//末行颜色
	
	refresh();
	refresh();
	show_text(0);
	refresh();
	
	//获取按键，开始响应
	int ch = get_key();
	refresh();
	get_act(ch);
}

void Screen::save(){

	//保存文本
	tx.save();
	move(sy-1 , 0);

	//提示保存成功
	clrtoeol();
	attrset(COLOR_PAIR(3));
	printw("Save Completed!");
	move(cur.show_y , cur.x);
	refresh();
}

int Screen::get_key(){
	
	//获取按键
	int ch = getch();
	return ch;
}

void Screen::move_curse_to(int y , int x){
	
	//移动光标到指定位置，用于鼠标点击
	cur.move_to(y , x , tx);
	int cur_pos = tx.get_pos(cur.print_line , cur.print_pos);
	show_text(cur_pos);
	move(cur.show_y , cur.x);
	refresh();
}

void Screen::move_curse(char c){
	
	//前进到下一个word
	if(c == 'w'){
		while(tx.rows[cur.cur_line_logic][cur.pos] != ' ' && cur.pos < tx.rows[cur.cur_line_logic].length()-2) move_curse('l');
		move_curse('l');
		refresh();
		return;
	}

	//前进到上一个word
	if(c == 'W'){
		while(tx.rows[cur.cur_line_logic][cur.pos] != ' ' && cur.pos > 1) move_curse('h');
		move_curse('h');
		refresh();
		return;
	}

	//到第k列
	if(c == '$' && k_count != 0){
		if(k_count > tx.rows[cur.cur_line_logic].length()-1) print_invalid();
		else{
			move_curse('0');
			move_curse('l');
		}
		k_count = 0;
		return;
	}

	//到第k行
	if(c == 'G') {
		if(k_count > tx.lines-1){
			print_invalid();
			return;
		}
		move_curse('(');
		int line = k_count;
		k_count = 0;
		while(cur.cur_line_logic != line) cur.move('j' , tx);
		move(cur.show_y , cur.x);
		int cur_pos = tx.get_pos(cur.print_line, cur.print_pos);
		show_text(cur_pos);
		return;
	}

	//搜索中的移动
	if(c == 'N' || c == 'n') {
		int e;
		if(c == 'N') e = -1;
		else e = 1;
		if(string_register.first == 0) print_invalid();
		else{
			int pre_op = string_register.first;
			string s = string_register.second;
			int direction = e*pre_op;
			int cur_pos = tx.get_pos(cur.cur_line_logic , cur.pos);
			int Find[1001][3];
			tx.content_search(Find , s);
			int pos = cur_pos;
			int end_pos = tx.buffer.length();
			while(tx.highlights[pos]) pos += direction;
			bool find_flag = false;
			int distance;
			for( ; pos < end_pos && pos >= 0 ; pos += direction) {
				if(tx.highlights[pos]) {
					distance = pos-cur_pos;
					find_flag = true;
					break;
				}
			}
			if(!find_flag){
				cur_pos = tx.get_pos(cur.print_line , cur.print_pos);
				show_text(cur_pos);
				move(cur.show_y , cur.x);
				refresh();
				move(sy-1 , 0);
				clrtoeol();
				attrset(COLOR_PAIR(3));
				printw("No string fits ");
				if(direction == -1) printw("before");
				else printw("after");
				printw(" this position");
				refresh();
			}else{
				if (distance < 0) distance = -distance;
				for(int i = 0 ; i < distance ; ++i) {
					if(direction == 1) cur.move('l' , tx);
					else cur.move('h' , tx);
				}
				cur_pos = tx.get_pos(cur.print_line , cur.print_pos);
				show_text(cur_pos);
				move(cur.show_y , cur.x);
				refresh();
			}
		}
		return;
	}

	//其他的移动，调用cur类的成员函数，再更新屏幕
	refresh();
	for(int i = 0 ; i < k_count+(!k_count) ; ++i) cur.move(c , tx);
	move(cur.show_y , cur.x);
	int cur_pos = tx.get_pos(cur.print_line, cur.print_pos);
	show_text(cur_pos);
	refresh();
}

void Screen::s_scroll(char x){

	//滚动翻页，调用cursor的成员函数
	cur.c_scroll(x , tx);
	int cur_pos = tx.get_pos(cur.print_line , cur.print_pos);
	show_text(cur_pos);
	refresh();
}

void Screen::get_act(int c){
	
	//人机交互的核心接口，对用户输入的相应
	
	refresh();
	bool flag = true;
	if(c >= '1' && c <= '9' || (c == '0' && k_count!=0)){
		k_count *= 10;
		k_count += c - '0';
		refresh();
	    char ch = getch();
		get_act(ch);
		return;
	}

	//键盘特殊按键到普通按键的映射
	switch(c){
		case KEY_UP:
			c = 'k';break;
		case KEY_DOWN:
			c = 'j';break;
		case KEY_LEFT:
			c = 'h';break;
		case KEY_RIGHT:
			c = 'l';break;
		case KEY_PPAGE:
			c = char(21);break;
		case KEY_NPAGE:
			c = char(4);break;
		case KEY_HOME:
			c = '0';break;
		case KEY_END:
			c = '$';break; 
		case KEY_DC:
			c = 'x';break;
		case KEY_BACKSPACE:
			c = 'h';break;
		case KEY_F(2):
			c = char(12);break;
		case KEY_IC:
			c = 'i';break;
	}

	//对不同的按键进行不同的操作
	switch(c){
		refresh();

		//鼠标点击
		case KEY_MOUSE:
			MEVENT mort;
			getmouse(&mort);
			move_curse_to(mort.y , mort.x);
			break;

		//进入插入模式
		case 'I':
		case 'i':
		case 'O':
		case 'o':
		case 'A':
		case 'a':
			if(mode == 1){
				print_invalid();
				break;
			}
			tx.clear_highlights();
			show_text(tx.get_pos(cur.print_line , cur.print_pos));
			go_insert_mode(c);
			break;

		//移动
		case 'h':
		case 'j':
		case 'k':
		case 'l':
		case '(':
		case ')':
		case '0':
		case '$':
		case 'N':
		case 'n':
		case 'w':
		case 'W':
		case 'G':
			move_curse(c);
			break;

		//保存
		case char(19):
			save();
			break;
		
		//退出
		case 'q':
			refresh();
			exit_scr();
			flag = false;
			break;

		//翻页
		case char(2):
		case char(21):
		case char(6):
		case char(4):
			refresh();
			s_scroll(c);
			break;
		
		//删除
		case 'd':
		case 'D': 
		case 'x':
			if(mode == 1){
				print_invalid();
				break;
			}
			tx.clear_highlights();
			show_text(tx.get_pos(cur.print_line , cur.print_pos));
			go_delete_mode(c);
			break;
		
		//搜索
		case '?':case '/':
			if(c == '/') string_register.first = 1;
			else string_register.first = -1;
			go_search_mode(c);
			break;
		
		//替换
		case 's' :
			if(mode == 1) {
				print_invalid();
				break;
			}
			go_replace_mode();
			break;
		
		//取消高亮
		case char(12):
			tx.clear_highlights();
			show_text(tx.get_pos(cur.print_line , cur.print_pos));
			break;
		
		default:
			print_invalid();
	}
	
	k_count = 0;
	refresh();

	if(flag) {
		int ch = getch();
		get_act(ch);
	}
}

void Screen::go_insert_mode(char c){
	//进入插入模式
	refresh();
	int ny = cur.show_y;
	int nx = cur.x;

	//初始移动
	switch(c){
		case 'i':
			break;
		case 'I':
			move_curse('0');
			break;
		case 'a':
			move_curse('l');
			break;
		case 'A':
			move_curse('$');
			break;
		case 'o':
			move_curse('$');
			refresh();
			tx.content_insert_enter(cur.cur_line_logic , cur.pos);
			move_curse('l');
			refresh();
			break;
		case 'O':
			move_curse('0');
			move_curse('h');
			tx.content_insert_enter(cur.cur_line_logic , cur.pos);
			move_curse('l');
			break;
	}
	refresh();
	
	//开始插入
	inserting();
}


void Screen::show_text(int p){

	//更新屏幕
	clear();
	string s = tx.buffer;
	int pos;
	int y = 0 , x = 0;

	//从Text类得到buffer字符串成员
	for(pos = p ; pos < s.length() && y < sy-1; ++pos){
		if(tx.highlights[pos]) attrset(COLOR_PAIR(2));
		else attrset(COLOR_PAIR(1));
		char inchar = s[pos];
		addch(inchar);
		getyx(stdscr , y , x);
	}
	refresh();

	//处理光标位置
	move(cur.show_y , cur.x);
	show_yx();
	move(cur.show_y , cur.x);
	refresh();
}


void Screen::exit_scr(){
	//退出
//	printw("you");
//	getch();
	refresh();
	endwin();
}

void Screen::show_yx() {

	//末行显示光标位置
	int px = cur.pos , py = cur.cur_line_logic;
	move(sy-1 , 0);
	clrtoeol();
	attrset(COLOR_PAIR(3));
	printw("row %d , col %d" , py , px);
	refresh();
}

void Screen::go_delete_mode(char c){
	fout << "HERE" << endl;
	fout << tx.rows.empty() << endl;	
	//对删除操作的响应
	if(tx.rows.empty()) {
	//	fout << "HEY" << endl;
	//	printw("HERE");
	//	refresh();
	//	getch();
		return;
	}
	switch(c) {
		
		//删除一个字符
		case 'x':
			cout << endl;
			tx.c_delete(cur.cur_line_logic , cur.pos);
			cout << endl;
			show_text(tx.get_pos(cur.print_line , cur.print_pos));
			break;
		
		//删除一整行
		case 'D':
			if(tx.lines == 1){
				tx.content_delete(0);
				clear();
				move_curse_to(0 , 0);
			}else{
				if(cur.cur_line_logic == 0){
					while(cur.cur_line_logic == 0)
					  move_curse('j');
					tx.content_delete(1);
					move_curse('(');
				}else{
					int d_line = cur.cur_line_logic;
					while(cur.cur_line_logic == d_line) 
					  move_curse('k');
					tx.content_delete(d_line+1);
					move_curse('0');
				}
				show_text(tx.get_pos(cur.print_line , cur.print_pos));
			}
			break;
			break;
		
		//删除多个字符，多行
		case 'd':
			int d_length = 0;
			int ch;
			while(ch = getch()){
				if(ch >= '0' && ch <= '9'){
					d_length*=10;
					d_length += ch-'0';	
				}else{
					if(ch == 'h' || ch == 'l'){
						int o_line = cur.cur_line_logic;
						int o_pos = cur.pos;
						for(int i = 0 ; i < d_length && cur.cur_line_logic==o_line ; ++i){
							move_curse(ch);
						}
						if(ch == 'h') tx.content_delete(o_line , cur.pos , o_pos);
						else tx.content_delete(o_line , o_pos , cur.pos);
						if(ch == 'l'){
							while(cur.pos != o_pos) move_curse('h');
						}
					} else if (ch == 'k' || ch == 'j') {
						for(int i = 0 ; i < d_length && cur.cur_line_logic >= 0 && cur.cur_line_logic < tx.lines ; ++i){
							go_delete_mode('D');
							if(ch == 'j') move_curse('j');
						}
					} else {
						print_invalid();
					}
					if(tx.buffer.empty()){
						clear();
					}
					else show_text(tx.get_pos(cur.print_line , cur.print_pos));
					break;
				}
			}
			break;
	}
}

void Screen::inserting(){
	//插入模式中的操作
	int t_pos = cur.pos;
	int ch = getch();

	//ESC退出以及特殊按键，如方向键
	while (ch != 27){
		switch(ch) {
			case KEY_UP:
				move_curse('k');
				break;
			case KEY_DOWN:
				move_curse('j');
				break;
			case KEY_LEFT:
				move_curse('h');
				break;
			case KEY_RIGHT:
				move_curse('l');
				break;
			case KEY_BACKSPACE:
				move_curse('h');
				go_delete_mode('x');
				break;	
			case KEY_DC:
				go_delete_mode('x');
				break;
			case KEY_HOME:
				move_curse('0');
				break;
			case KEY_END:
				move_curse('$');
				break;
			case KEY_NPAGE:
				s_scroll(char(4));
				break;
			case KEY_PPAGE:
				s_scroll(char(21));
				break;
			case 10:
				tx.content_insert_enter(cur.cur_line_logic , cur.pos);
				move_curse('l');
				break;

			//普通按键，插入字符	
			default:
				string s;
				s += ch;
				tx.content_insert(s , cur.cur_line_logic , cur.pos);
				move_curse('l');
				int cur_pos = tx.get_pos(cur.print_line , cur.print_pos);
				show_text(cur_pos);
		}
		ch = getch();
	}
		
	exit_insert_mode();	
}

void Screen::exit_insert_mode() {
	noecho();
}

void Screen::go_replace_mode(){

	//替换模式
	int cur_pos = tx.get_pos(cur.print_line , cur.print_pos);

	//提示
	move(sy-1 , 0);
	clrtoeol();
	attrset(COLOR_PAIR(3));
	printw("old: ");
	refresh();
	echo();
	char s[1000];
	getstr(s);
	noecho();
	show_text(cur_pos);
	move(cur.show_y , cur.x);
	move(sy-1 , 0);
	clrtoeol();
	attrset(COLOR_PAIR(3));
	printw("new: ");
	refresh();
	echo();
	char t[1000];
	getstr(t);
	noecho();
	refresh();
	string old_s = s , new_s = t;
	
	//调用Text类的替换函数
	tx.replace(old_s , new_s);
	
	//更新屏幕
	show_text(0);
	move(sy-1 , 0);
	
	//提示完成
	clrtoeol();
	attrset(COLOR_PAIR(3));
	printw("Replace Completed!");
	move(0 , 0);
	cur.move('(' , tx);
	refresh();
	noecho();
}

void Screen::go_search_mode(char c){

	//搜索模式
	int direction;
	if(c == '/') direction = 1;
	else direction = -1;

	//提示，导引
	move(sy-1 , 0);
	clrtoeol();
	attrset(COLOR_PAIR(3));
	printw("input your string: ");
	refresh();
	echo();
	char s[1000];
	getstr(s);
	string_register.second = s;
	noecho();
	int cur_pos = cur.text_pos;
	int Find[1001][3];
	
	//调用Text类的搜索函数，得到高亮位置
	tx.content_search(Find , s);
	int pos = cur_pos;
	int end_pos = tx.buffer.length();

	//寻找下一个高亮位置
	while(tx.highlights[pos]) pos += direction;
	bool find_flag = false;
	int distance;
	for( ; pos < end_pos && pos >= 0 ; pos += direction) {
		if(tx.highlights[pos]) {
			distance = pos-cur_pos;
			find_flag = true;
			break;
		}
	}


	if(!find_flag){

		//没有找到
		cur_pos = tx.get_pos(cur.print_line , cur.print_pos);
		show_text(cur_pos);
		move(cur.show_y , cur.x);
		refresh();
		move(sy-1 , 0);
		clrtoeol();
		attrset(COLOR_PAIR(3));
		printw("No string fits ");
		if(c == '?') printw("before");
		else printw("after");
		printw(" this position");
		move(cur.show_y , cur.x);
		refresh();
	}else{

		//找到
		if (distance < 0) distance = -distance;
		for(int i = 0 ; i < distance ; ++i) {
			if(direction == 1) cur.move('l' , tx);
			else cur.move('h' , tx);
		}
		cur_pos = tx.get_pos(cur.print_line , cur.print_pos);
		show_text(cur_pos);
		move(cur.show_y , cur.x);
		refresh();
	}
}

void Screen::print_invalid(){

	//提示操作有误
	move(sy-1 , 0);
	clrtoeol();
	attrset(COLOR_PAIR(3));
	printw("Invalid Operation");
	move(cur.show_y , cur.x);
	refresh();
}
