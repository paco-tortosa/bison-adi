%{
    #include "cDriver_t.hpp"
    #include "parser.hh"
%}

%{
    #define FLEX_VERSION (YY_FLEX_MAJOR_VERSION * 100 + YY_FLEX_MINOR_VERSION)
    //#define EKO ECHO
    #define EKO 
%}

%option noyywrap nounput noinput batch debug
%option never-interactive

%{
    yy::parser::symbol_type make_NUMBER (const std::string &_strText, const yy::parser::location_type& _loc);
    yy::parser::symbol_type make_STRING (const std::string &_strText, const yy::parser::location_type& _cLocation);
    #define YY_USER_ACTION  cDriver.m_cLocation.columns (yyleng); // Code run each time a pattern is matched.
%}

cell  [A-Z]+[0-9]+
id    [A-Za-z_]+[A-Za-z_0-9]*
float [0-9]*([\.][0-9]*)?
blank [ \t\r]
string (\"[^\"]*\")

%%

%{
    cDriver.m_cLocation.step();  // Code run each time yylex is called.
%}

{blank}+   cDriver.m_cLocation.step();
\n         {
            EKO;
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
"="        EKO; return yy::parser::make_EQ     (cDriver.m_cLocation);
"<>"       EKO; return yy::parser::make_NEQ    (cDriver.m_cLocation);
":"        EKO; return yy::parser::make_COLON  (cDriver.m_cLocation);
">"        EKO; return yy::parser::make_GT     (cDriver.m_cLocation);
">="       EKO; return yy::parser::make_GE     (cDriver.m_cLocation);
"<"        EKO; return yy::parser::make_LT     (cDriver.m_cLocation);
"<="       EKO; return yy::parser::make_LE     (cDriver.m_cLocation);
"!"        EKO; return yy::parser::make_EXCL   (cDriver.m_cLocation);
"alias"    EKO; return yy::parser::make_ALIAS  (cDriver.m_cLocation);
"sheet"    EKO; return yy::parser::make_SHEET  (cDriver.m_cLocation);

{cell}     EKO; return yy::parser::make_CELL   (yytext, cDriver.m_cLocation);  //{cell} before {id}
{id}       EKO; return yy::parser::make_ID     (yytext, cDriver.m_cLocation);
{float}    EKO; return make_NUMBER             (yytext, cDriver.m_cLocation);
{string}   EKO; return make_STRING             (yytext, cDriver.m_cLocation);
.          {
             throw yy::parser::syntax_error (cDriver.m_cLocation, "invalid character: " + std::string(yytext));
           }
<<EOF>>    return yy::parser::make_YYEOF (cDriver.m_cLocation);
%%

yy::parser::symbol_type make_STRING (const std::string &_strText, const yy::parser::location_type& _cLocation) {
  std::string strContent = _strText.substr(1, _strText.size()-2);
  return yy::parser::make_STRING (strContent, _cLocation);
}

yy::parser::symbol_type make_NUMBER (const std::string &_strText, const yy::parser::location_type& _cLocation) {
  double n = std::stod(_strText.c_str());
  return yy::parser::make_NUMBER (n, _cLocation);
}
