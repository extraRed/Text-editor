//文件名：screen.h

//作者：史杨勍惟  （学号：1200012741）

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
#include<cstdio>
#include"cursor.h"
#include"text.h"
#pragma once

class Screen{
	private:
		int mode;//模式，1表示只读，2表示读写
		Text tx;//文本
		Cursor cur;//光标
		int k_count;//移动计数器
		int sy , sx;//屏幕大小参数
		pair<int , string> string_register;//搜索时需要的字符串寄存器
		ofstream fout;//输出文本，调试用
	public:
		Screen(Text _tx , int choice , string out);
		void init();//初始化
		~Screen();
		int get_key();//获得用户按键
		void get_act(int);//对按键的响应
		void move_curse(char);//移动光标
		void move_curse_to(int , int);//移动光标
		void s_scroll(char);//滚动屏幕
		void go_insert_mode(char);//进入插入模式
		void go_delete_mode(char);//进入删除模式
		void save();//保存
		void exit_scr();//退出
		void show_text(int p = 0);//显示文本
		void show_yx();//显示光标位置
		void inserting();//插入模式中
		void exit_insert_mode();//退出插入模式
		void go_replace_mode();//替换
		void go_search_mode(char);//搜索
		void print_invalid();//告知错误操作
};
