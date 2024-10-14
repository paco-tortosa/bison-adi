#ifndef cExpr_t_hh
#define cExpr_t_hh

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>

class cExpr_t
{
public:
    static std::map<std::string, std::string> s_MapAlias;

    enum class encExprType_t {
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
    encExprType_t m_encExprType;

    enum class encExprDataType_t {
        UNDEFINED, 
        NUMBER,
        STRING
    };
    encExprDataType_t m_encExprDataType;

    cExpr_t()
        : m_encExprType(encExprType_t::UNDEFINED)
        , m_encExprDataType(encExprDataType_t::UNDEFINED)
    {
    }

    std::string m_GetExprDataType(){
        switch(this->m_encExprDataType){
        case encExprDataType_t::UNDEFINED:
            return "UNDEFINED";
        case encExprDataType_t::NUMBER:
            return "NUMBER";
        case encExprDataType_t::STRING:
            return "STRING";
        }
    }

    static encExprDataType_t s_GetExprDataTypeOfFunction(const std::string& _strFunction, std::vector<std::shared_ptr<cExpr_t>>& _vcExprArgs){
        if(_strFunction == "IF"){
            if(_vcExprArgs.size() >= 3){
                return _vcExprArgs[2]->m_encExprDataType;
            }
        }
        return encExprDataType_t::UNDEFINED;
    }

    void m_AddDependency(cExpr_t& _cExpr){
        for(auto& c:_cExpr.m_vstrDependOnCells){
            if(std::find(m_vstrDependOnCells.begin(), m_vstrDependOnCells.end(), c) == m_vstrDependOnCells.end()) {
                m_vstrDependOnCells.push_back(c);
            }
        }
    }

    std::string m_PrintDependencies(){
        std::stringstream ss;
        for(auto& c:m_vstrDependOnCells){
            if(ss.str().size()){
                ss << ", ";
            }
            ss << c;
        }
        return ss.str();
    }

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

    //Depends on cells
    std::vector<std::string> m_vstrDependOnCells;

    std::string m_GetXlsStyleCode();
    std::string m_GetCStyleCode();
};

#endif
