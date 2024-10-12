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
%token MINUS   "-";
%token PLUS    "+";
%token STAR    "*";
%token SLASH   "/";
%token LPAREN  "(";
%token RPAREN  ")";
%token ENDL    "\n";

%token <int> NUMBER
%nterm <int> exp line
%left "+" "-";
%left "*" "/";
%precedence NEG;

%%
%start input;
input:
    %empty          {}
    | input "\n"    {}
    | input line    { std::cout << " = " << $2 << std::endl;}
;

line:
    exp "\n"      { $$ = $1;}
;

exp:
    NUMBER                  
    | exp "+" exp           { $$ = $1 + $3; }
    | exp "-" exp           { $$ = $1 - $3; }
    | exp "*" exp           { $$ = $1 * $3; }
    | exp "/" exp           { $$ = $1 / $3; }
    | "-" exp %prec NEG     { $$ = -$2; }
    | "(" exp ")"           { $$ = $2; }
;
%%

void yy::parser::error (const location_type& _cLocation, const std::string& _strError) {
    std::cerr << _cLocation << ": " << _strError << '\n';
}
