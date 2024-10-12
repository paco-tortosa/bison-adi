#ifndef cExpr_t_hh
#define cExpr_t_hh

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

class cExpr_t
{
public:
    static std::map<std::string, std::string> s_MapAlias;

    enum class encType_t {
        UNDEFINED, 
        NUMBER,
        STRING,
        ID,
        CELL,
        CELL_WITH_SHEET,
        RANGE, 
        FUNCTION, 
        ARITH_ADD,
        ARITH_SUB,
        ARITH_MUL,
        ARITH_DIV,
        ARITH_UNARY_MINUS,
        ARITH_IN_PAREN,
        GT,
        GE,
        LT,
        LE,
        EQ,
        NEQ
    };

    cExpr_t()
        : m_encType(encType_t::UNDEFINED)
    {
    }

    encType_t m_encType;

    //Value
    double m_dValue;                                    //encType_t::NUMBER  -> m_dValue
    std::string m_strString;                            //encType_t::STRING
                                                        //encType_t::ID
    std::string m_strSheet;                             //encType_t::CELL_WITH_SHEET
    std::string m_strCell;                              //encType_t::CELL    -> m_strCell
    std::string m_strCell2;                             //encType_t::RANGE   -> m_strCell:m_strCell2
    std::string m_strFunction;                          //encType_t::FUNCTION

    std::vector<std::shared_ptr<cExpr_t>> m_vspcExpr;   //encType_t::FUNCTION (args)
                                                        //encType_t::ARITH_ADD
                                                        //encType_t::ARITH_SUB
                                                        //encType_t::ARITH_MUL
                                                        //encType_t::ARITH_DIV
                                                        //encType_t::ARITH_UNARY_MINUS
                                                        //encType_t::ARITH_IN_PAREN
                                                        //encType_t::GT
                                                        //encType_t::GE
                                                        //encType_t::LT
                                                        //encType_t::LE
                                                        //encType_t::EQ
                                                        //encType_t::NEQ

    std::string m_GetXlsStyleCode();
    std::string m_GetCStyleCode();
};

#endif
