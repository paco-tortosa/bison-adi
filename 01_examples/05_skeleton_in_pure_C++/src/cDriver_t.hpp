#ifndef DRIVER_HH
#define DRIVER_HH

#include <string>
#include <map>
#include "parser.hh"
#include "cApp_t.hpp"

# define YY_DECL yy::parser::symbol_type yylex (cDriver_t& cDriver)
YY_DECL;

class cDriver_t
{
public:
    cDriver_t() 
        : m_bTraceParsing (false)
        , m_bTraceScanning (false)
    {}

    yy::location m_cLocation;                   //The token's location used by the scanner.
    bool m_bTraceParsing;                       //Whether to generate parser debug traces.
    bool m_bTraceScanning;                      //Whether to generate scanner debug traces.
    int m_Parse(const std::string& _strBuffer); //Run the parser on string.  Return 0 on success.

    //User app
    cApp_t m_cApp;
};
#endif // ! DRIVER_HH
