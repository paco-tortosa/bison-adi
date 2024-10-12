#include "cExpr_t.hpp"

std::ostream& operator << (std::ostream& out, const cExpr_t& c)
{
    switch (c.m_encType)
    {
    case cExpr_t::encType_t::UNDEFINED:
        out << "UNDEFINED";
        break;
    case cExpr_t::encType_t::NUMBER:
        out << c.m_dValue;
        break;
    case cExpr_t::encType_t::CELL:
        out << c.m_strCell;
        break;
    case cExpr_t::encType_t::RANGE:
        out << c.m_strCell << ":" << c.m_strCell2;
        break;
    case cExpr_t::encType_t::FUNCTION:
    {
        out << c.m_strFunction << "(";
        for (int i = 0; i < c.m_vspcExpr.size(); i++) {
            if (i) {
                out << ",";
            }
            out << *c.m_vspcExpr[i];
        }
        out << ")";
        break;
    }
    case cExpr_t::encType_t::ARITH_ADD:
        out << *c.m_vspcExpr[0] << "+" << *c.m_vspcExpr[1];
        break;
    case cExpr_t::encType_t::ARITH_SUB:
        out << *c.m_vspcExpr[0] << "-" << *c.m_vspcExpr[1];
        break;
    case cExpr_t::encType_t::ARITH_MUL:
        out << *c.m_vspcExpr[0] << "*" << *c.m_vspcExpr[1];
        break;
    case cExpr_t::encType_t::ARITH_DIV:
        out << *c.m_vspcExpr[0] << "/" << *c.m_vspcExpr[1];
        break;
    case cExpr_t::encType_t::ARITH_UNARY_MINUS:
        out << "-" << *c.m_vspcExpr[0];
        break;
    case cExpr_t::encType_t::ARITH_IN_PAREN:
        out << "(" << *c.m_vspcExpr[0] << ")";
        break;
    }
    return out;
}

void cExpr_t::m_Print() {
    std::cout << *this;
}