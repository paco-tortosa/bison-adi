%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define fileno _fileno //to prevent a compile error
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;
%}

%option noyywrap
%option yylineno
%option never-interactive

DIGIT   [0-9]
INTEGER {DIGIT}+
FLOAT   {DIGIT}+\.{DIGIT}*
ID      [A-Za-z_][0-9A-Za-z_]+

%%

"quit"      {return EOF;}
{INTEGER}   {yylval.iVal = atoi(yytext); return TK_INTEGER;} 
{FLOAT}     {yylval.dVal = atof(yytext); return TK_FLOAT;} 
{ID}        {strcpy_s(yylval.strVal, sizeof(yylval.strVal), yytext); return TK_IDENTIFIER;} 
[=]         {return *yytext;} 
[ \t]       //Ignore blanks

%%
