%{
//终结符 
#define IF		100
#define THEN	101
#define ELSE	102
#define ID		103
#define NUM	104
#define INT	105
#define FLOAT	106
#define CHAR 110
#define WHILE 107
#define DO 108
#define BREAK 109

#define RELOP 1000
#define LT 1001
#define LE 1002
#define EQ 1003
#define NE 1004
#define GT 1005
#define GE 1006
#define OP 1007


typedef int TOKEN;

int yylval;

%}

delim		[ \t\n]
ws		{delim}+
letter		[A-Za-z]
digit		[0-9]
id		{letter}({letter}|{digit})*
number		-?{digit}+(\.{digit}+)?(E[+-]?{digit}+)?			
flnumber 	-?{digit}*\.{digit}+
fenumber	-?{digit}+(\.{digit}*)?([eE][+-]?{digit}+)?
op		[+\-*/=]
delimiter		[{}(),]
%option yylineno
%%

{ws}				{;}
\/\/([^\n])+	{	printf("Line %d line comments : %s\n", yylineno, yytext);}

\/\*([^\*])*\*([\*]|[^\*\/]([^\*])*[\*])*\/      { printf("Line %d\tComments:%s\n", yylineno, yytext); }

"if"		{ return IF;}
"else"		{ return ELSE;}
"int"		{ return INT;}
"float"		{ return FLOAT;}
"char"      { return CHAR;}
"do"        { return DO;}
"while"     { return WHILE;}
"then"      { return THEN;}
"break"     { return BREAK;}
{id}		{ return ID;}
{number}	{ return NUM;}
{op}		{ return yytext[0];}
"="     { return '=';}
"{"		{ return '{';}
"}"		{ return '}';}
"("		{ return '(';}
")"		{ return ')';}
";"		{ return ';';}
"<"     { return '<';}
"<="    { return LE;}
">"     { return '>';}
">="    { return GE;}
"=="    { return EQ;}
"!="    { return NE;}
.           		{ printf("Unknown : %s\n",yytext);}

%%

int yywrap()
{
	return 1;
}

/*function to be complete*/
int installID()
{
	return 0;
}

