%{
    #include "parser.hh"
    #include "driver.hh"
%}

%{
    #define FLEX_VERSION (YY_FLEX_MAJOR_VERSION * 100 + YY_FLEX_MINOR_VERSION)
%}

%option noyywrap nounput noinput batch debug
%option never-interactive

%{
    // A number symbol corresponding to the value in S.
    yy::parser::symbol_type make_NUMBER (const std::string &s, const yy::parser::location_type& loc);
    #define YY_USER_ACTION  drv.m_cLocation.columns (yyleng); // Code run each time a pattern is matched.
%}

id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
blank [ \t\r]

%%

%{
    drv.m_cLocation.step ();  // Code run each time yylex is called.
%}

{blank}+   drv.m_cLocation.step();
\n+        drv.m_cLocation.lines(yyleng); drv.m_cLocation.step();

"-"        return yy::parser::make_MINUS  (drv.m_cLocation);
"+"        return yy::parser::make_PLUS   (drv.m_cLocation);
"*"        return yy::parser::make_STAR   (drv.m_cLocation);
"/"        return yy::parser::make_SLASH  (drv.m_cLocation);
"("        return yy::parser::make_LPAREN (drv.m_cLocation);
")"        return yy::parser::make_RPAREN (drv.m_cLocation);
":="       return yy::parser::make_ASSIGN (drv.m_cLocation);

{int}      return make_NUMBER (yytext, drv.m_cLocation);
{id}       return yy::parser::make_IDENTIFIER (yytext, drv.m_cLocation);
.          {
             throw yy::parser::syntax_error
               (drv.m_cLocation, "invalid character: " + std::string(yytext));
           }
<<EOF>>    return yy::parser::make_YYEOF (drv.m_cLocation);
%%

yy::parser::symbol_type make_NUMBER (const std::string &s, const yy::parser::location_type& _cLocation) {
  errno = 0;
  long n = strtol (s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error (_cLocation, "integer is out of range: " + s);
  return yy::parser::make_NUMBER ((int) n, _cLocation);
}
