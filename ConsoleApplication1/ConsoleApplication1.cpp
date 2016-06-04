// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//
#define _CRT_SECURE_NO_DEPRECATE
#include "stdafx.h"
#include<cstdio>
#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<cmath>
using namespace std;


struct TOKEN//词法 TOKEN结构体
{
	int code;//编码,id
	int num;//递增编号
	TOKEN *next;
};
TOKEN *TOKEN_head, *TOKEN_tail;//TOKEN队列
struct str//词法 符号表结构体
{
	int num;//编号
	string  word;//字符串内容
	str *next;
};
str *string_head, *string_tail;//string队列
struct generor//语法 产生式结构体
{
	char left;//产生式的左部
	string right;//产生式的右部
	int len;//产生式右部的长度
};
generor css[20];//语法 20个产生式
struct actionStruct//语法 action表结构体
{
	char sr;//移进或归约
	int state;//转到的状态编号
};
actionStruct action[46][18];//action表
int go_to[46][11];//语法 go_to表
struct ike//语法 分析栈结构体，双链
{
	ike *pre;
	int num;//状态
	int word;//符号编码
	ike *next;
};
ike *stack_head, *stack_tail;//分析栈首尾指针
struct L//语义四元式的数据结构
{
	int k;
	string op;//操作符
	string op1;//操作数
	string op2;//操作数
	string result;//结果
	L *next;//语义四元式向后指针
	L *Ltrue;//回填true链向前指针
	L *Lfalse;//回填false链向前指针
};
L *L_four_head, *L_four_tail, *L_true_head, *L_false_head;//四元式链，true链，false链
struct symb//语义输入时符号表
{
	string word;//变量名称
	int addr;//变量地址
	symb *next;
};
symb *symb_head, *symb_tail;//语义符号链表

							//词法分析相关的一些函数的声明

void scan();//按字符读取源文件
void lexical();//词法分析主程序
int JUDGE(char ch);//判断输入字符的类型
void OutputOne(char ch);//写入TOKEN.txt
void OutputT(char ch, string word);//写入string.txt
void InOne(TOKEN *temp);//插入结点到队列TOKEN
void InT(str *temp);//插入结点到队列string
void output();//输出三个队列的内容
void outfile();//输出三个队列的内容到相应文件中

			   //语法分析相关的一些函数

void Grammer();//语法分析主程序
void GrammerInit();//初始化语法分析数据结构
int GrammerSLR(int a);//语法分析主体部分
int id1(int a);//给输入字符编号，转化成action表列编号
string id10(int i);//给输入字符反编号
int id2(char ch);//给非终结状态编号，转化成go_to表列编号
int id20(char ch);//给非终结状态编号
char id21(int j);//给非终结状态反编号
void add(ike *temp);//给ike分析栈链表增加一个结点
void del();//给ike分析栈链表删除一个结点

		   //语义分析相关的一些函数

void Semantic(int m);//语义分析主程序
void add_L_four(L *temp);//向四元式链中加一个结点
void add_L_true(L *temp);//向true链中加一个结点
void add_L_false(L *temp);//向false链中加一个结点
void add_symb(symb *temp);//向语义符号表链中加一个结点
void output_yuyi();//输出中间代码四元式和最后符号表
string newop(int m);//把数字变成字符串
string id_numtoname(int num);//把编号转换成相应的变量名
int lookup(string m);//变量声明检查

					 //全局变量的声明

FILE *fp;//文件指针
int wordcount;//标志符计数
int err;//标志词法分析结果正确或错误
int nl;//读取行数
int yuyi_linshi;//语义临时变量
string E_name, T_name, F_name, M_name, id_name, id1_name, id2_name, errword;//用于归约时名称传递和未声明变量的输出
int id_num, id1_num, id2_num, id_left, id_while, id_then, id_do;//用于记录一些特殊的字符位置信息



int main()
{
	//	cout<<"*  对给定文件依次进行词法分析，语法分析，语义分析： *\n"<<endl;
	lexical();//词法
	Grammer();//语法
	output_yuyi();//语义
	cout << endl;
	system("pause");
	return(0);
}

//词法分析

void lexical()
{
	TOKEN_head = new TOKEN;
	TOKEN_head->next = NULL;
	TOKEN_tail = new TOKEN;
	TOKEN_tail->next = NULL;
	string_head = new str;
	string_head->next = NULL;
	string_tail = new str;
	string_tail->next = NULL;//初始化三个队列的首尾指针
	L_four_head = new L;
	L_four_head->next = NULL;
	L_four_tail = new L;
	L_four_tail->k = 0;
	L_four_tail->next = NULL;
	L_true_head = new L;
	L_true_head->Ltrue = NULL;
	L_false_head = new L;
	L_false_head->Lfalse = NULL;
	symb_head = new symb;
	symb_head->next = NULL;
	symb_tail = new symb;
	symb_tail->next = NULL;
	yuyi_linshi = -1;
	id_num = 0;
	wordcount = 0;//初始化字符计数器
	err = 0;//初始化词法分析错误标志
	nl = 1;//初始化读取行数
	scan();
	if (err == 0)
	{
		char m;
		output();
		//	cout<<"词法分析正确完成!"<<endl<<endl<<"如果将结果保存到文件中请输入 y ，否则请输入其它字母：";
		//	cin>>m;
		//	cout<<endl;
		//	if(m=='y')
		//	{
		outfile();
		cout << "结果成功保存在token.txt和string.txt两个文件中" << endl;
		cout << endl;
		//	}
	}
}
void scan()
{
	cout << endl;
	char ch;
	string word;
	char document[50];
	int flag = 0;
	cout << "请输入源文件路径及名称（当前目录下有可测试文件test.txt）:";
	cin >> document;
	cout << endl;
	cout << "词法分析如下\n" << endl;

	if ((fp = fopen(document, "rt")) == NULL)
	{
		err = 1;
		cout << "无法找到该文件!" << endl;
		return;
	}
	while (!feof(fp))
	{
		word = "";
		ch = fgetc(fp);
		flag = JUDGE(ch);
		if (flag == 1)
			OutputOne(ch);
		else if (flag == 3)
			OutputT(ch, word);
		else if (flag == 4 || flag == 5 || flag == 6)
			continue;
		else
		{
			cout << nl << "行  " << "错误:非法字符!  " << ch << endl;
			err = 1;
		}
	}
	fclose(fp);
}
int JUDGE(char ch)
{
	int flag = 0;
	if (ch == '=' || ch == '+' || ch == '*' || ch == '>' || ch == ':' || ch == ';' || ch == '{' || ch == '}' || ch == '(' || ch == ')')
		flag = 1;//界符
	else if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z'))
		flag = 3;//字母
	else if (ch == ' ')
		flag = 4;//空格
	else if (feof(fp))
		flag = 5;//结束
	else if (ch == '\n')
	{
		flag = 6;//换行
		nl++;
	}
	else
		flag = 0;//非法字符
	return(flag);
}
void OutputOne(char ch)
{
	int id;
	switch (ch)
	{
	case '=': id = 1; break;
	case '+': id = 2; break;
	case '*': id = 3; break;
	case '>': id = 4; break;
	case ':': id = 5; break;
	case ';': id = 6; break;
	case '{': id = 7; break;
	case '}': id = 8; break;
	case '(': id = 9; break;
	case ')': id = 10; break;//界符编码
	default: id = 0;
	}
	TOKEN *temp;
	temp = new TOKEN;
	temp->code = id;
	temp->num = -1;
	temp->next = NULL;
	InOne(temp);
	return;
}
void OutputT(char ch, string word)
{
	TOKEN *temp;
	temp = new TOKEN;
	temp->code = -1;
	temp->num = -1;
	temp->next = NULL;
	str *temp1;
	temp1 = new str;
	temp1->num = -1;
	temp1->word = "";
	temp1->next = NULL;
	int flag = 0;
	word = word + ch;
	ch = fgetc(fp);
	flag = JUDGE(ch);
	if (flag == 1 || flag == 4 || flag == 5 || flag == 6)
	{
		if (word == "and" || word == "if" || word == "then" || word == "while" || word == "do" || word == "int")
		{
			if (word == "and")
				temp->code = 31;
			else if (word == "if")
				temp->code = 32;
			else if (word == "then")
				temp->code = 33;
			else if (word == "while")
				temp->code = 35;
			else if (word == "do")
				temp->code = 36;
			else if (word == "int")
				temp->code = 37;//关键字编码
			InOne(temp);
			if (flag == 1)
				OutputOne(ch);
			else if (flag == 4 || flag == 5 || flag == 6)
				return;
		}
		else if (flag == 1)
		{
			wordcount++;
			temp->code = 25;
			temp->num = wordcount;
			InOne(temp);
			temp1->num = wordcount;
			temp1->word = word;
			InT(temp1);
			OutputOne(ch);
		}
		else if (flag == 4 || flag == 5 || flag == 6)
		{
			wordcount++;
			temp->code = 25;
			temp->num = wordcount;
			InOne(temp);
			temp1->num = wordcount;
			temp1->word = word;
			InT(temp1);
		}
		return;
	}
	else if (flag == 2 || flag == 3)
		OutputT(ch, word);//形成字符串
	else
	{
		err = 1;
		cout << nl << "行  " << "错误:非法字符!  " << ch << endl;
		return;
	}
}
void InOne(TOKEN *temp)
{
	if (TOKEN_head->next == NULL)
	{
		TOKEN_head->next = temp;
		TOKEN_tail->next = temp;
	}
	else
	{
		TOKEN_tail->next->next = temp;
		TOKEN_tail->next = temp;
	}
}
void InT(str *temp)
{
	if (string_head->next == NULL)
	{
		string_head->next = temp;
		string_tail->next = temp;
	}
	else
	{
		string_tail->next->next = temp;
		string_tail->next = temp;
	}
}
void output()
{
	cout << "TOKEN表内容如下：" << endl;
	TOKEN *temp1;
	temp1 = new TOKEN;
	temp1 = TOKEN_head->next;
	while (temp1 != NULL)
	{
		cout << temp1->code;
		if (temp1->num == -1)
		{
			cout << endl;
		}
		else
		{
			cout << "   " << temp1->num << endl;
		}
		temp1 = temp1->next;
	}
	cout << "符号表内容如下：" << endl;
	str *temp3;
	temp3 = new str;
	temp3 = string_head->next;
	while (temp3 != NULL)
	{
		cout << temp3->num << "   " << temp3->word << endl;
		temp3 = temp3->next;
	}
}
void outfile()
{
	ofstream fOutputOne("TOKEN.txt");//写文件
	ofstream fOutputT("string.txt");
	TOKEN *temp1;
	temp1 = new TOKEN;
	temp1 = TOKEN_head->next;
	while (temp1 != NULL)
	{
		fOutputOne << temp1->code;
		if (temp1->num == -1)
			fOutputOne << endl;
		else
			fOutputOne << "   " << temp1->num << endl;
		temp1 = temp1->next;
	}
	str *temp3;
	temp3 = new str;
	temp3 = string_head->next;
	while (temp3 != NULL)
	{
		fOutputT << temp3->num << "   " << temp3->word << endl;
		temp3 = temp3->next;
	}
}

//语法分析

void Grammer()
{
	if (err == 0)
	{
		system("pause");
		cout << endl;
		cout << "*  语法分析 *\n" << endl;
		GrammerInit();//初始化语法分析数据结构
		TOKEN *temp;
		temp = new TOKEN;
		temp = TOKEN_head->next;
		int p, q;
		p = 0;
		q = 0;
		cout << "语法分析过程如下：" << endl;
		while (temp != NULL)
		{
			int w;
			w = id1(temp->code);
			p = GrammerSLR(w);
			if (p == 1) break;
			if (p == 0)
				temp = temp->next;
			if (temp == NULL) q = 1;
		}//语法分析
		if (q == 1)
			while (1)
			{
				p = GrammerSLR(17);
				if (p == 3) break;
			}//最后输入$来完成语法分析
	}
}
void GrammerInit()
{
	stack_head = new ike;
	stack_tail = new ike;
	stack_head->pre = NULL;
	stack_head->next = stack_tail;
	stack_head->num = 0;
	stack_head->word = '!';
	stack_tail->pre = stack_head;
	stack_tail->next = NULL;
	//初始化栈分析链表
	css[0].left = 'Q';
	css[0].right = "P";
	css[1].left = 'P';
	css[1].right = "id()L;R";
	css[2].left = 'L';
	css[2].right = "L;D";
	css[3].left = 'L';
	css[3].right = "D";
	css[4].left = 'D';
	css[4].right = "id:int";
	css[5].left = 'E';
	css[5].right = "E+T";
	css[6].left = 'E';
	css[6].right = "T";
	css[7].left = 'T';
	css[7].right = "T*F";
	css[8].left = 'T';
	css[8].right = "F";
	css[9].left = 'F';
	css[9].right = "(E)";
	css[10].left = 'F';
	css[10].right = "id";
	css[11].left = 'B';
	css[11].right = "B and B";
	css[12].left = 'B';
	css[12].right = "id>id";
	css[13].left = 'M';
	css[13].right = "id=E";
	css[14].left = 'S';
	css[14].right = "if B then M";
	css[15].left = 'S';
	css[15].right = "while B do M";
	css[16].left = 'S';
	css[16].right = "M";
	css[17].left = 'N';
	css[17].right = "N;S";
	css[18].left = 'N';
	css[18].right = "S";
	css[19].left = 'R';
	css[19].right = "{N}";
	int i, j;
	for (i = 0; i<20; i++)
	{
		char *css_len;
		css_len = &css[i].right[0];
		css[i].len = strlen(css_len);
	}
	css[1].len = 6;
	css[4].len = 3;
	css[10].len = 1;
	css[11].len = 3;
	css[12].len = 3;
	css[13].len = 3;
	css[14].len = 4;
	css[15].len = 4;
	//初始化产生式
	for (i = 0; i<46; i++)
	{
		for (j = 0; j<18; j++)
			action[i][j].sr = '#';
	}//初始化action表
	for (i = 0; i<46; i++)
	{
		for (j = 0; j<11; j++)
			go_to[i][j] = -1;
	}//初始化go_to表
	 //给action表和go_to表赋初值
	action[0][0].sr = 's'; action[0][0].state = 2;
	action[1][17].sr = '@';//结束
	action[2][1].sr = 's'; action[2][1].state = 3;
	action[3][2].sr = 's'; action[3][2].state = 4;
	action[4][0].sr = 's'; action[4][0].state = 5;
	action[5][4].sr = 's'; action[5][4].state = 6;
	action[6][11].sr = 's'; action[6][11].state = 7;
	action[7][3].sr = 'r'; action[7][3].state = 4;
	action[8][3].sr = 'r'; action[8][3].state = 3;
	action[9][3].sr = 's'; action[9][3].state = 10;
	action[10][0].sr = 's'; action[10][0].state = 5;
	action[10][9].sr = 's'; action[10][9].state = 13;
	action[11][17].sr = 'r'; action[11][17].state = 1;
	action[12][3].sr = 'r'; action[12][3].state = 2;
	action[13][0].sr = 's'; action[13][0].state = 14;
	action[13][13].sr = 's'; action[13][13].state = 23;
	action[13][15].sr = 's'; action[13][15].state = 27;
	action[14][8].sr = 's'; action[14][8].state = 15;
	action[15][0].sr = 's'; action[15][0].state = 36;
	action[15][1].sr = 's'; action[15][1].state = 41;
	action[16][6].sr = 's'; action[16][6].state = 43;
	action[16][3].sr = 'r'; action[16][3].state = 13;
	action[16][10].sr = 'r'; action[16][10].state = 13;
	action[17][3].sr = 's'; action[17][3].state = 19;
	action[17][10].sr = 's'; action[17][10].state = 18;
	action[18][17].sr = 'r'; action[18][17].state = 19;
	action[19][0].sr = 's'; action[19][0].state = 14;
	action[19][13].sr = 's'; action[19][13].state = 23;
	action[19][15].sr = 's'; action[19][15].state = 27;
	action[20][3].sr = 'r'; action[20][3].state = 17;
	action[20][10].sr = 'r'; action[20][10].state = 17;
	action[21][3].sr = 'r'; action[21][3].state = 18;
	action[21][10].sr = 'r'; action[21][10].state = 18;
	action[22][3].sr = 'r'; action[22][3].state = 16;
	action[22][10].sr = 'r'; action[22][10].state = 16;
	action[23][0].sr = 's'; action[23][0].state = 31;
	action[24][12].sr = 's'; action[24][12].state = 34;
	action[24][14].sr = 's'; action[24][14].state = 25;
	action[25][0].sr = 's'; action[25][0].state = 14;
	action[26][3].sr = 'r'; action[26][3].state = 14;
	action[26][10].sr = 'r'; action[26][10].state = 14;
	action[27][0].sr = 's'; action[27][0].state = 31;
	action[28][12].sr = 's'; action[28][12].state = 34;
	action[28][16].sr = 's'; action[28][16].state = 29;
	action[29][0].sr = 's'; action[29][0].state = 14;
	action[30][3].sr = 'r'; action[30][3].state = 15;
	action[30][10].sr = 'r'; action[30][10].state = 15;
	action[31][7].sr = 's'; action[31][7].state = 32;
	action[32][0].sr = 's'; action[32][0].state = 33;
	action[33][12].sr = 'r'; action[33][12].state = 12;
	action[33][14].sr = 'r'; action[33][14].state = 12;
	action[33][16].sr = 'r'; action[33][16].state = 12;
	action[34][0].sr = 's'; action[34][0].state = 31;
	action[35][12].sr = 'r'; action[35][12].state = 11;
	action[35][14].sr = 'r'; action[35][14].state = 11;
	action[35][16].sr = 'r'; action[35][16].state = 11;
	action[36][2].sr = 'r'; action[36][2].state = 10;
	action[36][3].sr = 'r'; action[36][3].state = 10;
	action[36][5].sr = 'r'; action[36][5].state = 10;
	action[36][6].sr = 'r'; action[36][6].state = 10;
	action[36][10].sr = 'r'; action[36][10].state = 10;
	action[37][2].sr = 'r'; action[37][2].state = 8;
	action[37][3].sr = 'r'; action[37][3].state = 8;
	action[37][5].sr = 'r'; action[37][5].state = 8;
	action[37][6].sr = 'r'; action[37][6].state = 8;
	action[37][10].sr = 'r'; action[37][10].state = 8;
	action[38][2].sr = 'r'; action[38][2].state = 6;
	action[38][3].sr = 'r'; action[38][3].state = 6;
	action[38][5].sr = 's'; action[38][5].state = 39;
	action[38][6].sr = 'r'; action[38][6].state = 6;
	action[38][10].sr = 'r'; action[38][10].state = 6;
	action[39][0].sr = 's'; action[39][0].state = 36;
	action[39][1].sr = 's'; action[39][1].state = 41;
	action[40][2].sr = 'r'; action[40][2].state = 7;
	action[40][3].sr = 'r'; action[40][3].state = 7;
	action[40][5].sr = 'r'; action[40][5].state = 7;
	action[40][6].sr = 'r'; action[40][6].state = 7;
	action[40][10].sr = 'r'; action[40][10].state = 7;
	action[41][0].sr = 's'; action[41][0].state = 36;
	action[41][1].sr = 's'; action[41][1].state = 41;
	action[42][2].sr = 's'; action[42][2].state = 45;
	action[42][6].sr = 's'; action[42][6].state = 43;
	action[43][0].sr = 's'; action[43][0].state = 36;
	action[43][1].sr = 's'; action[43][1].state = 41;
	action[44][2].sr = 'r'; action[44][2].state = 5;
	action[44][3].sr = 'r'; action[44][3].state = 5;
	action[44][5].sr = 's'; action[44][5].state = 39;
	action[44][6].sr = 'r'; action[44][6].state = 5;
	action[44][10].sr = 'r'; action[44][10].state = 5;
	action[45][2].sr = 'r'; action[45][2].state = 9;
	action[45][3].sr = 'r'; action[45][3].state = 9;
	action[45][5].sr = 'r'; action[45][5].state = 9;
	action[45][6].sr = 'r'; action[45][6].state = 9;
	action[45][10].sr = 'r'; action[45][10].state = 9;

	go_to[0][0] = 1; go_to[4][1] = 8; go_to[4][9] = 9; go_to[10][1] = 12; go_to[10][2] = 11; go_to[13][7] = 22; go_to[13][8] = 21; go_to[13][10] = 17;
	go_to[15][3] = 16; go_to[15][4] = 38; go_to[15][5] = 37; go_to[19][7] = 20; go_to[19][8] = 20; go_to[23][6] = 24; go_to[25][7] = 26; go_to[27][6] = 28;
	go_to[29][7] = 30; go_to[34][6] = 35; go_to[39][5] = 40; go_to[41][3] = 42; go_to[41][4] = 38; go_to[41][5] = 37; go_to[43][4] = 44; go_to[43][5] = 37;

}
int id1(int i)//按action表，给输入字符编号
{
	int j;
	j = -1;
	if (i == 25) { j = 0; id_num++; }//设置变量名称标志,出现一个id则id_num++
	if (i == 1) { j = 8, id_left = id_num; }//设置产生试左边变量名称标志
	if (i == 2) j = 6;
	if (i == 3) j = 5;
	if (i == 4) j = 7;
	if (i == 5) j = 4;
	if (i == 6) j = 3;
	if (i == 7) j = 9;
	if (i == 8) j = 10;
	if (i == 9) j = 1;
	if (i == 10) j = 2;
	if (i == 31) j = 12;
	if (i == 32) j = 13;
	if (i == 33) { j = 14; id_then = L_four_tail->k + 1; }//设置if语句中then位置标志
	if (i == 35) { j = 15; id_while = L_four_tail->k + 1; }//设置while语句中while位置标志
	if (i == 36) { j = 16; id_do = L_four_tail->k + 1; }//设置while语句中do位置标志
	if (i == 37) j = 11;
	return(j);
}
string id10(int i)//反编号输入字符
{
	string ch;
	if (i == 0) ch = "id";
	if (i == 1) ch = "(";
	if (i == 2) ch = ")";
	if (i == 3) ch = ";";
	if (i == 4) ch = ":";
	if (i == 5) ch = "*";
	if (i == 6) ch = "+";
	if (i == 7) ch = ">";
	if (i == 8) ch = "=";
	if (i == 9) ch = "{";
	if (i == 10) ch = "}";
	if (i == 11) ch = "int";
	if (i == 12) ch = "and";
	if (i == 13) ch = "if";
	if (i == 14) ch = "then";
	if (i == 15) ch = "while";
	if (i == 16) ch = "do";
	if (i == 17) ch = "$";
	return(ch);
}
int id2(char ch)//按go_to表给非终结符编号
{
	int j;
	j = -1;
	if (ch == 'P') j = 0;
	if (ch == 'D') j = 1;
	if (ch == 'R') j = 2;
	if (ch == 'E') j = 3;
	if (ch == 'T') j = 4;
	if (ch == 'F') j = 5;
	if (ch == 'B') j = 6;
	if (ch == 'M') j = 7;
	if (ch == 'S') j = 8;
	if (ch == 'L') j = 9;
	if (ch == 'N') j = 10;
	return(j);
}
int id20(char ch)//给非终结符编号
{
	int j;
	j = -1;
	if (ch == 'P') j = 100;
	if (ch == 'D') j = 101;
	if (ch == 'R') j = 102;
	if (ch == 'E') j = 103;
	if (ch == 'T') j = 104;
	if (ch == 'F') j = 105;
	if (ch == 'B') j = 106;
	if (ch == 'M') j = 107;
	if (ch == 'S') j = 108;
	if (ch == 'L') j = 109;
	if (ch == 'N') j = 1010;
	return(j);
}
char id21(int j)//反编号非终结符
{
	char ch;
	if (j == 100 || j == 0) ch = 'P';
	if (j == 101 || j == 1) ch = 'D';
	if (j == 102 || j == 2) ch = 'R';
	if (j == 103 || j == 3) ch = 'E';
	if (j == 104 || j == 4) ch = 'T';
	if (j == 105 || j == 5) ch = 'F';
	if (j == 106 || j == 6) ch = 'B';
	if (j == 107 || j == 7) ch = 'M';
	if (j == 108 || j == 8) ch = 'S';
	if (j == 109 || j == 9) ch = 'L';
	if (j == 1010 || j == 10) ch = 'N';
	return(ch);
}
void add(ike *temp)//加一个结点
{
	if (stack_head->next == stack_tail)
	{
		temp->pre = stack_head;
		temp->next = stack_tail;
		stack_head->next = temp;
		stack_tail->pre = temp;
	}
	else
	{
		temp->pre = stack_tail->pre;
		temp->next = stack_tail;
		stack_tail->pre->next = temp;
		stack_tail->pre = temp;
	}
}
void del()//删除一个结点
{
	stack_tail->pre->pre->next = stack_tail;
	stack_tail->pre = stack_tail->pre->pre;
}
int GrammerSLR(int w)
{
	cout<<"当前输入："<<id10(w)<<"   ";
	int i, flag = 0, state_temp;//flag错误标志，0正常移进，1错误,2归约，3结束
	char sr_temp;
	sr_temp = action[stack_tail->pre->num][w].sr;//动作
	state_temp = action[stack_tail->pre->num][w].state;//状态变化
	if (sr_temp == '#')//错误动作
	{
		flag = 1;
		err = 3;
		cout << "语法分析出错!" << endl;
	}
	else if (sr_temp == 's')//移进动作
	{
		ike *temp;
		temp = new ike;
		temp->next = NULL;
		temp->pre = NULL;
		temp->word = w;
		temp->num = state_temp;
		add(temp);
		cout<<"动作（移进）：" << sr_temp << stack_tail->pre->pre->num << "    "<<"状态转为：s"<<stack_tail->pre->num<<"  "<<"栈顶符号："<<id10(stack_tail->pre->word) << endl;
		flag = 0;
	}
	else if (sr_temp == 'r')//归约动作
	{
		int p = id2(css[state_temp].left);
		int q = css[state_temp].len;
		for (i = 0; i<q; i++)
			del();
		ike *temp;
		temp = new ike;
		temp->next = NULL;
		temp->pre = NULL;
		temp->word = id20(css[state_temp].left);
		temp->num = go_to[stack_tail->pre->num][p];//查go_to表
		add(temp);
		cout<<"动作（归约）：" << sr_temp << state_temp << "  " << css[state_temp].left << "→" << css[state_temp].right << "    "<<"状态转为："<<stack_tail->pre->num<<"  "<<"栈顶符号："<<id21(stack_tail->pre->word) << endl;
		flag = 2;
		Semantic(state_temp);//在产生树的同时进行语义分析
	}
	else if (sr_temp == '@')//结束动作,AC
	{
		cout << "END"<<"动作（归约）："/*<<sr_temp<<state_temp */<< "  " << css[state_temp].left << "→" << css[state_temp].right << "    "<<"状态转为："<<stack_tail->pre->num<<"  "<<"栈顶符号："<<id21(stack_tail->pre->word) << endl;
		flag = 3;
		cout << "语法分析正确完成!" << endl;
	}
	else//其他意外情况
	{
		flag = 1;
		err = 3;
		cout << "语法分析出错!" << endl;
	}
	//cout << "------------------------------------------------------------------------"<<endl;
	return(flag);
}

//语义分析

void Semantic(int m)
{
	L *temp;
	int k;
	k = 1;
	temp = new L;
	temp->op = "  ";
	temp->op1 = "  ";
	temp->op2 = "  ";
	temp->result = "";
	temp->next = NULL;
	temp->Ltrue = NULL;
	temp->Lfalse = NULL;
	if (m == 4)//变量声明时加入符号表链
	{
		symb *Stemp;
		Stemp = new symb;
		id_name = id_numtoname(id_num);
		Stemp->word = id_name;
		Stemp->next = NULL;
		add_symb(Stemp);
	}
	if (m == 5)//归约E→E+T
	{
		temp->op = "+";
		temp->op1 = E_name;
		temp->op2 = T_name;
		yuyi_linshi++;//申请临时变量
		E_name = "t" + newop(yuyi_linshi);
		temp->result = E_name;
		add_L_four(temp);//加一个四元式结点
	}
	if (m == 6)//归约E→T
	{
		E_name = T_name;
	}
	if (m == 7)//归约T→T*F
	{
		temp->op = "*";
		temp->op1 = T_name;
		temp->op2 = F_name;
		yuyi_linshi++;//申请临时变量
		T_name = "t" + newop(yuyi_linshi);
		temp->result = T_name;
		add_L_four(temp);//加一个四元式结点
	}
	if (m == 8)//归约T→F
	{
		T_name = F_name;
	}
	if (m == 9)//归约F→(E)
	{
		F_name = E_name;
	}
	if (m == 10)//归约F→id
	{
		id_name = id_numtoname(id_num);
		F_name = id_name;
		k = lookup(id_name);//检查变量是否声明
		if (k == 0)
		{
			err = 2;
			errword = id_name;
			return;
		}
	}
	if (m == 12)//归约B→id>id
	{
		temp->op = "J>";
		id1_num = id_num - 1;
		id1_name = id_numtoname(id1_num);
		k = lookup(id1_name);//检查变量是否声明
		if (k == 0)
		{
			err = 2;
			errword = id1_name;
			return;
		}
		id2_num = id_num;
		id2_name = id_numtoname(id2_num);
		k = lookup(id2_name);//检查变量是否声明
		if (k == 0)
		{
			err = 2;
			errword = id2_name;
			return;
		}
		temp->result = "-1";
		temp->op1 = id1_name;
		temp->op2 = id2_name;
		add_L_four(temp);//加一个四元式结点
		add_L_true(temp);//加一个true链结点
		L *temp2;
		temp2 = new L;
		temp2->op = "J";
		temp2->op1 = "  ";
		temp2->op2 = "  ";
		temp2->result = "-1";
		add_L_four(temp2);//加一个四元式结点
		add_L_false(temp2);//加一个false链结点
	}
	if (m == 13)//归约M→id=E
	{
		temp->op = "=";
		temp->op1 = E_name;
		temp->op2 = "  ";
		id_name = id_numtoname(id_left);
		temp->result = id_name;
		add_L_four(temp);//加一个四元式结点
		yuyi_linshi = -1;//临时变量开始重新计数
	}
	if (m == 14)//归约S→if B then M
	{
		int a;
		a = id_then;
		temp = L_true_head->Ltrue;
		while (temp != NULL)
		{
			temp->result = "L" + newop(a);
			a = temp->k;
			temp = temp->Ltrue;
		}
		a = L_four_tail->k + 1;
		temp = L_false_head->Lfalse;
		while (temp != NULL)
		{
			temp->result = "L" + newop(a);
			temp = temp->Lfalse;
		}
		L_true_head->Ltrue = NULL;
		L_false_head->Lfalse = NULL;//回填并清空true链和false链
	}
	if (m == 15)//归约S→while B do M
	{
		int a;
		a = id_do;
		temp = L_true_head->Ltrue;
		while (temp != NULL)
		{
			temp->result = "L" + newop(a);
			a = temp->k;
			temp = temp->Ltrue;
		}
		a = L_four_tail->k + 2;
		temp = L_false_head->Lfalse;
		while (temp != NULL)
		{
			temp->result = "L" + newop(a);
			temp = temp->Lfalse;
		}
		L *temp1;
		temp1 = new L;
		temp1->op = "J";
		temp1->op1 = "  ";
		temp1->op2 = "  ";
		temp1->next = NULL;
		temp1->result = "L" + newop(id_while);
		add_L_four(temp1);//加一个四元式结点
		L_true_head->Ltrue = NULL;
		L_false_head->Lfalse = NULL;//回填并清空true链和false链
	}
}
string newop(int m)//数字变成字符串
{
	int shang, yushu;
	string chuan, chuan1;
	shang = m;
	chuan = "";
	while (1)
	{
		yushu = shang % 10;
		chuan = chuan + char(48 + yushu);
		shang = shang / 10;
		if (shang == 0)
			break;
	}
	int i;
	char *ch;
	ch = &chuan[0];
	chuan1 = "";
	for (i = strlen(ch) - 1; i >= 0; i--)
		chuan1 = chuan1 + chuan[i];
	return(chuan1);
}
void add_L_four(L *temp)//加一个四元式结点
{
	temp->k = L_four_tail->k + 1;
	if (L_four_head->next == NULL)
	{
		L_four_head->next = temp;
		L_four_tail->next = temp;
	}
	else
	{
		L_four_tail->next->next = temp;
		L_four_tail->next = temp;
	}
	L_four_tail->k = L_four_tail->next->k;
}
void add_L_true(L *temp)//加一个true链结点
{
	temp->Ltrue = L_true_head->Ltrue;
	L_true_head->Ltrue = temp;

}
void add_L_false(L *temp)//加一个false链结点
{
	temp->Lfalse = L_false_head->Lfalse;
	L_false_head->Lfalse = temp;
}
void add_symb(symb *temp)//加一个语义符号表链结点
{
	if (symb_head->next == NULL)
	{
		temp->addr = 0;
		symb_head->next = temp;
		symb_tail->next = temp;
	}
	else
	{
		temp->addr = symb_tail->next->addr + 4;
		symb_tail->next->next = temp;
		symb_tail->next = temp;
	}
}
void output_yuyi()
{
	if (err == 0)//语义分析正确时的输出
	{
		cout << endl;
		system("pause");
		cout << endl;
		cout << "*  语义分析  *\n" << endl;
		cout << "中间代码如下：" << endl;
		L *temp;
		temp = L_four_head->next;
		while (temp != NULL)
		{
			cout << "L" << temp->k - 1 << ": [ " << temp->op << ",  " << temp->op1 << ",  " << temp->op2 << ",  " << temp->result << " ]" << endl;
			temp = temp->next;
		}

	}
	if (err == 2)//语义分析错误时的输出
	{
		cout << endl;
		system("pause");
		cout << endl;
		cout << "*  语义分析  *\n" << endl;
		cout << "语义分析出错：变量" << errword << "未声明!" << endl;
	}
}
string id_numtoname(int num)//根据编号找变量的名称
{
	str *temp;
	string name;
	temp = string_head->next;
	while (temp != NULL)
	{
		if (num == temp->num)
		{
			name = temp->word;
			break;
		}
		temp = temp->next;
	}
	return(name);
}
int lookup(string m)//检查变量是否已经声明
{
	symb *temp;
	int i;
	i = 0;
	temp = new symb;
	temp = symb_head->next;
	while (temp != NULL)
	{
		if (m == temp->word)
		{
			i = 1;
			break;
		}
		temp = temp->next;
	}
	return(i);
}