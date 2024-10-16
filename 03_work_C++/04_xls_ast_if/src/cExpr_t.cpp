#include "cExpr_t.hpp"
#include <sstream>

std::map<std::string, std::string> cExpr_t::s_MapAlias;

std::string cExpr_t::m_GetXlsStyleCode()
{
    #define GENERATOR m_GetXlsStyleCode

    std::stringstream ss;
    switch (m_encType)
    {
    case cExpr_t::encType_t::UNDEFINED:
        ss << "UNDEFINED";
        break;
    case cExpr_t::encType_t::NUMBER:
        ss << m_dValue;
        break;
    case cExpr_t::encType_t::CELL:
        ss << m_strCell;
        break;
    case cExpr_t::encType_t::CELL_WITH_SHEET:
        ss << m_strSheet << "!" << m_strCell;
        break;
    case cExpr_t::encType_t::RANGE:
        ss << m_strCell << ":" << m_strCell2;
        break;
    case cExpr_t::encType_t::FUNCTION:
    {
        ss << m_strFunction << "(";
        for (int i = 0; i < m_vspcExpr.size(); i++) {
            if (i) {
                ss << ",";
            }
            ss << m_vspcExpr[i]->GENERATOR();
        }
        ss << ")";
        break;
    }
    case cExpr_t::encType_t::ARITH_ADD:
        ss << m_vspcExpr[0]->GENERATOR() << "+" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encType_t::ARITH_SUB:
        ss << m_vspcExpr[0]->GENERATOR() << "-" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encType_t::ARITH_MUL:
        ss << m_vspcExpr[0]->GENERATOR() << "*" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encType_t::ARITH_DIV:
        ss << m_vspcExpr[0]->GENERATOR() << "/" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encType_t::ARITH_UNARY_MINUS:
        ss << "-" << m_vspcExpr[0]->GENERATOR();
        break;
    case cExpr_t::encType_t::ARITH_IN_PAREN:
        ss << "(" << m_vspcExpr[0]->GENERATOR() << ")";
        break;
    case cExpr_t::encType_t::ID:
        ss << m_strString;
        break;
    case cExpr_t::encType_t::STRING:
        ss << "\"" << m_strString << "\"";
        break;
    case cExpr_t::encType_t::EQ:
        ss << m_vspcExpr[0]->GENERATOR() << "=" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encType_t::NEQ:
        ss << m_vspcExpr[0]->GENERATOR() << "<>" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encType_t::GT:
        ss << m_vspcExpr[0]->GENERATOR() << ">" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encType_t::GE:
        ss << m_vspcExpr[0]->GENERATOR() << ">=" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encType_t::LT:
        ss << m_vspcExpr[0]->GENERATOR() << "<" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encType_t::LE:
        ss << m_vspcExpr[0]->GENERATOR() << "<=" << m_vspcExpr[1]->GENERATOR();
        break;
    }
    return ss.str();

    #undef GENERATOR 
}


std::string cExpr_t::m_GetCStyleCode()
{
    #define GENERATOR m_GetCStyleCode
    
    std::stringstream ss;
    switch (m_encType)
    {
    case cExpr_t::encType_t::UNDEFINED:
        ss << "UNDEFINED";
        break;
    case cExpr_t::encType_t::NUMBER:
        ss << m_dValue;
        break;
    case cExpr_t::encType_t::ID:
        if (s_MapAlias.count(m_strString)) {
            ss << "mapCells[\"" << s_MapAlias[m_strString] << "\"]";
        }
        break;
    case cExpr_t::encType_t::CELL:
        ss << "mapCells[\"" << m_strCell << "\"]";
        break;
    case cExpr_t::encType_t::CELL_WITH_SHEET:
        ss << "mapCells[\"" << m_strSheet << "!" << m_strCell << "\"]";
        break;
    case cExpr_t::encType_t::RANGE:
        ss << m_strCell << ":" << m_strCell2;
        break;
    case cExpr_t::encType_t::FUNCTION:
    {
        if (m_strFunction == "OR") {
            ss << "(";
            for (int i = 0; i < m_vspcExpr.size(); i++) {
                if (i) {
                    ss << " || ";
                }
                ss << m_vspcExpr[i]->GENERATOR();
            }
            ss << ")";
        }
        else if (m_strFunction == "IF") {
            ss << "( " << m_vspcExpr[0]->GENERATOR() << " ? " << m_vspcExpr[1]->GENERATOR() << " : " << m_vspcExpr[2]->GENERATOR() << " )";
        }
        else {
            ss << m_strFunction << "(";
            for (int i = 0; i < m_vspcExpr.size(); i++) {
                if (i) {
                    ss << ",";
                }
                ss << m_vspcExpr[i]->GENERATOR();
            }
            ss << ")";
        }
        break;
    }
    case cExpr_t::encType_t::ARITH_ADD:
        ss << m_vspcExpr[0]->GENERATOR() << "+" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encType_t::ARITH_SUB:
        ss << m_vspcExpr[0]->GENERATOR() << "-" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encType_t::ARITH_MUL:
        ss << m_vspcExpr[0]->GENERATOR() << "*" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encType_t::ARITH_DIV:
        ss << m_vspcExpr[0]->GENERATOR() << "/" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encType_t::ARITH_UNARY_MINUS:
        ss << "-" << m_vspcExpr[0]->GENERATOR();
        break;
    case cExpr_t::encType_t::ARITH_IN_PAREN:
        ss << "(" << m_vspcExpr[0]->GENERATOR() << ")";
        break;
    case cExpr_t::encType_t::STRING:
        ss << "\"" << m_strString << "\"";
        break;
    case cExpr_t::encType_t::EQ:
        ss << "(" << m_vspcExpr[0]->GENERATOR() << "==" << m_vspcExpr[1]->GENERATOR() << ")";
        break;
    case cExpr_t::encType_t::NEQ:
        ss << "(" << m_vspcExpr[0]->GENERATOR() << "<>" << m_vspcExpr[1]->GENERATOR() << ")";
        break;
    case cExpr_t::encType_t::GT:
        ss << "(" << m_vspcExpr[0]->GENERATOR() << ">" << m_vspcExpr[1]->GENERATOR() << ")";
        break;
    case cExpr_t::encType_t::GE:
        ss << "(" << m_vspcExpr[0]->GENERATOR() << ">=" << m_vspcExpr[1]->GENERATOR() << ")";
        break;
    case cExpr_t::encType_t::LT:
        ss << "(" << m_vspcExpr[0]->GENERATOR() << "<" << m_vspcExpr[1]->GENERATOR() << ")";
        break;
    case cExpr_t::encType_t::LE:
        ss << "(" << m_vspcExpr[0]->GENERATOR() << "<=" << m_vspcExpr[1]->GENERATOR() << ")";
        break;
    }
    return ss.str();

    #undef GENERATOR 
}
