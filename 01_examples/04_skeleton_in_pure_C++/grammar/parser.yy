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
    class driver;
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
    #include "driver.hh"
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

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%nterm <int> exp
%left "+" "-";
%left "*" "/";

%%
%start unit;
unit: assignments exp  { drv.m_iResult = $2; };

assignments:
    %empty                      {}
    | assignments assignment    {};

assignment:
    "identifier" ":=" exp { drv.m_mapVariables[$1] = $3; };

exp:
    "number"
    | "identifier"  { $$ = drv.m_mapVariables[$1]; }
    | exp "+" exp   { $$ = $1 + $3; }
    | exp "-" exp   { $$ = $1 - $3; }
    | exp "*" exp   { $$ = $1 * $3; }
    | exp "/" exp   { $$ = $1 / $3; }
    | "(" exp ")"   { $$ = $2; }
%%

void yy::parser::error (const location_type& l, const std::string& m) {
    std::cerr << l << ": " << m << '\n';
}
