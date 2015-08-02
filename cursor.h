//文件名：Cursor.h

//作者：陈庆英  （学号：1200012768）

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

#include<iostream>
#include<fstream>
#include<cstdio>
#include"text.h"
#pragma once
class Cursor{
	friend class Screen;
	friend class Text;
	private:
		int y;					//显示中纵坐标
		int show_y;				//屏幕中纵坐标
		int x;					//屏幕中横坐标
		int text_pos;			//整个文本中的位置
		int pos;				//Text类中的位置
		int print_y;			//起始输出字符的纵坐标
		int print_x;			//起始输出字符的横坐标（为0，辅助变量）
		int print_line;			//起始输出字符所在的逻辑行
		int print_pos;			//起始输出字符所在的逻辑行中的位置
		int print_len;			//起始输出字符所在的逻辑行的长度
		int scr_y;				//屏幕的高
		int scr_x;				//屏幕的宽
		int cur_line_logic;		//当前逻辑行
		int len_line_logic;		//当前行的长度,每次使用都要更新（向text要）
		int total_line_logic;	//总逻辑行数,每次使用都要更新（向text要）
		int max_y;				//显示行总数
		ofstream fout;			//作调试用
	public:
		Cursor(int , int , int, int, int, string);	//构造函数
		~Cursor(){fout.close();}
		void move(char , Text&);					//移动:h, j, k, l, 0, $, (, )
		void move_to(int _y , int _x , Text&);		//移动到指定位置
		void c_scroll(char , Text&);				//翻页：^B, ^U, ^F, ^D
		void one_line_up(Text&);					//对(可能)上移一行的情况（h, k), 更新第一个输出字符的位置
		void one_line_dn(Text&);					//对(可能)下移一行的情况（l, j), 更新第一个输出字符的位置
		void page_up(int&, int&, int&, int&, int, bool , Text&);	//改变翻页时的光标位置及输出的第一个字符的位置
		void page_dn(int&, int&, int&, int&, int, bool , Text&, bool);
		void go_head(Text&);						//当到逻辑行头时，更新第一个输出字符的位置
		void go_rear(Text&);						//当到逻辑行尾时，更新第一个输出字符的位置
		void change_size(int, int);					//设定窗口的大小
		void cal_max_y(Text&);						//计算显示行总数
};
