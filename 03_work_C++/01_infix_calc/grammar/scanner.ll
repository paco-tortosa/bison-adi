%{
    #include "parser.hh"
    #include "cDriver_t.hpp"
%}

%{
    #define FLEX_VERSION (YY_FLEX_MAJOR_VERSION * 100 + YY_FLEX_MINOR_VERSION)
%}

%option noyywrap nounput noinput batch debug
%option never-interactive

%{
    // A number symbol corresponding to the value in S.
    yy::parser::symbol_type make_NUMBER (const std::string &_strText, const yy::parser::location_type& _loc);
    #define YY_USER_ACTION  cDriver.m_cLocation.columns (yyleng); // Code run each time a pattern is matched.
%}

id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
blank [ \t\r]

%%

%{
    cDriver.m_cLocation.step();  // Code run each time yylex is called.
%}

{blank}+   cDriver.m_cLocation.step();
\n         {
            cDriver.m_cLocation.lines(yyleng); 
            cDriver.m_cLocation.step(); 
            return yy::parser::make_ENDL  (cDriver.m_cLocation);
           }

"-"        ECHO; return yy::parser::make_MINUS  (cDriver.m_cLocation);
"+"        ECHO; return yy::parser::make_PLUS   (cDriver.m_cLocation);
"*"        ECHO; return yy::parser::make_STAR   (cDriver.m_cLocation);
"/"        ECHO; return yy::parser::make_SLASH  (cDriver.m_cLocation);
"("        ECHO; return yy::parser::make_LPAREN (cDriver.m_cLocation);
")"        ECHO; return yy::parser::make_RPAREN (cDriver.m_cLocation);

{int}      ECHO; return make_NUMBER (yytext, cDriver.m_cLocation);
.          {
             throw yy::parser::syntax_error (cDriver.m_cLocation, "invalid character: " + std::string(yytext));
           }
<<EOF>>    return yy::parser::make_YYEOF (cDriver.m_cLocation);
%%

yy::parser::symbol_type make_NUMBER (const std::string &_strText, const yy::parser::location_type& _cLocation) {
  errno = 0;
  long n = strtol (_strText.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error (_cLocation, "integer is out of range: " + _strText);
  return yy::parser::make_NUMBER ((int) n, _cLocation);
}
