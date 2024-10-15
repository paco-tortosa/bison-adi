#ifndef cApp_t_hh
#define cApp_t_hh

#include <string>
#include <vector>
#include <map>
#include "cExpr_t.hpp"
#include "cAssign_t.hpp"

class cApp_t
{
public:
    cApp_t() 
    {
    }

    std::map<std::string, cAssign_t> m_mapcAssign;
    //std::vector<cAssign_t> m_vcAssign;
};
#endif
