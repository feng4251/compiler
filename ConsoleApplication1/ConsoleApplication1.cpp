// ConsoleApplication1.cpp : �������̨Ӧ�ó������ڵ㡣
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


struct TOKEN//�ʷ� TOKEN�ṹ��
{
	int code;//����,id
	int num;//�������
	TOKEN *next;
};
TOKEN *TOKEN_head, *TOKEN_tail;//TOKEN����
struct str//�ʷ� ���ű�ṹ��
{
	int num;//���
	string  word;//�ַ�������
	str *next;
};
str *string_head, *string_tail;//string����
struct generor//�﷨ ����ʽ�ṹ��
{
	char left;//����ʽ����
	string right;//����ʽ���Ҳ�
	int len;//����ʽ�Ҳ��ĳ���
};
generor css[20];//�﷨ 20������ʽ
struct actionStruct//�﷨ action��ṹ��
{
	char sr;//�ƽ����Լ
	int state;//ת����״̬���
};
actionStruct action[46][18];//action��
int go_to[46][11];//�﷨ go_to��
struct ike//�﷨ ����ջ�ṹ�壬˫��
{
	ike *pre;
	int num;//״̬
	int word;//���ű���
	ike *next;
};
ike *stack_head, *stack_tail;//����ջ��βָ��
struct L//������Ԫʽ�����ݽṹ
{
	int k;
	string op;//������
	string op1;//������
	string op2;//������
	string result;//���
	L *next;//������Ԫʽ���ָ��
	L *Ltrue;//����true����ǰָ��
	L *Lfalse;//����false����ǰָ��
};
L *L_four_head, *L_four_tail, *L_true_head, *L_false_head;//��Ԫʽ����true����false��
struct symb//��������ʱ���ű�
{
	string word;//��������
	int addr;//������ַ
	symb *next;
};
symb *symb_head, *symb_tail;//�����������

							//�ʷ�������ص�һЩ����������

void scan();//���ַ���ȡԴ�ļ�
void lexical();//�ʷ�����������
int JUDGE(char ch);//�ж������ַ�������
void OutputOne(char ch);//д��TOKEN.txt
void OutputT(char ch, string word);//д��string.txt
void InOne(TOKEN *temp);//�����㵽����TOKEN
void InT(str *temp);//�����㵽����string
void output();//����������е�����
void outfile();//����������е����ݵ���Ӧ�ļ���

			   //�﷨������ص�һЩ����

void Grammer();//�﷨����������
void GrammerInit();//��ʼ���﷨�������ݽṹ
int GrammerSLR(int a);//�﷨�������岿��
int id1(int a);//�������ַ���ţ�ת����action���б��
string id10(int i);//�������ַ������
int id2(char ch);//�����ս�״̬��ţ�ת����go_to���б��
int id20(char ch);//�����ս�״̬���
char id21(int j);//�����ս�״̬�����
void add(ike *temp);//��ike����ջ��������һ�����
void del();//��ike����ջ����ɾ��һ�����

		   //���������ص�һЩ����

void Semantic(int m);//�������������
void add_L_four(L *temp);//����Ԫʽ���м�һ�����
void add_L_true(L *temp);//��true���м�һ�����
void add_L_false(L *temp);//��false���м�һ�����
void add_symb(symb *temp);//��������ű����м�һ�����
void output_yuyi();//����м������Ԫʽ�������ű�
string newop(int m);//�����ֱ���ַ���
string id_numtoname(int num);//�ѱ��ת������Ӧ�ı�����
int lookup(string m);//�����������

					 //ȫ�ֱ���������

FILE *fp;//�ļ�ָ��
int wordcount;//��־������
int err;//��־�ʷ����������ȷ�����
int nl;//��ȡ����
int yuyi_linshi;//������ʱ����
string E_name, T_name, F_name, M_name, id_name, id1_name, id2_name, errword;//���ڹ�Լʱ���ƴ��ݺ�δ�������������
int id_num, id1_num, id2_num, id_left, id_while, id_then, id_do;//���ڼ�¼һЩ������ַ�λ����Ϣ



int main()
{
	//	cout<<"*  �Ը����ļ����ν��дʷ��������﷨��������������� *\n"<<endl;
	lexical();//�ʷ�
	Grammer();//�﷨
	output_yuyi();//����
	cout << endl;
	system("pause");
	return(0);
}

//�ʷ�����

void lexical()
{
	TOKEN_head = new TOKEN;
	TOKEN_head->next = NULL;
	TOKEN_tail = new TOKEN;
	TOKEN_tail->next = NULL;
	string_head = new str;
	string_head->next = NULL;
	string_tail = new str;
	string_tail->next = NULL;//��ʼ���������е���βָ��
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
	wordcount = 0;//��ʼ���ַ�������
	err = 0;//��ʼ���ʷ����������־
	nl = 1;//��ʼ����ȡ����
	scan();
	if (err == 0)
	{
		char m;
		output();
		//	cout<<"�ʷ�������ȷ���!"<<endl<<endl<<"�����������浽�ļ��������� y ������������������ĸ��";
		//	cin>>m;
		//	cout<<endl;
		//	if(m=='y')
		//	{
		outfile();
		cout << "����ɹ�������token.txt��string.txt�����ļ���" << endl;
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
	cout << "������Դ�ļ�·�������ƣ���ǰĿ¼���пɲ����ļ�test.txt��:";
	cin >> document;
	cout << endl;
	cout << "�ʷ���������\n" << endl;

	if ((fp = fopen(document, "rt")) == NULL)
	{
		err = 1;
		cout << "�޷��ҵ����ļ�!" << endl;
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
			cout << nl << "��  " << "����:�Ƿ��ַ�!  " << ch << endl;
			err = 1;
		}
	}
	fclose(fp);
}
int JUDGE(char ch)
{
	int flag = 0;
	if (ch == '=' || ch == '+' || ch == '*' || ch == '>' || ch == ':' || ch == ';' || ch == '{' || ch == '}' || ch == '(' || ch == ')')
		flag = 1;//���
	else if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z'))
		flag = 3;//��ĸ
	else if (ch == ' ')
		flag = 4;//�ո�
	else if (feof(fp))
		flag = 5;//����
	else if (ch == '\n')
	{
		flag = 6;//����
		nl++;
	}
	else
		flag = 0;//�Ƿ��ַ�
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
	case ')': id = 10; break;//�������
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
				temp->code = 37;//�ؼ��ֱ���
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
		OutputT(ch, word);//�γ��ַ���
	else
	{
		err = 1;
		cout << nl << "��  " << "����:�Ƿ��ַ�!  " << ch << endl;
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
	cout << "TOKEN���������£�" << endl;
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
	cout << "���ű��������£�" << endl;
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
	ofstream fOutputOne("TOKEN.txt");//д�ļ�
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

//�﷨����

void Grammer()
{
	if (err == 0)
	{
		system("pause");
		cout << endl;
		cout << "*  �﷨���� *\n" << endl;
		GrammerInit();//��ʼ���﷨�������ݽṹ
		TOKEN *temp;
		temp = new TOKEN;
		temp = TOKEN_head->next;
		int p, q;
		p = 0;
		q = 0;
		cout << "�﷨�����������£�" << endl;
		while (temp != NULL)
		{
			int w;
			w = id1(temp->code);
			p = GrammerSLR(w);
			if (p == 1) break;
			if (p == 0)
				temp = temp->next;
			if (temp == NULL) q = 1;
		}//�﷨����
		if (q == 1)
			while (1)
			{
				p = GrammerSLR(17);
				if (p == 3) break;
			}//�������$������﷨����
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
	//��ʼ��ջ��������
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
	//��ʼ������ʽ
	for (i = 0; i<46; i++)
	{
		for (j = 0; j<18; j++)
			action[i][j].sr = '#';
	}//��ʼ��action��
	for (i = 0; i<46; i++)
	{
		for (j = 0; j<11; j++)
			go_to[i][j] = -1;
	}//��ʼ��go_to��
	 //��action���go_to����ֵ
	action[0][0].sr = 's'; action[0][0].state = 2;
	action[1][17].sr = '@';//����
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
int id1(int i)//��action���������ַ����
{
	int j;
	j = -1;
	if (i == 25) { j = 0; id_num++; }//���ñ������Ʊ�־,����һ��id��id_num++
	if (i == 1) { j = 8, id_left = id_num; }//���ò�������߱������Ʊ�־
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
	if (i == 33) { j = 14; id_then = L_four_tail->k + 1; }//����if�����thenλ�ñ�־
	if (i == 35) { j = 15; id_while = L_four_tail->k + 1; }//����while�����whileλ�ñ�־
	if (i == 36) { j = 16; id_do = L_four_tail->k + 1; }//����while�����doλ�ñ�־
	if (i == 37) j = 11;
	return(j);
}
string id10(int i)//����������ַ�
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
int id2(char ch)//��go_to������ս�����
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
int id20(char ch)//�����ս�����
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
char id21(int j)//����ŷ��ս��
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
void add(ike *temp)//��һ�����
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
void del()//ɾ��һ�����
{
	stack_tail->pre->pre->next = stack_tail;
	stack_tail->pre = stack_tail->pre->pre;
}
int GrammerSLR(int w)
{
	cout<<"��ǰ���룺"<<id10(w)<<"   ";
	int i, flag = 0, state_temp;//flag�����־��0�����ƽ���1����,2��Լ��3����
	char sr_temp;
	sr_temp = action[stack_tail->pre->num][w].sr;//����
	state_temp = action[stack_tail->pre->num][w].state;//״̬�仯
	if (sr_temp == '#')//������
	{
		flag = 1;
		err = 3;
		cout << "�﷨��������!" << endl;
	}
	else if (sr_temp == 's')//�ƽ�����
	{
		ike *temp;
		temp = new ike;
		temp->next = NULL;
		temp->pre = NULL;
		temp->word = w;
		temp->num = state_temp;
		add(temp);
		cout<<"�������ƽ�����" << sr_temp << stack_tail->pre->pre->num << "    "<<"״̬תΪ��s"<<stack_tail->pre->num<<"  "<<"ջ�����ţ�"<<id10(stack_tail->pre->word) << endl;
		flag = 0;
	}
	else if (sr_temp == 'r')//��Լ����
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
		temp->num = go_to[stack_tail->pre->num][p];//��go_to��
		add(temp);
		cout<<"��������Լ����" << sr_temp << state_temp << "  " << css[state_temp].left << "��" << css[state_temp].right << "    "<<"״̬תΪ��"<<stack_tail->pre->num<<"  "<<"ջ�����ţ�"<<id21(stack_tail->pre->word) << endl;
		flag = 2;
		Semantic(state_temp);//�ڲ�������ͬʱ�����������
	}
	else if (sr_temp == '@')//��������,AC
	{
		cout << "END"<<"��������Լ����"/*<<sr_temp<<state_temp */<< "  " << css[state_temp].left << "��" << css[state_temp].right << "    "<<"״̬תΪ��"<<stack_tail->pre->num<<"  "<<"ջ�����ţ�"<<id21(stack_tail->pre->word) << endl;
		flag = 3;
		cout << "�﷨������ȷ���!" << endl;
	}
	else//�����������
	{
		flag = 1;
		err = 3;
		cout << "�﷨��������!" << endl;
	}
	//cout << "------------------------------------------------------------------------"<<endl;
	return(flag);
}

//�������

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
	if (m == 4)//��������ʱ������ű���
	{
		symb *Stemp;
		Stemp = new symb;
		id_name = id_numtoname(id_num);
		Stemp->word = id_name;
		Stemp->next = NULL;
		add_symb(Stemp);
	}
	if (m == 5)//��ԼE��E+T
	{
		temp->op = "+";
		temp->op1 = E_name;
		temp->op2 = T_name;
		yuyi_linshi++;//������ʱ����
		E_name = "t" + newop(yuyi_linshi);
		temp->result = E_name;
		add_L_four(temp);//��һ����Ԫʽ���
	}
	if (m == 6)//��ԼE��T
	{
		E_name = T_name;
	}
	if (m == 7)//��ԼT��T*F
	{
		temp->op = "*";
		temp->op1 = T_name;
		temp->op2 = F_name;
		yuyi_linshi++;//������ʱ����
		T_name = "t" + newop(yuyi_linshi);
		temp->result = T_name;
		add_L_four(temp);//��һ����Ԫʽ���
	}
	if (m == 8)//��ԼT��F
	{
		T_name = F_name;
	}
	if (m == 9)//��ԼF��(E)
	{
		F_name = E_name;
	}
	if (m == 10)//��ԼF��id
	{
		id_name = id_numtoname(id_num);
		F_name = id_name;
		k = lookup(id_name);//�������Ƿ�����
		if (k == 0)
		{
			err = 2;
			errword = id_name;
			return;
		}
	}
	if (m == 12)//��ԼB��id>id
	{
		temp->op = "J>";
		id1_num = id_num - 1;
		id1_name = id_numtoname(id1_num);
		k = lookup(id1_name);//�������Ƿ�����
		if (k == 0)
		{
			err = 2;
			errword = id1_name;
			return;
		}
		id2_num = id_num;
		id2_name = id_numtoname(id2_num);
		k = lookup(id2_name);//�������Ƿ�����
		if (k == 0)
		{
			err = 2;
			errword = id2_name;
			return;
		}
		temp->result = "-1";
		temp->op1 = id1_name;
		temp->op2 = id2_name;
		add_L_four(temp);//��һ����Ԫʽ���
		add_L_true(temp);//��һ��true�����
		L *temp2;
		temp2 = new L;
		temp2->op = "J";
		temp2->op1 = "  ";
		temp2->op2 = "  ";
		temp2->result = "-1";
		add_L_four(temp2);//��һ����Ԫʽ���
		add_L_false(temp2);//��һ��false�����
	}
	if (m == 13)//��ԼM��id=E
	{
		temp->op = "=";
		temp->op1 = E_name;
		temp->op2 = "  ";
		id_name = id_numtoname(id_left);
		temp->result = id_name;
		add_L_four(temp);//��һ����Ԫʽ���
		yuyi_linshi = -1;//��ʱ������ʼ���¼���
	}
	if (m == 14)//��ԼS��if B then M
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
		L_false_head->Lfalse = NULL;//������true����false��
	}
	if (m == 15)//��ԼS��while B do M
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
		add_L_four(temp1);//��һ����Ԫʽ���
		L_true_head->Ltrue = NULL;
		L_false_head->Lfalse = NULL;//������true����false��
	}
}
string newop(int m)//���ֱ���ַ���
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
void add_L_four(L *temp)//��һ����Ԫʽ���
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
void add_L_true(L *temp)//��һ��true�����
{
	temp->Ltrue = L_true_head->Ltrue;
	L_true_head->Ltrue = temp;

}
void add_L_false(L *temp)//��һ��false�����
{
	temp->Lfalse = L_false_head->Lfalse;
	L_false_head->Lfalse = temp;
}
void add_symb(symb *temp)//��һ��������ű������
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
	if (err == 0)//���������ȷʱ�����
	{
		cout << endl;
		system("pause");
		cout << endl;
		cout << "*  �������  *\n" << endl;
		cout << "�м�������£�" << endl;
		L *temp;
		temp = L_four_head->next;
		while (temp != NULL)
		{
			cout << "L" << temp->k - 1 << ": [ " << temp->op << ",  " << temp->op1 << ",  " << temp->op2 << ",  " << temp->result << " ]" << endl;
			temp = temp->next;
		}

	}
	if (err == 2)//�����������ʱ�����
	{
		cout << endl;
		system("pause");
		cout << endl;
		cout << "*  �������  *\n" << endl;
		cout << "���������������" << errword << "δ����!" << endl;
	}
}
string id_numtoname(int num)//���ݱ���ұ���������
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
int lookup(string m)//�������Ƿ��Ѿ�����
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