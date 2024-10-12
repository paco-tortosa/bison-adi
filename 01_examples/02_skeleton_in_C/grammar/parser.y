%{
#define YYDEBUG 0  //Debugging: 1 (enable), 0 (disable)
#define YYMALLOC malloc
#define YYFREE free
#include "parser.h"
#include "lexer.c"

void yyerror(const char *msg) {
   printf("** Line %d: %s\n", yylloc.first_line, msg);
}

%}

%locations
%union{
  int iVal;
  double dVal;
  char strVal[20];
}

%start input
%token <iVal>   TK_INTEGER
%token <dVal>   TK_FLOAT 
%token <strVal> TK_IDENTIFIER 
%token <iVal>   TK_EQUAL 

%% 
input:
    | input assignment
	;

assignment:    
        TK_IDENTIFIER '=' TK_INTEGER    { printf("Integer assignemnt : %s = %d", $1, $3); }
	|   TK_IDENTIFIER '=' TK_FLOAT      { printf("Float assignemnt   : %s = %f", $1, $3); }
	;
%%

int main(int argc, char **argv) {
#if YYDEBUG == 1
    yydebug = 1;
#endif

    char strInput[] = "var = 34\nvar1 = 3.14\nquit";
    YY_BUFFER_STATE yy_buffer_state = yy_scan_string(strInput);
    yyparse();
    yy_delete_buffer(yy_buffer_state);
    return 0;
}

