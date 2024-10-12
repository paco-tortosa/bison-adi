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

%code {
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
%nterm <double> expr
%nterm assignment
%nterm <std::vector<double>> args
%left "+" "-";
%left "*" "/";
%precedence NEG;

%%
%start input;
input:
    %empty                  {}
    | input "\n"            {}
    | input expr "\n"       {}
    | input assignment "\n" {}
;

assignment:
    CELL "=" expr           { cDriver.m_cApp.m_mapCells[$1] = $3; }
;

args:
    expr                    { cDriver.m_cApp.m_args.push_back($1);}
    | args "," expr         { cDriver.m_cApp.m_args.push_back($3);}
;

expr:
    NUMBER                  { $$ = $1; }
    | CELL                  { $$ = cDriver.m_cApp.m_mapCells[$1]; }               
    | CELL ":" CELL         { std::cout << "range : " << $1 << ":" << $3 << std::endl; $$ = 0;}
    | ID "(" args ")"       { 
                                for(auto& a:cDriver.m_cApp.m_args) $$ += a; 
                                cDriver.m_cApp.m_args.clear();
                            }
    | expr "+" expr         { $$ = $1 + $3; }
    | expr "-" expr         { $$ = $1 - $3; }
    | expr "*" expr         { $$ = $1 * $3; }
    | expr "/" expr         { $$ = $1 / $3; }
    | "-" expr %prec NEG    { $$ = -$2; }
    | "(" expr ")"          { $$ = $2; }
;
%%

void yy::parser::error (const location_type& _cLocation, const std::string& _strError) {
    std::cerr << _cLocation << ": " << _strError << '\n';
}
