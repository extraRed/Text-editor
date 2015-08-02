//文件名：text.cpp

//作者: 刘敏行（1200012863）姜双（1200012826）

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


#include "text.h"
#include "cursor.h"
#include <queue>
#include <algorithm>
using namespace std;

Text::Text(const char * FILENAME){
	fstream iofile(FILENAME  , ios::in | ios::out);
	if(! iofile)
	{
		cout << "File open error!" << endl;
		exit(-1);
	}
	int length=strlen(FILENAME);
	File_name= new char[length+1];
	strcpy(File_name, FILENAME);
	File_name[length]='\0';
	char ch;
	lines=0;
	num_of_words=0;
	while(ch = iofile.get()){
		if (ch == EOF)
		{
//			if(!content.empty())
//				lines++;		
			break;
		}
		if(ch == '\n')
			lines++;
		content += ch;
		num_of_words++;
	}
//	cout << "HERE" ;
//	cout << content.length() << endl;
	if(content.length() == 0) {
		lines = 1;
		content = "\n";
	}
//	--lines;
//	cout << lines << endl;
	buffer = content;			// 复制到缓存中
	if(lines == 0)
		return;
	string::iterator p;
//	cout << "HERE@@@" << endl;
	for(p=buffer.begin(); ; )
	{
		if(buffer.length() == 0) break;
		string temp;
		while(1)
		{
			//cout << "H)" << endl;
			if(*p == '\n')
			{
				//cout << "H!" << endl;
				temp += '\n';
				rows.push_back(temp);
				p++;
				break;
			}
			else
			{
				//cout << "H@" << endl;
				temp += *p;
				p++;
			}
			if(p == buffer.end())
			{
				//cout << "H#" << endl;
				temp += *p;
				break;
			}
		}
		if(p == buffer.end())
			 break;
		else
			continue;
	}
	highlights.clear();
	highlights.resize(buffer.length()+1);
	fill(highlights.begin() , highlights.end() , 0);
}
Text::~Text(){;}

void Text::save(){
	const char *p= buffer.c_str();
	int l=strlen(p);
	ofstream fout(File_name);
	if(!fout)
	{
		//cout << "File open error!" << endl;
		return;
	}
	for(int i=0; i<l; i++)
		fout << p[i];
	fout.close();
	return;
}

void Text::content_insert(string s , int line , int pos){								// insert a string at the cursor
	if(rows.empty())
	{
		buffer = '\n';
		rows.push_back(buffer);
		lines = 1;
//		buffer=s;
//		buffer += '\n';
//		rows.push_back(s);
//		rows[0] += '\n';
//		return;
	}
	int cur_line = line, cur_pos = pos;
	vector<string>::iterator p=rows.begin();
	string temp=*(p+cur_line);
	string a;
	for(int i=0; i < cur_pos; i++)
		a += temp[i];																// a contains the information before cur_pos
	a += s;																			// insert s
	string b;
	for(int i=cur_pos; i<temp.size(); i++)
		b += temp[i];
	a += b;																			// a += information after cur_pos
	*(p+cur_line)=a;
	buffer.clear();
	for(p=rows.begin(); p != rows.end(); p++)
		buffer += *p;
	clear_highlights();
}
void Text::content_insert_enter(int _line, int _pos){
	queue<string> temp;	// save the content before inserting
	vector<string>::iterator p;
	if(rows.empty())													// empty file
	{
		buffer = "\n";
		rows.push_back(buffer);
		lines = 1;
//		string t="\n";
//		rows.push_back(t);
//		buffer=t;
//		lines++;
		return;
	}
	if(_line == -1 || (_line == 0 && _pos == 0))							// -1 means creating a new line in the very begining of the content
	{
		for(p= rows.begin(); p != rows.end(); p++)
			temp.push(*p);
		rows.erase(rows.begin(), rows.end());
		string t = "\n";
		rows.push_back(t);
		while(!temp.empty())
		{
			rows.push_back(temp.front());
			temp.pop();
		}
		buffer.clear();
		for(p=rows.begin(); p != rows.end(); p++)						// every time we change the content, we should update the buffer
			buffer += *p;
		lines++;
		return;
	}
	else if(_pos == 0)													// create a new line before the current line
	{
		p=rows.begin()+_line;
		for(; p != rows.end(); p++)
			temp.push(*p);
		rows.erase(rows.begin()+_line, rows.end());
		string t="\n";
		rows.push_back(t);
		while(!temp.empty())
		{
			rows.push_back(temp.front());
			temp.pop();
		}
		buffer.clear();
		for(p=rows.begin(); p != rows.end(); p++)
			buffer += *p;
		lines++;
		return;
	}
	else							
	{
		p=rows.begin();
		int len=(*(p+_line)).size();
		if(len == _pos+1)															// create a new line after the current line ,the cursor is at the end of the current line
		{
			for(p=rows.begin()+_line+1; p != rows.end(); p++)
				temp.push(*p);
			string t = "\n";
			rows.erase(rows.begin()+_line+1, rows.end());
			rows.push_back(t);
			while(!temp.empty())
			{
				rows.push_back(temp.front());
				temp.pop();
			}
			buffer.clear();
			for(p=rows.begin(); p != rows.end(); p++)
				buffer += *p;
			lines++;
			return;
		}
		else																// insert a '\n' to make one line two new lines
		{
			string t1,t2;
			int i;
			p=rows.begin()+_line;
			t1=*p;
			for(i=0; i<_pos; i++)											// _pos is where we insert '\n, before it we insert '\n' to make the first new line ,after it is the second
					t2 += t1[i];
			t2 += '\n';
			for(p=rows.begin()+_line+1; p != rows.end(); p++)
				temp.push(*p);
			rows.erase(rows.begin()+_line, rows.end());
			rows.push_back(t2);
			t2.clear();
			for(i=_pos; i<t1.size(); i++)
				t2 += t1[i];
			rows.push_back(t2);
			while(!temp.empty())
			{
				rows.push_back(temp.front());
				temp.pop();
			}
			buffer.clear();
			for(p=rows.begin(); p != rows.end(); p++)
				buffer += *p;
			lines++;
			return;
		}
	}
}
void Text::c_delete(int line , int pos) {
	int cur_line = line , cur_pos = pos;
	rows[line].erase(pos , 1);
	int t_pos = get_pos(line , pos);
	buffer.erase(t_pos , 1);
	clear_highlights();
}

/*void Text::content_delete(int pos1, int pos2){							// 删除第pos1行到第pos2行的所有内容
	vector<string>::iterator del=rows.begin();
	if(del+pos2 <= rows.end())
	rows.erase(del+pos1-1, del+pos2);
	del=rows.begin();
	buffer.clear();
	for(; del != rows.end(); del++)
		buffer += *del;
	clear_highlights();
}*/
void Text::content_delete(int pos){											// 删除第pos行的内容
	if(lines == 1){
		buffer = "\n";
		rows.clear();
		rows.push_back(buffer);
		lines = 1;
		return;
	}
	vector<string>::iterator del=rows.begin();
	rows.erase(del+pos-1);
	lines--;
	del=rows.begin();
	buffer.clear();
	for(; del != rows.end(); del++)
		buffer += *del;
	clear_highlights();
}


void Text::content_delete(int rowid,int rowpos1,int rowpos2)				//delete row rowid from rowpos1 to rowpos2	
{
	string::iterator str=rows[rowid].begin();
	rows[rowid].erase(str+rowpos1,str+rowpos2+1);
	vector<string>::iterator del=rows.begin();
	buffer.clear();
	for(; del != rows.end(); del++)
		buffer += *del;
	clear_highlights();
}
int Text::one_line_length(int a){										//返回第a个逻辑行的长度（包括‘\n’）
	vector<string>::iterator f=rows.begin();
	string temp=*(f+a);
	const char *p=temp.c_str();
	return strlen(p);
}
int Text::total_lines(){
	return lines;
}

int Text::get_pos(int _line , int _pos){
	int sum = 0;
	for(int i = 0 ; i < _line ; ++i) sum += rows[i].length();
	sum += _pos;
	return sum;
}
void Text::clear_highlights()					//clear highlights
{
	highlights.clear();
	highlights.resize(buffer.length()+1);
	fill(highlights.begin(),highlights.end(),0);
}
void Text::update_highlights(int m,int Find[1001][3])	//update highlights according to the searching result
{
	clear_highlights();
	for(int i=0;i<m;++i)
	{
		int sum=get_pos(Find[i][2],Find[i][0]);
		for(int j=sum;j<=sum+Find[i][1]-Find[i][0];++j)
			highlights[j]=1;
	}	
}
void Text::getnext(string s,int next[][101],int id)					//get next	
{																	//I referred to the book somewhere in this function
									//since we have many substrings due to'*', I add a dimension to next
	int len=s.length();
	int i=0, j=-1;
	next[id][0] = -1;
	while(i<len)
	{
		if(j==-1 || s[i]==s[j] || s[j]=='?')	//'?'represents any character so count it when calculating next
			next[id][++i] = ++j;
		else
			j = next[id][j];
	}
}
int Text::kmp(int ti,int id,string t,string tmp,int next[][101],int pos[][101],int cnt[])//kmp	
{																						//I referred to the book somewhere in this function
									//ti:the starting match position of pattern string
									//id:the id of the substring of target string
									//t:the pattern string
									//tmp:the target string
									//pos:records all the matched position
									//cnt:records the number of the matched cases
	int len=tmp.length();
	int tn=t.length();
	int j=0;
	bool check=false;
	while(ti<tn && j<len)
	{
		if(j==-1 || t[ti]==tmp[j] || tmp[j]=='?')	//'?' is regarded to match any charater
		{
			++ti;
			++j;
		}
		else
			j = next[id][j];
		if(j==len)							//if find matched, then go and find next one
		{
			check=true;
			j = next[id][j];
			pos[id][cnt[id]++]=ti;
		}
	}
	return check;						//return 1 if there is at least one matched case
										//else return 0
}
int Text::partition(string s,string tmp[],int &all,int &no,int next[][101])//partition s by'*'
{
										//s:target string
										//tmp:a string array to store all substrings of s partitioned by'*'
										//all:check if s has only'*'
										//no:check if s has no'*';
	int sn=s.length(),j=0,k=0,id=-1;
	while(j<sn)
	{
		while(j<sn && s[j]=='*')		//do until s ends or find a no star character
		{
			no=0;
			++j;
		}
		if(j<sn)
		{
			all=0;
			++id;
		}
		else
			break;
		tmp[id].clear();
		k=j; 
		while(k<sn && s[k]!='*')		//get the string between two *s(or the beginning/the end of the string)
			++k;
		for(int l=j;l<k;++l)			//move the substring to tmp[id];
			tmp[id]+=s[l];
		getnext(tmp[id],next,id);		//for each substring, get its next;
		j=k;
	}
	return id; 							//return the total substrings partitioned by '*'
}	
	
int Text::content_search(int Find[1001][3],string s)	//search string
{
										//Find:records the infomation of the matched case
										//for each m, Find[m][2] records the rowid
										//Find[m][0] records the beginning of the matched position
										//Find[m][1] records the end of the matched position
	int i,j,k,l,sum,tn,sn,first_len,matched,rowid,next[101][101],cnt[101],pos[101][101],cntrow[101];
	int flag,star_first,star_last,star_all,no_star;		//some variables to handle some special cases
	string t,tmp[101];
	flag=star_first=star_last=0;
	star_all=no_star=1;
	sn=s.length();
	if(s[0]=='*')
		star_first=1;
	if(s[sn-1]=='*')
		star_last=1;
	memset(next,0,sizeof(next));
	sum=partition(s,tmp,star_all,no_star,next);
	first_len=tmp[0].length();
	rowid=matched=0;
	vector<string>::iterator p=rows.begin();
	while(p!=rows.end())		//for each string in rows, do matching process 
	{
		t=*p;
		tn=t.length();
		if(star_all)			//star_all means it matches the whole string
		{
			Find[matched][0]=0;
			Find[matched][1]=tn-1;
			++p;
			++rowid;
			++matched;
			continue;
		}		
		memset(cnt,0,sizeof(cnt));
		memset(pos,0,sizeof(pos));
		flag=i=j=0;
		while(i<tn && j<=sum)
		{
			flag=kmp(i,j,t,tmp[j],next,pos,cnt);
			if(flag==0)			//only if when the pattern string matches all target strings can be regarded as "matched"
				break;
			i=pos[j][0];
			++j;
			if(j<=sum && i>=tn)	//if the pattern string ends but there are still target strings, it fails to match
				flag=0;
		}
		if(flag==0)			//match failed
		{
			++p;
			++rowid;
			continue;
		}
		else
		{
			{
				if(no_star)		//if the target string doesn't contain any '*', then I record every matched case
				{
					Find[matched][0]=pos[0][0]-first_len;
					Find[matched][1]=pos[0][0]-1;
					Find[matched][2]=rowid;
					for(int f=1;f<cnt[0];++f)
					{
						++matched;
						Find[matched][0]=pos[0][f]-first_len;
						Find[matched][1]=pos[0][f]-1;
						Find[matched][2]=rowid;
						if(Find[matched][0]<=Find[matched-1][1])
							--matched;
					}
				}
				else if(sum==0)		//if there is at least one star, I record the longest matched case
				{
					Find[matched][0]=pos[0][0]-first_len;
					Find[matched][1]=pos[0][0]-1;
					Find[matched][2]=rowid;
				}
				else
				{
					Find[matched][0]=pos[0][0]-first_len;
					Find[matched][1]=pos[sum][cnt[sum]-1]-1;
					Find[matched][2]=rowid;
				}
			}
			{
				if(star_first && star_last)
				{
					Find[matched][0]=0;
					Find[matched][1]=tn-1;
					Find[matched][2]=rowid;
				}
				else if(star_first)
				{
					Find[matched][0]=0;
					Find[matched][1]=pos[sum][cnt[sum]-1]-1;
					Find[matched][2]=rowid;
				}
				else if(star_last)
				{
					Find[matched][0]=pos[0][0]-first_len;
					Find[matched][1]=tn-1;
					Find[matched][2]=rowid;
				}
			}
			++matched;
			++p;
			++rowid;
		}
	}
	update_highlights(matched , Find);
	return matched;
}
void Text::replace(string rep , string insertion)		//replace string
{
														//rep:the string user wants to be replaced
														//insertion:the string user wants to replace
	int Find[1001][3];
	int cnt=content_search(Find,rep);					//search the target string
	int bias;											//this bias is to correct the position changes when deleting and inserting
														//thanks to shiyqw, I correct this mistake 
	if(cnt>0)
	{
		content_delete(Find[0][2],Find[0][0],Find[0][1]);	
		content_insert(insertion,Find[0][2],Find[0][0]);	
	}	
	for(int i=1;i<cnt;++i)
	{
		if(Find[i][2]!=Find[i-1][2])
			bias=0;
		else
			bias+=insertion.length()-rep.length();
		Find[i][0]+=bias;
		Find[i][1]+=bias;
		content_delete(Find[i][2],Find[i][0],Find[i][1]);	//first delete
		content_insert(insertion,Find[i][2],Find[i][0]);	//then insert
	}
	int Result[1001][3];					//when updating highlights, I should update the length as well
											//thanks to shiyqw, I correct this mistake
	for(int i = 0 ; i < 1001 ; ++i){
		for(int j = 0 ; j < 3 ; ++j) {
			Result[i][j] = Find[i][j];
		}
		Result[i][1] = Result[i][0] + insertion.length() - 1;
	}
	update_highlights(cnt,Result);
}
