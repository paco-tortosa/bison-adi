#include "cExpr_t.hpp"
#include <sstream>

std::map<std::string, std::string> cExpr_t::s_MapAlias;

std::string cExpr_t::m_GetXlsStyleCode()
{
    #define GENERATOR m_GetXlsStyleCode

    std::stringstream ss;
    switch (m_encExprType)
    {
    case cExpr_t::encExprType_t::UNDEFINED:
        ss << "UNDEFINED";
        break;
    case cExpr_t::encExprType_t::NUMBER:
        ss << m_dValue;
        break;
    case cExpr_t::encExprType_t::CELL:
        ss << m_strCell;
        break;
    case cExpr_t::encExprType_t::CELL_WITH_SHEET:
        ss << m_strSheet << "!" << m_strCell;
        break;
    case cExpr_t::encExprType_t::RANGE:
        ss << m_strCell << ":" << m_strCell2;
        break;
    case cExpr_t::encExprType_t::RANGE_WITH_SHEET:
        ss << m_strSheet << "!" << m_strCell << ":" << m_strCell2;
        break;
    case cExpr_t::encExprType_t::FUNCTION:
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
    case cExpr_t::encExprType_t::ARITH_ADD:
        ss << m_vspcExpr[0]->GENERATOR() << "+" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encExprType_t::ARITH_SUB:
        ss << m_vspcExpr[0]->GENERATOR() << "-" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encExprType_t::ARITH_MUL:
        ss << m_vspcExpr[0]->GENERATOR() << "*" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encExprType_t::ARITH_DIV:
        ss << m_vspcExpr[0]->GENERATOR() << "/" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encExprType_t::ARITH_UNARY_MINUS:
        ss << "-" << m_vspcExpr[0]->GENERATOR();
        break;
    case cExpr_t::encExprType_t::ARITH_IN_PAREN:
        ss << "(" << m_vspcExpr[0]->GENERATOR() << ")";
        break;
    case cExpr_t::encExprType_t::ID:
        ss << m_strString;
        break;
    case cExpr_t::encExprType_t::STRING:
        ss << "\"" << m_strString << "\"";
        break;
    case cExpr_t::encExprType_t::EQ:
        ss << m_vspcExpr[0]->GENERATOR() << "=" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encExprType_t::NEQ:
        ss << m_vspcExpr[0]->GENERATOR() << "<>" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encExprType_t::GT:
        ss << m_vspcExpr[0]->GENERATOR() << ">" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encExprType_t::GE:
        ss << m_vspcExpr[0]->GENERATOR() << ">=" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encExprType_t::LT:
        ss << m_vspcExpr[0]->GENERATOR() << "<" << m_vspcExpr[1]->GENERATOR();
        break;
    case cExpr_t::encExprType_t::LE:
        ss << m_vspcExpr[0]->GENERATOR() << "<=" << m_vspcExpr[1]->GENERATOR();
        break;
    }
    return ss.str();

    #undef GENERATOR 
}


std::string cExpr_t::m_GetCStyleCode(std::map<std::string, cExpr_t>& _mapcCells)
{
    #define GENERATOR m_GetCStyleCode
    
    std::stringstream ss;
    switch (m_encExprType)
    {
    case cExpr_t::encExprType_t::UNDEFINED:
        ss << "UNDEFINED";
        break;
    case cExpr_t::encExprType_t::NUMBER:
        ss << m_dValue;
        break;
    case cExpr_t::encExprType_t::ID:
        if (s_MapAlias.count(m_strString)) {
            ss << "mapCells[\"" << s_MapAlias[m_strString] << "\"]";
        }
        break;
    case cExpr_t::encExprType_t::CELL:
        if(_mapcCells.count(m_strCell)){
            if(_mapcCells[m_strCell].m_encExprDataType == encExprDataType_t::NUMBER)
                ss << "mapCells[\"" << m_strCell << "\"].num()";
            else if(_mapcCells[m_strCell].m_encExprDataType == encExprDataType_t::STRING)
                ss << "mapCells[\"" << m_strCell << "\"].str()";
        }
        else {
            ss << "mapCells[\"" << m_strCell << "\"].undef()";
        }
        break;
    case cExpr_t::encExprType_t::CELL_WITH_SHEET:
    {
        std::string strCellWithSheet = m_strSheet + "!" + m_strCell;
        if(_mapcCells.count(strCellWithSheet)){
            if(_mapcCells[strCellWithSheet].m_encExprDataType == encExprDataType_t::NUMBER)
                ss << "mapCells[\"" << strCellWithSheet << "\"].num()";
            else if(_mapcCells[strCellWithSheet].m_encExprDataType == encExprDataType_t::STRING)
                ss << "mapCells[\"" << strCellWithSheet << "\"].str()";
        }
        else {
            ss << "mapCells[\"" << strCellWithSheet << "\"].undef()";
        }
        //ss << "mapCells[\"" << strCellWithSheet << "\"].to_define()";
        break;
    }
    case cExpr_t::encExprType_t::RANGE:
        ss << m_strCell << ":" << m_strCell2;
        break;
    case cExpr_t::encExprType_t::RANGE_WITH_SHEET:
        ss << m_strSheet << "!" << m_strCell << ":" << m_strCell2;
        break;
    case cExpr_t::encExprType_t::FUNCTION:
    {
        if (m_strFunction == "OR") {
            ss << "(";
            for (int i = 0; i < m_vspcExpr.size(); i++) {
                if (i) {
                    ss << " || ";
                }
                ss << m_vspcExpr[i]->GENERATOR(_mapcCells);
            }
            ss << ")";
        }
        else if (m_strFunction == "IF") {
            ss << "( " << m_vspcExpr[0]->GENERATOR(_mapcCells) << " ? " << m_vspcExpr[1]->GENERATOR(_mapcCells) << " : " << m_vspcExpr[2]->GENERATOR(_mapcCells) << " )";
        }
        else {
            ss << m_strFunction << "(";
            for (int i = 0; i < m_vspcExpr.size(); i++) {
                if (i) {
                    ss << ",";
                }
                ss << m_vspcExpr[i]->GENERATOR(_mapcCells);
            }
            ss << ")";
        }
        break;
    }
    case cExpr_t::encExprType_t::ARITH_ADD:
        ss << m_vspcExpr[0]->GENERATOR(_mapcCells) << "+" << m_vspcExpr[1]->GENERATOR(_mapcCells);
        break;
    case cExpr_t::encExprType_t::ARITH_SUB:
        ss << m_vspcExpr[0]->GENERATOR(_mapcCells) << "-" << m_vspcExpr[1]->GENERATOR(_mapcCells);
        break;
    case cExpr_t::encExprType_t::ARITH_MUL:
        ss << m_vspcExpr[0]->GENERATOR(_mapcCells) << "*" << m_vspcExpr[1]->GENERATOR(_mapcCells);
        break;
    case cExpr_t::encExprType_t::ARITH_DIV:
        ss << m_vspcExpr[0]->GENERATOR(_mapcCells) << "/" << m_vspcExpr[1]->GENERATOR(_mapcCells);
        break;
    case cExpr_t::encExprType_t::ARITH_UNARY_MINUS:
        ss << "-" << m_vspcExpr[0]->GENERATOR(_mapcCells);
        break;
    case cExpr_t::encExprType_t::ARITH_IN_PAREN:
        ss << "(" << m_vspcExpr[0]->GENERATOR(_mapcCells) << ")";
        break;
    case cExpr_t::encExprType_t::STRING:
        ss << "\"" << m_strString << "\"";
        break;
    case cExpr_t::encExprType_t::EQ:
        ss << "(" << m_vspcExpr[0]->GENERATOR(_mapcCells) << "==" << m_vspcExpr[1]->GENERATOR(_mapcCells) << ")";
        break;
    case cExpr_t::encExprType_t::NEQ:
        ss << "(" << m_vspcExpr[0]->GENERATOR(_mapcCells) << "<>" << m_vspcExpr[1]->GENERATOR(_mapcCells) << ")";
        break;
    case cExpr_t::encExprType_t::GT:
        ss << "(" << m_vspcExpr[0]->GENERATOR(_mapcCells) << ">" << m_vspcExpr[1]->GENERATOR(_mapcCells) << ")";
        break;
    case cExpr_t::encExprType_t::GE:
        ss << "(" << m_vspcExpr[0]->GENERATOR(_mapcCells) << ">=" << m_vspcExpr[1]->GENERATOR(_mapcCells) << ")";
        break;
    case cExpr_t::encExprType_t::LT:
        ss << "(" << m_vspcExpr[0]->GENERATOR(_mapcCells) << "<" << m_vspcExpr[1]->GENERATOR(_mapcCells) << ")";
        break;
    case cExpr_t::encExprType_t::LE:
        ss << "(" << m_vspcExpr[0]->GENERATOR(_mapcCells) << "<=" << m_vspcExpr[1]->GENERATOR(_mapcCells) << ")";
        break;
    }
    return ss.str();

    #undef GENERATOR 
}
