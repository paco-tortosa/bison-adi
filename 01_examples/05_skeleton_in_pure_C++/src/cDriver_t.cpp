#include "cDriver_t.hpp"
#include "parser.hh"

extern int yy_flex_debug;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE yy_scan_string ( const char *yy_str  );

int cDriver_t::m_Parse(const std::string& _strBuffer) {
    std::string strBuffer("Buffer");
    m_cLocation.initialize(&strBuffer);

    //Scan begin
    yy_flex_debug = m_bTraceScanning;
    yy_scan_string(_strBuffer.c_str());

    //Parse
    yy::parser parse (*this);
    parse.set_debug_level(m_bTraceParsing);
    int res = parse.parse();

    return res;
}
