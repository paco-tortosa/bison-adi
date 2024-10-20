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

    std::string m_GetExprTypeAndValue(){
        switch(m_encExprType){
        case encExprType_t::UNDEFINED:
            return "UNDEFINED";
        case encExprType_t::NUMBER:
            return "NUMBER " + std::to_string(m_dValue);
        case encExprType_t::STRING:
            return "STRING \"" + m_strString + "\"";
        case encExprType_t::ID:
            return "ID";
        case encExprType_t::CELL:
            return "CELL " +  m_strCell;
        case encExprType_t::CELL_WITH_SHEET:
            return "CELL_WITH_SHEET " + m_strSheet + "!" + m_strCell;
        case encExprType_t::RANGE:
            return "RANGE";
        case encExprType_t::FUNCTION:
            return "FUNCTION";
        case encExprType_t::ARITH_ADD:
            return "ARITH_ADD";
        case encExprType_t::ARITH_SUB:
            return "ARITH_SUB";
        case encExprType_t::ARITH_MUL:
            return "ARITH_MUL";
        case encExprType_t::ARITH_DIV:
            return "ARITH_DIV";
        case encExprType_t::ARITH_UNARY_MINUS:
            return "ARITH_UNARY_MINUS";
        case encExprType_t::ARITH_IN_PAREN:
            return "ARITH_IN_PAREN";
        case encExprType_t::GT:
            return "GT";
        case encExprType_t::GE:
            return "GE";
        case encExprType_t::LT:
            return "LT";
        case encExprType_t::LE:
            return "LE";
        case encExprType_t::EQ:
            return "EQ";
        case encExprType_t::NEQ:
            return "NEQ";
        default:
            return "???";
        }
    }

    encExprType_t m_encExprType;

    enum class encExprDataType_t {
        UNDEFINED, 
        NUMBER,
        STRING,
        REFERENCE
    };

    cExpr_t()
        : m_encExprType(encExprType_t::UNDEFINED)
        , m_encExprDataType(encExprDataType_t::UNDEFINED)
        , m_pcExpr_ReferenceForExprDataType(nullptr)
    {
    }

    std::string m_GetExprDataType(){
        switch(this->m_encExprDataType){
        case encExprDataType_t::REFERENCE:
            return "Reference: "+m_pcExpr_ReferenceForExprDataType->m_GetExprTypeAndValue() + " (" + m_pcExpr_ReferenceForExprDataType->m_GetExprDataType() + ") ";
        case encExprDataType_t::NUMBER:
            return "NUMBER";
        case encExprDataType_t::STRING:
            return "STRING";
        case encExprDataType_t::UNDEFINED:
        default:
            return "UNDEFINED";
        }
    }

    bool m_IsFinalDataType(){
        switch(this->m_encExprDataType){
        case encExprDataType_t::NUMBER:
        case encExprDataType_t::STRING:
            return true;
        default:
            return false;
        }
    }

    void m_SetExprDataType(encExprDataType_t _encExprDataType){
        this->m_encExprDataType = _encExprDataType;
    }

    static cExpr_t* s_GetExprDataTypeOfFunction(const std::string& _strFunction, std::vector<std::shared_ptr<cExpr_t>>& _vcExprArgs){
        if(_strFunction == "IF"){
            if(_vcExprArgs.size() >= 3){
                return _vcExprArgs[2].get();
            }
        }
        return nullptr;
    }

    void m_AddDependency(cExpr_t& _cExpr){
        for(auto& c:_cExpr.m_vstrDependOnCells){
            if(std::find(m_vstrDependOnCells.begin(), m_vstrDependOnCells.end(), c) == m_vstrDependOnCells.end()) {
                m_vstrDependOnCells.push_back(c);
            }
        }
    }

    void m_SetExprReferenceForDataType(cExpr_t* _pcExpr){
        if(_pcExpr){
            m_pcExpr_ReferenceForExprDataType = (_pcExpr->m_pcExpr_ReferenceForExprDataType) ? _pcExpr->m_pcExpr_ReferenceForExprDataType : _pcExpr;
            m_encExprDataType = encExprDataType_t::REFERENCE;
        }
    }

    bool m_HasAPendingReference(std::string& _strReference){
        switch(m_encExprDataType){
            case encExprDataType_t::REFERENCE:
                return m_pcExpr_ReferenceForExprDataType->m_HasAPendingReference(_strReference);
            case encExprDataType_t::NUMBER:
            case encExprDataType_t::STRING:
            case encExprDataType_t::UNDEFINED:
            default:
                return false;
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

    //Reference (cell that gives the expr data type)
    cExpr_t* m_pcExpr_ReferenceForExprDataType;

    std::string m_GetXlsStyleCode();
    std::string m_GetCStyleCode(std::map<std::string, cExpr_t>& _mapcCells);

    encExprDataType_t m_encExprDataType;
};

#endif
