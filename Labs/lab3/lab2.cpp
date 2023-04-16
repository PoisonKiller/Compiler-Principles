/*
文法
终结符： ID、NUM、IF、'{'、'}'  ';' '=', '<', '>', '<=', 
        '>=', '==', '!=', '+', '-', '*', '/', '(', ')', 
        'int', 'float', 'char', 'else', 'while', 'do', 'break'

1 program -> block
2 block -> '{' decls stmts '}'
3 decls -> decl decls
4        | epsilon
5 decl -> type ID ';'

5 stmts -> stmt stmts
6       | epsilon

7 stmt -> ID '=' expr ';'
8		  | IF (bool) stmt ElseStmt
9         | WHILE (bool) stmt
10        | DO stmt WHILE (bool) ';'
11        | break ';'
12        | block

13 ElseStmt -> ELSE stmt
14          | epsilon

15  bool -> expr boolR

16  boolR -> < expr
17        | > expr
18        | <= expr
19        | >= expr
20        | == expr
21        | != expr
22        | epsilon

23  expr -> term exprR

24  exprR -> + term exprR
25        | - term exprR
26        | epsilon

27  term -> factor termR

28  termR -> * factor termR
29        | / factor termR
30        | epsilon

31  factor -> (expr)| ID | NUM
32  type -> INT | FLOAT | CHAR




FIRST集
program -> {
block -> {
decls -> int float char epsilon
decl -> int float char
stmts -> ID IF WHILE DO break { epsilon
stmt -> ID IF WHILE DO break { 
ElseStmt -> else epsilon
bool -> ID NUM (
boolR -> < > <= >= == != epsilon
expr -> ID NUM (
exprR -> + - epsilon
term -> ID NUM (
termR -> * / epsilon
factor -> ID NUM (
type -> int float char

follow集
program -> $
block -> $ } ID IF WHILE DO break {
decls -> ID IF WHILE DO break { 
decl -> ID IF WHILE DO break { int char float
stmts -> } 
stmt -> } ID IF WHILE DO break {
ElseStmt -> ID IF WHILE DO break { 
bool -> )
boolR -> )
expr -> ) ; < > <= >= == != 
exprR -> ; < > <= >= == != 
term -> + - ; < > <= >= == != 
termR -> + - ; < > <= >= == != 
factor -> * / + - ; < > <= >= == != 
type -> ID

*/ 


#include <iostream>
#include <fstream>
#include "lex.yy.c"    //由flex .l文件得到 
using namespace std;

TOKEN lookahead;

void program();
void block();
void decls();
void decl();
void stmts();
void stmt();
void ElseStmt();
void type();
void Bool();
void BoolR();
void expr();
void exprR();
void term();
void termR();
void factor();
void type();


void error(int no)
{
	printf("error: %d\n", no);	
} 

void match(TOKEN t)
{
	if (lookahead == t)	
		lookahead = yylex();
	else
		printf("token %d %c is needed\n", t, t);
} 

void program()
{
	printf("program -> block\n");
	block();
}

void block()
{
	printf("block -> '{' decls stmts '}'\n");
	if (lookahead == '{'){
		match('{'); decls(); stmts();match('}');
	}
	else
		error(2);
}

void decls()
{
    if (lookahead == INT || lookahead == FLOAT || lookahead == CHAR){
        printf("decls -> decl decls\n");
        decl(); decls();
    }
    else if (lookahead == ID || lookahead == IF 
        || lookahead == WHILE || lookahead == DO 
        || lookahead == BREAK || lookahead == '{'
        || lookahead == '}'){
        printf("decls -> epsilon\n");
        ;
    }
    else
        error(3);
}

void decl()
{
    if(lookahead == INT || lookahead == FLOAT) {
        printf("decl -> type ID ';'\n");
        type(); match(ID); match(';');
    }
    else
        error(4);
}

void stmts()
{
	//5 stmts -> stmt stmts 
	if (lookahead == ID || lookahead == IF 
        || lookahead == WHILE || lookahead == DO 
        || lookahead == BREAK || lookahead == '{'){
		printf("stmts -> stmt stmts \n");
		stmt(); stmts();
	}
	else if (lookahead == '}'){
		printf("stmts -> epsilon \n");
	}
	else
		error(5);
	
}


void stmt()
{
	if (lookahead == ID){
		printf("stmt -> ID '=' expr ';'\n");
		match(ID); match('='); expr(); match(';');
	}
	else if (lookahead == IF){
		printf("stmt -> IF (bool) stmt Elsestmt\n");
		match(IF); match('('); Bool(); match(')'); stmt(); ElseStmt();
	}
    else if(lookahead == WHILE){
        printf("stmt -> WHILE (bool) stmt\n");
        match(WHILE); match('('); Bool(); match(')'); stmt();
    }
    else if(lookahead == DO){
        printf("stmt -> DO stmt WHILE (bool) ';'\n");
        match(DO); stmt(); match(WHILE); match('('); Bool(); match(')'); match(';');
    }
    else if(lookahead == BREAK){
        printf("stmt -> break ';'\n");
        match(BREAK); match(';');
    }
    else if(lookahead == '{'){
        printf("stmt -> block\n");
        block();
    }
	else
		error(6);
}

void ElseStmt()
{
    if (lookahead == ELSE){
        printf("ElseStmt -> ELSE stmt\n");
        match(ELSE); stmt();
    }
    else if (lookahead == ID || lookahead == IF 
        || lookahead == WHILE || lookahead == DO 
        || lookahead == BREAK || lookahead == '{'
        || lookahead == '}'){
        printf("ElseStmt -> epsilon\n");
        ;
    }
    else
        error(7);
}

void type()
{
	if (lookahead == INT){
		printf("type -> INT \n");
		match(INT);
	}
	else if (lookahead == FLOAT){
		printf("type -> FLOAT\n");
		match(FLOAT);
	}
    else if (lookahead == CHAR){
        printf("type -> CHAR\n");
        match(CHAR);
    }
	else
		error(8);
} 

void Bool()
{
    if (lookahead == ID || lookahead == NUM || lookahead == '('){
        printf("bool -> expr boolR\n");
        expr(); BoolR();
    }
    else
        error(9);


}

void BoolR()
{
    if (lookahead == '<' || lookahead == '>' 
        || lookahead == LE || lookahead == GE 
        || lookahead == EQ || lookahead == NE){
        printf("boolR -> < expr\n");
        match(lookahead); expr();
    }
    else if (lookahead == ')' || lookahead == ';' || lookahead == '{' || lookahead == '}'){
        printf("boolR -> epsilon\n");
    }
    else
        error(10);
}

void expr()
{
    // printf("lookahead = %d\n", lookahead);
    if (lookahead == ID || lookahead == NUM || lookahead == '('){
        printf("expr -> term exprR\n");
        term(); exprR();
    }
    else
        error(11);
}

void exprR()
{
    if (lookahead == '+'){
        printf("exprR -> + term exprR\n");
        match(lookahead);term(); exprR();
    }
    else if (lookahead == '-'){
        printf("exprR -> - term exprR\n");
        match(lookahead);term(); exprR();
    }
    
    else if (lookahead == ')' || lookahead == ';' || lookahead == '<' || lookahead == '>' 
            || lookahead == LE || lookahead == GE || lookahead == EQ || lookahead == NE){
        
        printf("exprR -> epsilon\n");
    }
    else
        error(12);
}

void term()
{
    if (lookahead == ID || lookahead == NUM || lookahead == '('){
        printf("term -> factor termR\n");
        factor(); termR();
    }
    else
        error(13);
}

void termR()
{
    if (lookahead == '*'){
        printf("termR -> * factor termR\n");
        match(lookahead);factor(); termR();
    }
    else if (lookahead == '/'){
        printf("termR -> / factor termR\n");
        match(lookahead);factor(); termR();
    }
    else if (lookahead == ')' || lookahead == ';' || lookahead == '<' || lookahead == '>' 
            || lookahead == LE || lookahead == GE || lookahead == EQ || lookahead == NE
            || lookahead == '+' || lookahead == '-'){
        printf("termR -> epsilon\n");
    }
    else
        error(14);
}

void factor()
{
    if (lookahead == ID){
        printf("factor -> ID\n");
        match(ID);
    }
    else if (lookahead == NUM){
        printf("factor -> NUM\n");
        match(NUM);
    }
    else if (lookahead == '('){
        printf("factor -> (expr)\n");
        match('('); expr(); match(')');
    }
    else
        error(15);
}

void Parse()
{
	lookahead = yylex();
	program();  //start symbol
}

int main()
{
    // 支持从文件读取
    char filename[1000] = "";
    printf("Enter the filename: ");
    scanf("%s", &filename);
    printf("\n");
    freopen(filename, "r", stdin);
 	Parse();
    return 0;
}

