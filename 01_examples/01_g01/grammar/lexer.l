%{   
//#pragma warning(disable: 4996)
#include <stdio.h>
#include <stdlib.h>
#define YY_DECL int yylex()
#define fileno _fileno //to prevent a compile error

#include "parser.h"
%}

%option noyywrap
%option never-interactive

DIGIT    [0-9]
   
%%

"-"            {printf("-"); return MINUS;}
"+"            {printf("+"); return PLUS;}
"*"            {printf("*"); return MULT;}
"/"            {printf("/"); return DIV;}
"="            {printf("="); return EQUAL;}
"("            {printf("("); return L_PAREN;}
")"            {printf(")"); return R_PAREN;}

(\.{DIGIT}+)|({DIGIT}+(\.{DIGIT}*)?([eE][+-]?[0-9]+)?)   {yylval.dval = atof(yytext); return NUMBER;}

[ \t]+         {/* ignore spaces */}

"\n"           {return END;}

.              {printf("Error at line %d: unrecognized symbol \"%s\"\n", yylloc.first_line, yytext); exit(0);}

%%
