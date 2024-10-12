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

%printer { yyo << $$; } <*>;

%define api.token.prefix {TOK_}
%token ASSIGN  ":=";
%token MINUS   "-";
%token PLUS    "+";
%token STAR    "*";
%token SLASH   "/";
%token LPAREN  "(";
%token RPAREN  ")";

%token <std::string> IDENTIFIER
%token <int> NUMBER
%nterm <int> exp
%left "+" "-";
%left "*" "/";

%%
%start unit;
unit: assignments exp  { cDriver.m_cApp.m_iResult = $2; };

assignments:
    %empty                      {}
    | assignments assignment    {};

assignment:
    IDENTIFIER ":=" exp { cDriver.m_cApp.m_mapVariables[$1] = $3; };

exp:
    NUMBER
    | IDENTIFIER  { $$ = cDriver.m_cApp.m_mapVariables[$1]; }
    | exp "+" exp   { $$ = $1 + $3; }
    | exp "-" exp   { $$ = $1 - $3; }
    | exp "*" exp   { $$ = $1 * $3; }
    | exp "/" exp   { $$ = $1 / $3; }
    | "(" exp ")"   { $$ = $2; }
%%

void yy::parser::error (const location_type& _cLocation, const std::string& _strError) {
    std::cerr << _cLocation << ": " << _strError << '\n';
}
