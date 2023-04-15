/*
改写之后的文法：
expr -> term  rest | (expr)
rest -> + term rest | - term rest | epsilon 
term -> num | id | num.num | num.E(+|-)num

输入样例：
10 + 20 - a + b+temp
输出样例： 
10 20 + a - b + temp +

需要你补充完成的：
1. gettoken()的功能，识别浮点数
2. *、/运算及配对括号
 
*/

#include<stdio.h> 
#include<ctype.h>
#include<stack>

#define FLOAT 550 
#define NUM 500
#define ID 600
#define E 1321
#define E1 231



//全局量<tokenname, tokenvalue>  
int lookahead;   		//存放当前的词法单元的类别，即tokenname 
int tokenval = NULL;	//
char lexeme[1024];		//当前的词法单元对应的词素（字符串 ） 
int pretoken = NULL;


void expr(); 
void term();
void rest();
void factor();
void puttoken(int token);
int gettoken(); 

int gettoken()			//词法分析函数，返回值：当前词法单元的类型 
{
	char t;
	int i; 
	
	while (1) {
		t = getchar();
		if (t == ' ' || t == '\t' ) // 空格或者换行符->过滤掉 
			;
		else if (t == '\n') { // 因为在本程序中，回车符作为表达式的结束，
			return t;
		} 	
								// 所以不能把回车符当成空白符过滤 
		else if (isdigit(t)) {		//识别整型数值常量 
			tokenval = 0;
			while (isdigit(t)) {
				tokenval = tokenval * 10 + t - '0';
				t = getchar();
			}
			if (t == '.') return FLOAT; // 识别到小数点 
			if (t == 'E') {				// 识别到指数 
				t = getchar();
				if(t == '-') return E1;
				ungetc(t, stdin);
				return E;
			}
			ungetc(t, stdin);
			return NUM;
		}
		else if( isalpha(t)) {		//识别标识符 
             i=0; 
             do {
                 lexeme[i++]=t; t = getchar(); 
             }while( isalpha(t) || isdigit(t) );
             lexeme[i]='\0'; 
			 ungetc(t, stdin);
             return ID;
         }
		else {			//识别其他单个字符，例如 + - * / 的ASCII值作为tokenname
			tokenval = NULL;
			return t;
		}
	}
}

void puttoken(int token)
{
	if (token == NUM)
		printf("%d ", tokenval);
	else if (token == FLOAT)
		printf("%d.", tokenval);
	else if (token == ID)
		printf("%s ", lexeme);
	else if (token == E)
		printf("%dE", tokenval);
	else if (token == E1)
		printf("%dE-", tokenval);		
	else
		printf("%c ", (char)token);				
}
 
void error(char *msg)
{
	printf("%s\n", msg);
}

void match(int token)
{
	if (lookahead == token) 
		lookahead = gettoken(); 
	else
		error("syntax error");
}

/*
expr -> term rest 
rest -> + term rest | - term rest | epsilon 
term -> num | id 
*/
void expr()
{
	factor();
    term();  //先执行term，消除左递归
	rest();
}

void rest( )
{
    if (lookahead == '+')
    {
        match('+');
		factor();
        term();
        puttoken('+');
		rest();
    }
    if (lookahead == '-')
    {
        match('-');
		factor();
        term();
        puttoken('-');
		rest();
    }
    else ;
}

void term( )
{
   	if (lookahead == '*') {
   		match('*');  
		factor();
		puttoken('*');
		term();
	}
   	else if (lookahead == '/') {
   		match('/');   
		factor();
		puttoken('/'); 
		term();
	}
   	else ;
}

void factor()
{
	if (lookahead == NUM) {
		puttoken(lookahead); match(NUM); 
	}
	else if (lookahead == FLOAT) {
		puttoken(lookahead); match(FLOAT);
		factor();
	}
	//浮点数包括两个factor 
	else if (lookahead == E) {
		puttoken(lookahead); match(E);
		factor(); 
	}
	else if (lookahead == E1) {
		puttoken(lookahead); match(E1);
		factor();
	}
	else if (lookahead == ID) {
		puttoken(lookahead); match(ID);
	}
	else if (lookahead == '(') {
		match('(');
		expr();
		match(')');
	}
    else error("syntax error");
}
// (10+20)*a-temp 
// (10+20.5)*a-temp
// (10.2345+20.5E-10)*a-temp
// a * (10.2345 + 20.5E-10)  - temp
int main()
{
	lookahead = gettoken();
	expr();
	return 0;
}
