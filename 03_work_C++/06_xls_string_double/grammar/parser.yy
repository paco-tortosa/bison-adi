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
    #include <cstring> // strerror
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
    | input ID "alias" ID "!" CELL "\n" 
                            {
                                cExpr_t::s_MapAlias[$2] = $4 + "!" + $6;
                            }
;

assignment:
    CELL "=" expr           {
                                cDriver.m_cApp.m_vcAssign.push_back(cAssign_t());
                                cDriver.m_cApp.m_vcAssign.back().m_strCell = $1;
                                cDriver.m_cApp.m_vcAssign.back().m_cExpr = *$3;
                            }
;

expr:
    NUMBER                  { 
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::NUMBER;
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::NUMBER;
                                $$->m_dValue = $1;
                            }
    | ID                    {    
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::ID; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::UNDEFINED;
                                $$->m_strString = $1;
                            }              
    | STRING                { 
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::STRING; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::STRING;
                                $$->m_strString = $1.substr(1,$1.length()-2);
                            }              
    | ID "!" CELL           { 
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::CELL_WITH_SHEET; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::UNDEFINED;
                                $$->m_strSheet = $1;
                                $$->m_strCell = $3;
                            }              
    | CELL                  { 
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::CELL; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::UNDEFINED;
                                $$->m_strCell = $1;
                            }              
    | CELL ":" CELL         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::RANGE; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::UNDEFINED;
                                $$->m_strCell = $1;
                                $$->m_strCell2 = $1;
                            }
    | ID "(" args ")"       {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::FUNCTION; 
                                $$->m_encExprDataType = cExpr_t::s_GetExprDataTypeOfFunction($1, $3);
                                $$->m_strFunction = $1;
                                $$->m_vspcExpr = $3;
                            }
    | expr "+" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::ARITH_ADD; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::NUMBER;
                                $$->m_vspcExpr.push_back($1);
                                $$->m_vspcExpr.push_back($3);
                            }
    | expr "-" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::ARITH_SUB; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::NUMBER;
                                $$->m_vspcExpr.push_back($1);
                                $$->m_vspcExpr.push_back($3);
                            }
    | expr "*" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::ARITH_MUL; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::NUMBER;
                                $$->m_vspcExpr.push_back($1);
                                $$->m_vspcExpr.push_back($3);
                            }
    | expr "/" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::ARITH_DIV; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::NUMBER;
                                $$->m_vspcExpr.push_back($1);
                                $$->m_vspcExpr.push_back($3);
                            }
    | "-" expr %prec NEG    {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::ARITH_UNARY_MINUS; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::NUMBER;
                                $$->m_vspcExpr.push_back($2);
                            }
    | "(" expr ")"          {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::ARITH_IN_PAREN; 
                                $$->m_encExprDataType = $2->m_encExprDataType;
                                $$->m_vspcExpr.push_back($2);
                            }
    | expr "=" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::EQ; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::UNDEFINED;
                                $$->m_vspcExpr.push_back($1);
                                $$->m_vspcExpr.push_back($3);
                            }
    | expr "<>" expr        {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::NEQ; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::UNDEFINED;
                                $$->m_vspcExpr.push_back($1);
                                $$->m_vspcExpr.push_back($3);
                            }
    | expr ">" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::GT; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::UNDEFINED;
                                $$->m_vspcExpr.push_back($1);
                                $$->m_vspcExpr.push_back($3);
                            }
    | expr ">=" expr        {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::GE; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::UNDEFINED;
                                $$->m_vspcExpr.push_back($1);
                                $$->m_vspcExpr.push_back($3);
                            }
    | expr "<" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::LT; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::UNDEFINED;
                                $$->m_vspcExpr.push_back($1);
                                $$->m_vspcExpr.push_back($3);
                            }
    | expr "<=" expr        {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encExprType = cExpr_t::encExprType_t::LE; 
                                $$->m_encExprDataType = cExpr_t::encExprDataType_t::UNDEFINED;
                                $$->m_vspcExpr.push_back($1);
                                $$->m_vspcExpr.push_back($3);
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
