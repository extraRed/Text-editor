//文件名：Cursor.cpp

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

#include"cursor.h"
#include<string>
#include<ncurses.h>
Cursor::Cursor(int _y = 0, int _x = 0, int _text_pos = 0, int _line = 0, int _pos = 0, string str = "cur.txt"):fout(str.c_str())
{
	y = _y , x = _x, show_y = 0;
	text_pos = _text_pos;
	cur_line_logic = _line;
	pos = _pos;
	print_y = 0, print_x = 0;
	print_line = 0, print_pos = 0;
}
void Cursor::change_size(int sy, int sx)			//设置屏幕的大小
{
	scr_y = sy, scr_x = sx;
}
void Cursor::move(char c , Text& tx){
	total_line_logic = tx.lines;					//从Text类获得当前逻辑行的长度
	len_line_logic = tx.rows[cur_line_logic].length();//从Text类获得总逻辑行数
	switch(c)
	{
		case 'h':									//左移一个字符
			--text_pos;
			if (x > 0)
				--x, --pos;
			else if (pos == 0)						//到上一个逻辑行
			{
				if (cur_line_logic != 0)
				{
					--cur_line_logic;
					len_line_logic = tx.rows[cur_line_logic].length();
					pos = len_line_logic - 1;		//\n也算在len_line_logic里面，但应该移到\n的原位置
					x = pos % scr_x;
					--y;
					--show_y;
				}
			}
			else if (y != 0)						//逻辑行不变，移到上一显示行
			{
				--pos;
				x = scr_x - 1;
				--y;
				--show_y;
			}
			if (show_y < 0)							//更新光标的位置
				show_y = 0;
			one_line_up(tx);						//更新第一个输出字符的位置
			break;
		case 'j':									//下移一显示行
			if (x + len_line_logic - pos <= scr_x)	//应当移到下一个逻辑行
			{
				if (cur_line_logic != total_line_logic - 1)
				{
					text_pos += len_line_logic - pos;
					++cur_line_logic;
					len_line_logic = tx.rows[cur_line_logic].length();
					if (x < len_line_logic)	
					{
						pos = x;
						++y;
						++show_y;
					}
					else
					{
						x = len_line_logic - 1;
						pos = x;
						++y;
						++show_y;
					}
					text_pos += pos;
				}
			}
			else										//逻辑行不变，移到下一显示行
			{
				text_pos -= pos;
				if (pos + scr_x >= len_line_logic)		//下一显示行太短，则到行尾
				{
					pos = len_line_logic - 1;
					x = pos % scr_x;
					++y;
					++show_y;
				}
				else									//直接下移
				{
					pos += scr_x;
					++y;
					++show_y;
				}
				text_pos += pos;
			}
			if (show_y >= scr_y)
				show_y = scr_y - 1;
			one_line_dn(tx);					//更新第一个输出字符的位置
			break;
		case 'k':								//上移一显示行
			if (x < pos)
			{
				text_pos -= scr_x;
				pos -= scr_x;
				--y;
				--show_y;
			}
			else								//应当到上一个逻辑行
			{
				if (cur_line_logic != 0)
				{
					text_pos -= pos;
					--cur_line_logic;
					len_line_logic = tx.rows[cur_line_logic].length();	
					if ((len_line_logic - 1) % scr_x >= x)
					{
						pos = len_line_logic - (len_line_logic % scr_x - x);
						if (len_line_logic % scr_x == 0)
							pos -= scr_x;
						--y;
						--show_y;
					}
					else
					{
						pos = len_line_logic - 1;
						x = pos % scr_x;
						--y;
						--show_y;
					}
					text_pos -= len_line_logic - pos;
				}
			}
			if (show_y < 0)
				show_y = 0;
			one_line_up(tx);						//更新第一个输出字符的位置
			break;
		case 'l':									//右移一个字符
				if (pos == len_line_logic - 1)		//到下一个逻辑行
				{
					if (cur_line_logic != total_line_logic - 1)
					{
						++text_pos;
						++cur_line_logic;
						len_line_logic = tx.rows[cur_line_logic].length();
						pos = 0;
						x = 0;
						++y;
						++show_y;
					}
				}
				else if (x ==  scr_x - 1)			//逻辑行不变，到下一显示行
				{
				++text_pos;
				++pos;
				x = 0;
				++y;
				++show_y;
				}
				else
				{
					++text_pos;
					++x;
					++pos;
				}
			if (show_y >= scr_y)
				show_y = scr_y - 1;
			one_line_dn(tx);						//更新第一个输出字符的位置
			break;
		case '0':
			x = 0;
			y -= pos / scr_x;
			show_y -= pos / scr_x;
			text_pos -= pos;			
			pos = 0;
			if (show_y < 0)
				show_y = 0;
			go_head(tx);							//更新第一个输出字符的位置
			break;
		case '$':
			y -= pos / scr_x;
			show_y -= pos / scr_x;
			text_pos -= pos;
			y += (len_line_logic - 1) / scr_x;
			show_y += (len_line_logic - 1) / scr_x;
			pos = len_line_logic - 1;				//移到\n的位置
			x = pos % scr_x;
			text_pos += pos;
			if (show_y >= scr_y)
				show_y = scr_y - 1;
			go_rear(tx);							//更新第一个输出字符的位置
			break;
		case '(':									//到文件头
			x = 0, y = 0;
			show_y = 0;
			print_y = 0;
			print_line = 0;
			print_pos = 0;
			cur_line_logic = 0;
			pos = 0;
			text_pos = 0;
			break;
		case ')':									//到文件尾
			cur_line_logic = total_line_logic - 1;
			len_line_logic = tx.rows[cur_line_logic].length();
			pos = len_line_logic - 1;
			x = pos % scr_x;
			y = -1;
			int len, re;
			for (int i = 0; i < total_line_logic; ++i)
			{
				len = tx.rows[i].length();
				re = len % scr_x;
				y += len / scr_x;
				if (re != 0)
					++y;
			}
			print_line = total_line_logic - 1;
			print_pos = 0;
			show_y = len / scr_x;
			print_y = y - show_y;
			break;
		default:
			break;
	}
}
void Cursor::move_to(int _y, int _x , Text& tx){	
	if (_x >= scr_x)									//不合法的位置
		return;			
	total_line_logic = tx.lines;
	int i, len, re, cnt = 0, tmp;
	int t_pos = 0;
	_y += print_y;
	for (i = 0; i < total_line_logic; ++i)
	{
		len = tx.rows[i].length();	
		re = len % scr_x;
		cnt += len / scr_x;
		if (re != 0)
			++cnt;
		if (cnt >= _y + 1)
		{
			tmp = len / scr_x;
			if (re != 0)
				++tmp;
			break;
		}
		else
			t_pos += len;
	}
	if (i < total_line_logic)							//给定的_y合法
	{
		y = _y;
		show_y = y - print_y;
		cur_line_logic = i;
		cnt -= tmp;
		pos = (y - cnt) * scr_x;
		x = 0;
		int limit_x = len - pos - 1;
		_x = _x < limit_x ? _x : limit_x;				//若目标行太短，则移到行尾
		while (x < _x)
			move('l', tx);
	}
	else												//移到最后一显示行
	{
		cal_max_y(tx);
		y = max_y - 1;
		show_y = y - print_y;
		cur_line_logic = total_line_logic - 1;
		pos = ((len - 1) / scr_x) * scr_x;
		x = 0;
		int limit_x = len - pos - 1;
		_x = _x < limit_x ? _x : limit_x;
		while (x < _x)
			move('l', tx);
	}
}
void Cursor::cal_max_y(Text & tx)						//计算显示行的最大值
{
	int len, re;
	max_y = 0;
	for (int jj = 0; jj < total_line_logic; ++jj)		//总逻辑行已经由调用者求出
	{
		len = tx.rows[jj].length();
		max_y += len / scr_x;
		re = len % scr_x;
		if (re != 0)
			++max_y;
	}
}
void Cursor::c_scroll(char c , Text& tx)
{
	total_line_logic = tx.lines;	
	switch (c)
	{
	case 2:
		page_up(y, x, cur_line_logic, pos, scr_y / 2, true , tx);				  //改变光标的位置，用true标记
		page_up(print_y, print_x, print_line, print_pos, scr_y / 2, false , tx);  //改变输出的第一个字符的在文本中的位置，false标记
		break;
	case 6:
		cal_max_y(tx);
		page_dn(y, x, cur_line_logic, pos, scr_y / 2, true , tx, true);
		page_dn(print_y, print_x, print_line, print_pos, scr_y / 2, false , tx, true);
		show_y = y - print_y;
		break;
	case 21:
		page_up(y, x, cur_line_logic, pos, scr_y, true , tx);
		page_up(print_y, print_x, print_line, print_pos,scr_y, false , tx);
		break;
	case 4:
		cal_max_y(tx);
		page_dn(y, x, cur_line_logic, pos, scr_y, true , tx, true);
		page_dn(print_y, print_x, print_line, print_pos, scr_y, false , tx, true);
		show_y = y - print_y;
		break;
	default:
		break;
	}
}
void Cursor::one_line_up(Text& tx)				//对(可能)上移一行的情况（h, k), 更新第一个输出字符的位置
{
	if (y >= print_y)
		return;
	print_len = tx.rows[print_line].length();
	if (print_pos != 0)
	{
		print_pos -= scr_x;
		--print_y;
	}
	else if (print_line != 0)
	{
		--print_line;
		--print_y;
		print_len = tx.rows[print_line].length();
		int re = print_len % scr_x;
		print_pos = (print_len / scr_x) * scr_x;
		if (re == 0)
			print_pos -= scr_x;
	}
}
void Cursor::one_line_dn(Text& tx)				//对(可能)下移一行的情况（l, j), 更新第一个输出字符的位置
{
	if (y - print_y < scr_y)
		return;
	print_len = tx.rows[print_line].length();
	if (print_pos + scr_x <= print_len)
	{
		print_pos += scr_x;
		++print_y;
	}
	else if (print_line != total_line_logic - 1)//total_line_logic已由调用者给出
	{
		++print_line;
		++print_y;
		print_pos = 0;		
	}
}
void Cursor::page_up(int & yy, int & xx, int & line, int & p, int dif, bool p_or_t , Text& tx)
{
	dif = dif < print_y ? dif : print_y;
	int re, len, cnt = 0;
	cnt += p / scr_x;
	if (p_or_t)
		text_pos -= p;
	int i = line;
	if (cnt < dif)
	{
		for (i = line - 1; i >= 0; --i)
		{
			len = tx.rows[i].length();
			if (p_or_t)
				text_pos -= len;
			re = len % scr_x;
			cnt += len / scr_x;
			if (re != 0)
				++cnt;
			if (cnt >= dif)
				break;
		}
	}
	if (i >= 0)
	{
		line = i;
		p = (cnt - dif) * scr_x;
		len = tx.rows[i].length();
		if (len - 1 >= p + xx)			//此处不必考虑是最后一逻辑行的情况，因为肯定足够长
		{
			p += xx;
			if (p_or_t)
				text_pos += p;
			yy -= dif;
		}
		else
		{
			p = len - 1;
			xx = p % scr_x;
			if (p_or_t)
				text_pos += p;
			yy -= dif;
		}
	}
	else								//i 必定不是最后一逻辑行
	{
		line = 0;
		len = tx.rows[line].length();
		if (len - 1 >= xx)
		{
			p = xx;
			if (p_or_t)
				text_pos += p;
			yy = 0;
		}
		else
		{
			p = xx = len - 1;
			if (p_or_t)
				text_pos += p;
			yy = 0;
		}
	}
}
void Cursor::page_dn(int & yy, int & xx, int & line, int & p, int dif, bool p_or_t , Text& tx, bool spec_or_extra)
{
	int re, len, cnt = 0;
	if (max_y - print_y <= scr_y && spec_or_extra)		//spec_or_extra == true表示翻页
		return;											//spec_or_extra == false表示移到逻辑行尾时的翻页
	if (p_or_t)
	{
		int tmp = max_y - yy;
		dif = dif < tmp ? dif : tmp;
	}
	len = tx.rows[line].length();	
	re = len % scr_x;
	cnt += len / scr_x;
	if (re != 0)
		++cnt;
	re = (p + 1) % scr_x;
	cnt -= (p + 1) / scr_x;
	if (re != 0)
		--cnt;
	if (p_or_t)
		text_pos -= p;
	int i = line;
	if (cnt >= dif)
	{
		if (p + dif * scr_x < len)
		{
			p += dif * scr_x;
			if (p_or_t)
				text_pos += p;
			yy += dif;
		}
		else										//此时对于false的情况，应该有len % scr_x = 1
		{
			p = len - 1;
			if (p_or_t)
				text_pos += p;
			xx = p % scr_x;
			yy += dif;
		}
		return;
	}
	for (i = line + 1; i < total_line_logic; ++i)
	{
		if (p_or_t)									//加上前一逻辑行的长度
			text_pos += len;	
		len = tx.rows[i].length();
		re = len % scr_x;
		cnt += len / scr_x;
		if (re != 0)
			++cnt;
		if (cnt >= dif)
			break;
	}
	if (i < total_line_logic)
	{
		cnt -= len / scr_x;							//回到前一逻辑行
		if (re != 0)
			--cnt;
		line = i;
		p = (dif - cnt - 1) * scr_x;
		if (p + xx < len)
		{
			p += xx;
			if (p_or_t)
				text_pos += p;
			yy += dif;
		}
		else										//此时对于false的情况，应该有len % scr_x = 1
		{
			p = len - 1;
			if(p_or_t)
				text_pos += p;
			xx = p % scr_x;
			yy += dif;
		}
	}
	else											//行数不够
	{
		line = total_line_logic - 1;				//长度len在循环结束时已保存
		p = (len / scr_x) * scr_x;
		if (p + xx < len)
		{
			p += xx;
			if (p_or_t)
				text_pos += p;
			yy += cnt;
		}
		else
		{
			p = len - 1;
			xx = p % scr_x;
			if (p_or_t)
				text_pos += p;
			yy += cnt;
		}
	}
}
void Cursor::go_head(Text &tx)
{
	if (y >= print_y)
		return;
	print_y = y;
	print_pos = 0;
}
void Cursor::go_rear(Text &tx)							//改变第一个输出字符在文本中的位置
{
	if (y - print_y < scr_y)
		return;
	int dif = y - print_y - scr_y + 1;					//显示内容应该上移的显示行行数	
	page_dn(print_y, print_x, print_line, print_pos, dif, false , tx, false);
}														//利用用翻页的函数实现
