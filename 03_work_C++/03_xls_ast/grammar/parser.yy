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
    //TOP TOP TOP
    #include "cDriver_t.hpp"
}

%code requires{
    //requires requires requires
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
%token ASSIGN   "=";

%token <double> NUMBER
%token <std::string> ID CELL
%nterm <std::shared_ptr<cExpr_t>> expr
%nterm assignment
%nterm <std::vector<std::shared_ptr<cExpr_t>>> args
%left "+" "-";
%left "*" "/";
%precedence NEG;

%%
%start input;
input:
    %empty                  {}
    | input "\n"            {}
    | input assignment "\n" {   }
;

assignment:
    CELL "=" expr           {
                                std::cout << $1 << " = ";
                                $3->m_Print();
                                std::cout << std::endl; 
                            }
;

expr:
    NUMBER                  { 
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encType = cExpr_t::encType_t::NUMBER;
                                $$->m_dValue = $1;
                            }
    | CELL                  { 
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encType = cExpr_t::encType_t::CELL; 
                                $$->m_strCell = $1;
                            }              
    | CELL ":" CELL         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encType = cExpr_t::encType_t::RANGE; 
                                $$->m_strCell = $1;
                                $$->m_strCell2 = $1;
                            }
    | ID "(" args ")"       {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encType = cExpr_t::encType_t::FUNCTION; 
                                $$->m_strFunction = $1;
                                $$->m_vspcExpr = $3;
                            }
    | expr "+" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encType = cExpr_t::encType_t::ARITH_ADD; 
                                $$->m_vspcExpr.push_back($1);
                                $$->m_vspcExpr.push_back($3);
                            }
    | expr "-" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encType = cExpr_t::encType_t::ARITH_SUB; 
                                $$->m_vspcExpr.push_back($1);
                                $$->m_vspcExpr.push_back($3);
                            }
    | expr "*" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encType = cExpr_t::encType_t::ARITH_MUL; 
                                $$->m_vspcExpr.push_back($1);
                                $$->m_vspcExpr.push_back($3);
                            }
    | expr "/" expr         {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encType = cExpr_t::encType_t::ARITH_DIV; 
                                $$->m_vspcExpr.push_back($1);
                                $$->m_vspcExpr.push_back($3);
                            }
    | "-" expr %prec NEG    {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encType = cExpr_t::encType_t::ARITH_UNARY_MINUS; 
                                $$->m_vspcExpr.push_back($2);
                            }
    | "(" expr ")"          {   
                                $$ = std::make_shared<cExpr_t>();
                                $$->m_encType = cExpr_t::encType_t::ARITH_IN_PAREN; 
                                $$->m_vspcExpr.push_back($2);
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
