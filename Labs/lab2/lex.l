%{
enum yytokentype{
	INTNUM = 300, FLNUM,FENUM,ID, 
	INT, IF, ELSE,WHILE,DO,BREAK,REAL,TRUE,FALSE,RETURN,
	CHAR,BOOL,FLOAT,RELOP
};


int yylval;		//全局变量


#define LT 1
#define LE 2
#define EQ 3
#define NE 4
#define GT 5
#define GE 6


%}


delim		[ \t\n]
ws		{delim}+
letter	[A-Za-z]
digit		[0-9]
id		{letter}({letter}|{digit})*
number	-?{digit}+				
flnumber 	-?{digit}*\.{digit}+
fenumber	-?{digit}+(\.{digit}*)?([eE][+-]?{digit}+)?
op		[+\-*/=]
delimiter		[{}(),]
%option yylineno
%%


{ws}		{;}

\/\/([^\n])+	{	printf("Line %d line comments : %s\n", yylineno, yytext);}

\/\*([^\*])*\*([\*]|[^\*\/]([^\*])*[\*])*\/      { printf("Line %d\tComments:%s\n", yylineno, yytext); }
int		{	printf("INT\t\t%s\t\t\t%s\n", "keyword", yytext); return INT;}
char		{	printf("CHAR\t\t%s\t\t\t%s\n", "keyword", yytext); return CHAR;}
float		{	printf("FLOAT\t\t%s\t\t\t%s\n", "keyword", yytext); return FLOAT;}
bool		{	printf("BOOL\t\t%s\t\t\t%s\n", "keyword", yytext); return BOOL;}
if		{	printf("IF\t\t%s\t\t\t%s\n", "keyword", yytext); return IF;}
else		{	printf("ELSE\t\t%s\t\t\t%s\n", "keyword", yytext); return ELSE;}
while		{	printf("WHILE\t\t%s\t\t\t%s\n", "keyword", yytext); return WHILE;}
do		{	printf("DO\t\t%s\t\t\t%s\n", "keyword", yytext); return DO;}
break		{	printf("BREAK\t\t%s\t\t\t%s\n", "keyword", yytext); return BREAK;}
real		{	printf("REAL\t\t%s\t\t\t%s\n", "keyword", yytext); return REAL;}
false		{	printf("FALSE\t\t%s\t\t\t%s\n", "keyword", yytext); return FALSE;}
return	{	printf("RETURN\t\t%s\t\t\t%s\n", "keyword", yytext); return RETURN;}

{id}		{	printf("ID\t\t%s\t\t%s\n", "identifier", yytext); yylval = installID(); return ID;}
{number}	{	printf("CONST\t\t%s\t\t%s\n", "constant", yytext); return INTNUM;}
{flnumber}	{	printf("CONST\t\t%s\t\t%s\n", "constant", yytext); return FLNUM;}
{fenumber}	{	printf("CONST\t\t%s\t\t%s\n", "constant", yytext); return FENUM;}

{op}	{	printf("OP\t\t%s\t\t%s\n", "operator", yytext); }
	 
"<"		{	printf("RELOP\t\t%s\t%s\n", "relation operator", yytext); yylval = LT; return RELOP;}
"<="		{	printf("RELOP\t%s\t\t%s\n", "relation operator", yytext); yylval = LE; return RELOP;}
"=="		{	printf("RELOP\t%s\t\t%s\n", "relation operator", yytext); yylval = EQ; return RELOP;}
"!="		{	printf("RELOP\t%s\t\t%s\n", "relation operator", yytext); yylval = NE; return RELOP;}
">"		{	printf("RELOP\t%s\t\t%s\n", "relation operator", yytext); yylval = GT; return RELOP;}
">="		{	printf("RELOP\t%s\t\t%s\n", "relation operator", yytext); yylval = GE; return RELOP;}

	
{delimiter}	{	printf("DELIM\t\t%s\t\t%s\n", "delimiter", yytext); return yytext[0];}
"["		{	printf("DELIM\t\t%s\t\t%s\n", "delimiter", yytext); return yytext[0];}
"]"		{	printf("DELIM\t\t%s\t\t%s\n", "delimiter", yytext); return yytext[0];}

";"		{	printf("DELIM\t\t%s\t\t%s\n", "delimiter", yytext); return yytext[0]; }

.           {	printf("Error at line %d: Unknown character %s\n", yylineno, yytext); return yytext[0]; }

%%


int main(int argc, char **argv ) 
{
	int tok;
	
	++argv, --argc; /* skip over program name */
	if ( argc > 0 ) yyin = fopen( argv[0], "r" ); 
	else yyin = stdin; 

	
	while(tok = yylex()){
		;
		//printf("\t<%d, %d>\n", tok, yylval); 
	}

} 

int yywrap()
{
	return 1;
}

/*function to be complete*/
int installID()
{
	return 0;
}
