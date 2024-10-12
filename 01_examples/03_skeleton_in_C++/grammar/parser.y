%code top {
    #include <string>
    #include <sstream>
    std::stringstream ssOutput;
}

%{
#define YYDEBUG 0  //Debugging: 1 (enable), 0 (disable)
#define YYMALLOC malloc
#define YYFREE free

#include "parser.hpp"
#include "lexer.cpp"
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
        TK_IDENTIFIER '=' TK_INTEGER    { ssOutput << "Integer assignemnt : " << $1 << " = " << $3 << std::endl; }
	|   TK_IDENTIFIER '=' TK_FLOAT      { ssOutput << "Float assignemnt : " << $1 << " = " << $3 << std::endl; }
	;
%%

bool ParseBuffer(const std::string& _strBuffer, std::string& _strOutput) {
#if YYDEBUG == 1
    yydebug = 1;
#endif

    ssOutput.str("");
    YY_BUFFER_STATE yy_buffer_state = yy_scan_string(_strBuffer.c_str());
    yyparse();
    yy_delete_buffer(yy_buffer_state);
    _strOutput = ssOutput.str();
    return true;
}

