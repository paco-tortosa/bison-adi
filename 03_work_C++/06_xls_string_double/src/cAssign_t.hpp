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
    cAssign_t();

    std::string m_strCell;
    cExpr_t m_cExpr;   
    cExpr_t::encExprDataType_t m_encExprDataType;   //it Will be NUMBER or STRING
};

#endif
