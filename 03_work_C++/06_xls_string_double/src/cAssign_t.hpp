#ifndef cAssign_t_hh
#define cAssign_t_hh

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "cExpr_t.hpp"

class cAssign_t
{
public:
    cAssign_t()
    {
    }

    //Value
    std::string m_strCell;
    cExpr_t m_cExpr;   
};

#endif
