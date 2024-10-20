%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.2"
%header
%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #pragma warning(disable : 4065)
    #include <stdint.h>
    #include <cerrno>
    #include <climits>
    #include <cstdlib>
    #include <cstring>
    #include <string>
    class cDriver_t;
}

// The parsing context.
%param { cDriver_t& cDriver }

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code top{
    #include "cDriver_t.hpp"
}

%code requires{
    #include "cDriver_t.hpp"
}

//%printer { yyo << $$; } <*>;

%define api.token.prefix {TOK_}
%token MINUS    "-";
%token PLUS     "+";
%token STAR     "*";
%token SLASH    "/";
%token LPAREN   "(";
%token RPAREN   ")";
%token COLON    ":";
%token ENDL     "\n";
%token COMMA    ",";
%token GT       ">";
%token GE       ">=";
%token LT       "<";
%token LE       "<=";
%token EQ       "=";
%token NEQ      "<>";
%token EXCL     "!";
%token ALIAS    "alias";
%token SHEET    "sheet";

%token <double> NUMBER
%token <std::string> ID CELL STRING
%nterm <std::shared_ptr<cExpr_t>> expr
%nterm assignment
%nterm <std::vector<std::shared_ptr<cExpr_t>>> args

%nonassoc "=" "<=" "<" ">=" ">" "<>"  
%left "+" "-";
%left "*" "/";
%precedence NEG;

%%
%start input;
input:
    %empty                  {}
    | input "\n"            {}
    | input assignment "\n" {}
    | SHEET "=" STRING "\n" {
                                cDriver.m_strSheetInParsing = $3;
                            }
    | input ID "alias" ID "!" CELL "\n" {
                                cExpr_t::s_MapAlias[$2] = $4 + "!" + $6;
                            }
;

assignment:
    ID "!" CELL "=" expr    {
                                std::string strSheetAndCell = $1 + "!" + $3;
                                cDriver.m_cApp.m_mapcCells[strSheetAndCell] = *$5;
                            }
;

expr:
    NUMBER                  { 
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::NUMBER;
                                $$->m_SetExprDataType(cExpr_t::encExprDataType_t::NUMBER);
                                $$->m_dValue = $1;
                            }
    | ID                    {    
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::ID; 
                                $$->m_strString = $1;
                            }              
    | STRING                { 
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::STRING; 
                                $$->m_SetExprDataType(cExpr_t::encExprDataType_t::STRING);
                                $$->m_strString = $1;
                            }              
    | ID "!" CELL           { 
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::CELL_WITH_SHEET; 
                                $$->m_strSheet = $1;
                                $$->m_strCell = $3;
                                $$->m_vstrDependOnCells.push_back($3);
                            }              
    | CELL                  { 
                                $$ = std::make_shared<cExpr_t>();
                                // $$->m_encExprType = cExpr_t::encExprType_t::CELL; 
                                $$->m_encExprType = cExpr_t::encExprType_t::CELL_WITH_SHEET; 
                                $$->m_strSheet = cDriver.m_strSheetInParsing;
                                $$->m_strCell = $1;
                                $$->m_vstrDependOnCells.push_back($1);
                            }              
    | ID "!" CELL ":" CELL  { 
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::RANGE_WITH_SHEET; 
                                $$->m_strSheet = $1;
                                $$->m_strCell = $3;
                                $$->m_strCell2 = $5;
                                $$->m_vstrDependOnCells.push_back($3);  //TODO
                                $$->m_vstrDependOnCells.push_back($5);  //TODO
                            }              
    | CELL ":" CELL         {   
                                $$ = std::make_shared<cExpr_t>();
                                // $$->m_encExprType = cExpr_t::encExprType_t::RANGE; 
                                $$->m_encExprType = cExpr_t::encExprType_t::RANGE_WITH_SHEET; 
                                $$->m_strSheet = cDriver.m_strSheetInParsing;
                                $$->m_strCell = $1;
                                $$->m_strCell2 = $3;
                                $$->m_vstrDependOnCells.push_back($1);  //TODO
                                $$->m_vstrDependOnCells.push_back($3);  //TODO  
                            }
    | ID "(" args ")"       {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::FUNCTION; 
                                auto* t = cExpr_t::s_GetExprDataTypeOfFunction($1, $3);
                                $$->m_SetExprReferenceForDataType(t);
                                $$->m_strFunction = $1;
                                $$->m_vspcExpr = $3;
                                for(auto& e:$3){
                                    $$->m_AddDependency(*e);
                                }
                            }
    | expr "+" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::ARITH_ADD; 
                                $$->m_SetExprReferenceForDataType($1.get());
                                $$->m_vspcExpr.push_back($1);
                                $$->m_AddDependency(*$1);
                                $$->m_vspcExpr.push_back($3);
                                $$->m_AddDependency(*$3);
                            }
    | expr "-" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::ARITH_SUB; 
                                $$->m_SetExprReferenceForDataType($1.get());
                                $$->m_vspcExpr.push_back($1);
                                $$->m_AddDependency(*$1);
                                $$->m_vspcExpr.push_back($3);
                                $$->m_AddDependency(*$3);
                            }
    | expr "*" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::ARITH_MUL; 
                                $$->m_SetExprReferenceForDataType($1.get());
                                $$->m_vspcExpr.push_back($1);
                                $$->m_AddDependency(*$1);
                                $$->m_vspcExpr.push_back($3);
                                $$->m_AddDependency(*$3);
                            }
    | expr "/" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::ARITH_DIV; 
                                $$->m_SetExprReferenceForDataType($1.get());
                                $$->m_vspcExpr.push_back($1);
                                $$->m_AddDependency(*$1);
                                $$->m_vspcExpr.push_back($3);
                                $$->m_AddDependency(*$3);
                            }
    | "-" expr %prec NEG    {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::ARITH_UNARY_MINUS; 
                                $$->m_SetExprReferenceForDataType($2.get());
                                $$->m_vspcExpr.push_back($2);
                                $$->m_AddDependency(*$2);
                            }
    | "(" expr ")"          {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::ARITH_IN_PAREN; 
                                $$->m_SetExprReferenceForDataType($2.get());
                                $$->m_vspcExpr.push_back($2);
                                $$->m_AddDependency(*$2);
                            }
    | expr "=" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::EQ; 
                                $$->m_SetExprReferenceForDataType($1.get());
                                $$->m_vspcExpr.push_back($1);
                                $$->m_AddDependency(*$1);
                                $$->m_vspcExpr.push_back($3);
                                $$->m_AddDependency(*$3);
                            }
    | expr "<>" expr        {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::NEQ; 
                                $$->m_SetExprReferenceForDataType($1.get());
                                $$->m_vspcExpr.push_back($1);
                                $$->m_AddDependency(*$1);
                                $$->m_vspcExpr.push_back($3);
                                $$->m_AddDependency(*$3);
                            }
    | expr ">" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::GT; 
                                $$->m_SetExprReferenceForDataType($1.get());
                                $$->m_vspcExpr.push_back($1);
                                $$->m_AddDependency(*$1);
                                $$->m_vspcExpr.push_back($3);
                                $$->m_AddDependency(*$3);
                            }
    | expr ">=" expr        {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::GE; 
                                $$->m_SetExprReferenceForDataType($1.get());
                                $$->m_vspcExpr.push_back($1);
                                $$->m_AddDependency(*$1);
                                $$->m_vspcExpr.push_back($3);
                                $$->m_AddDependency(*$3);
                            }
    | expr "<" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::LT; 
                                $$->m_SetExprReferenceForDataType($1.get());
                                $$->m_vspcExpr.push_back($1);
                                $$->m_AddDependency(*$1);
                                $$->m_vspcExpr.push_back($3);
                                $$->m_AddDependency(*$3);
                            }
    | expr "<=" expr        {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::LE; 
                                $$->m_SetExprReferenceForDataType($1.get());
                                $$->m_vspcExpr.push_back($1);
                                $$->m_AddDependency(*$1);
                                $$->m_vspcExpr.push_back($3);
                                $$->m_AddDependency(*$3);
                            }
;

args:
    expr                    {   
                                $$.push_back($1);
                            }
    | args "," expr         {   
                                $1.push_back($3);
                                std::swap($$,$1);
                            }
;
%%

void yy::parser::error (const location_type& _cLocation, const std::string& _strError) {
    std::cerr << _cLocation << ": " << _strError << '\n';
}
