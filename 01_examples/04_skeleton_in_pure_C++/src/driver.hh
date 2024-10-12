#ifndef DRIVER_HH
#define DRIVER_HH

#include <string>
#include <map>
#include "parser.hh"

// Give Flex the prototype of yylex we want ...
# define YY_DECL yy::parser::symbol_type yylex (driver& drv)
YY_DECL;

class driver
{
public:
    driver() 
        : m_bTraceParsing (false)
        , m_bTraceScanning (false)
        , m_iResult (0)
    {
        m_mapVariables["one"] = 1;
        m_mapVariables["two"] = 2;
    }

    std::map<std::string, int> m_mapVariables;

    yy::location m_cLocation;           //The token's location used by the scanner.
    bool m_bTraceParsing;               //Whether to generate parser debug traces.
    bool m_bTraceScanning;              //Whether to generate scanner debug traces.
    int m_iResult;                      

    int m_Parse(const std::string& f);  //Run the parser on file F.  Return 0 on success.
};
#endif // ! DRIVER_HH
