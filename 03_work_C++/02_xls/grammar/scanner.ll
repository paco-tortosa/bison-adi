%{
    #include "parser.hh"
    #include "cDriver_t.hpp"
%}

%{
    #define FLEX_VERSION (YY_FLEX_MAJOR_VERSION * 100 + YY_FLEX_MINOR_VERSION)
    // #define EKO ECHO
    #define EKO 
%}

%option noyywrap nounput noinput batch debug
%option never-interactive

%{
    // A number symbol corresponding to the value in S.
    yy::parser::symbol_type make_NUMBER (const std::string &_strText, const yy::parser::location_type& _loc);
    #define YY_USER_ACTION  cDriver.m_cLocation.columns (yyleng); // Code run each time a pattern is matched.
%}

cell  [A-Z]+[0-9]+
id    [A-Za-z_]+[A-Za-z_0-9]*
float [0-9]*([\.][0-9]*)?
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

"-"        EKO; return yy::parser::make_MINUS  (cDriver.m_cLocation);
"+"        EKO; return yy::parser::make_PLUS   (cDriver.m_cLocation);
"*"        EKO; return yy::parser::make_STAR   (cDriver.m_cLocation);
"/"        EKO; return yy::parser::make_SLASH  (cDriver.m_cLocation);
"("        EKO; return yy::parser::make_LPAREN (cDriver.m_cLocation);
")"        EKO; return yy::parser::make_RPAREN (cDriver.m_cLocation);
","        EKO; return yy::parser::make_COMMA  (cDriver.m_cLocation);
"="        EKO; return yy::parser::make_ASSIGN (cDriver.m_cLocation);
":"        EKO; return yy::parser::make_COLON  (cDriver.m_cLocation);

{cell}     EKO; return yy::parser::make_CELL   (yytext, cDriver.m_cLocation);  //{cell} before {id}
{id}       EKO; return yy::parser::make_ID     (yytext, cDriver.m_cLocation);
{float}    EKO; return make_NUMBER             (yytext, cDriver.m_cLocation);
.          {
             throw yy::parser::syntax_error (cDriver.m_cLocation, "invalid character: " + std::string(yytext));
           }
<<EOF>>    return yy::parser::make_YYEOF (cDriver.m_cLocation);
%%

yy::parser::symbol_type make_NUMBER (const std::string &_strText, const yy::parser::location_type& _cLocation) {
  double n = std::stod(_strText.c_str());
  return yy::parser::make_NUMBER (n, _cLocation);
}
