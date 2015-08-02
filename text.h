//文件名：text.h

//作者：刘敏行（1200012863） ， 姜双（1200012826）

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

// <史杨勍惟、刘敏行、姜双、陈庆英>#include<iostream>

#include<fstream>
#include<cstdio>
#include<string>
#include<cstring>
#include<vector>
#pragma once

using namespace std;

//这个类我只是随便写了一下，还有很多成员需要添加，如搜索替换以及最好开个字符串数组来表示每行的字符串，还有比如插入行和删除行也可单独写函数，任何需要的函数都可以自行添加。

class Text{
		friend class Screen;
		friend class Cursor;
	private:
		char* File_name;// 保存文件名，为保存使用
		string content;//文本原始内容
		string buffer;//缓存所有修改的内容，如果保存的话就复制到文本
		int lines;//文本的行数
		int num_of_words;//文章的字数，每个‘\n'也计一个字符
		vector<string> rows; //按照行保存文本
		vector<bool> highlights;
	public:
		void content_insert(string s , int line , int pos);//在光标位置插入文本
		void content_insert_enter(int _line, int _pos);   // insert a '\n' to create a new line or make an existing line to two lines
		void content_delete(int pos1 , int pos2);//删除多行文本
		void content_delete(int pos);			// 删除一行
		void content_delete(int pos, unsigned length);// 从某个位置开始删除一定长度的文本
		void content_delete(string s);			// 删除给定的一段文字
		void content_delete(int rowid,int rowpos1,int rowpos2);		
		int content_search(int Find[1001][3],string s);
		void getnext(string s,int next[][101],int id);
		int kmp(int ti,int id,string t,string tmp,int next[][101],int pos[][101],int cnt[]);
		int partition(string s,string tmp[],int &all,int &no,int next[][101]);	
		int one_line_length(int a);				// 返回特定一行的字符个数
		int total_lines();						// 返回总逻辑行个数
		void replace(string , string);
		Text(const char * FILENAME);//读取文件
		Text();//空文件
		~Text();
		int get_pos(int , int);
		void save();//保存
		void c_delete(int , int);
		void update_highlights(int , int(*)[3]);
		void clear_highlights();
//		void show_content();
};
