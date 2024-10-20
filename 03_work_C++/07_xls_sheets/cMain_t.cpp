#include <iostream>
#include <fstream>
#include <sstream>
#include "cDriver_t.hpp"

int main()
{
    cDriver_t cDriver;

    ////Input
    //std::stringstream ss;
    //ss << "sheet = \"Calculator\"" << std::endl;
    //ss << "Calculator!A26 = \"4T4R\"" << std::endl;
    //ss << "Calculator!B13 = \"4T4R\"" << std::endl;
    //ss << "Calculator!B14 = 0" << std::endl;
    //ss << "Calculator!B15 = 1" << std::endl;
    //ss << "Calculator!B26 = IF(OR(A26=\"4T4R\",A26=\"4T4R_LS\"),0,1)" << std::endl;
    //ss << "Calculator!B30 = 1" << std::endl;
    //ss << "Calculator!B34 = 0" << std::endl;
    //ss << "Calculator!B35 = 1" << std::endl;
    //ss << "Calculator!D13 = IF(OR(B13=\"4T4R\",B13=\"4T4R_LS\"),0,1)" << std::endl;
    //ss << "Calculator!F13 = IF(D13=0,IF(B14=1,rx_adc_delay_4t4r_rand_min,0)+IF(B15,rx_adc_delay_4t4r_seq_min,0),IF(B14=1,rx_adc_delay_8t8r_rand_min,0)+IF(B15,rx_adc_delay_8t8r_seq_min,0))" << std::endl;
    //ss << "Calculator!F14 = IF(D13=0,IF(B14=1,inline_constants!B2,0)+IF(B15,rx_adc_delay_4t4r_seq_min,0),IF(B14=1,rx_adc_delay_8t8r_rand_min,0)+IF(B15,rx_adc_delay_8t8r_seq_min,0))" << std::endl;
    //ss << "Calculator!I13 = IF(B30=0,IF(SUM(B34:B35)+B26>0,\"ERROR:fddc bmem, fsrc, cfir are to be enabled only in fine enable cases\",\"\"),\"\")" << std::endl;
    //ss << "Calculator!K13 = 3*IF(D13=1,4,8)" << std::endl;
    //ss << "Calculator!K14 = IF(D13=1,4,8)" << std::endl;
    //ss << "Calculator!K15 = IF(D13=1,\"4\",\"8\")" << std::endl;
    //ss << "Calculator!K16 = IF(D13=1,A26,B13)" << std::endl;
    //ss << "Calculator!K17 = IF(D13=1,B14,Z99)" << std::endl;
    //ss << "Calculator!Z99 = \"4T4R\"" << std::endl;
    //ss << "rx_adc_delay_4t4r_rand_min alias inline_constants!B2" << std::endl;
    //ss << "rx_adc_delay_4t4r_seq_min alias inline_constants!B3" << std::endl;
    //ss << "rx_adc_delay_8t8r_rand_min alias inline_constants!B4" << std::endl;
    //ss << "rx_adc_delay_8t8r_seq_min alias inline_constants!B5" << std::endl;
    //std::string strText2Parse = ss.str();

    std::ifstream f_in("D:\\BackBlaze\\Me_June2020\\SVN\\Cursos\\_Languages\\02_Data\\297\\1628_Info\\files_github\\00_openpyxl\\02_full\\output.txt");
    std::ostringstream ss;
    //ss << f_in.rdbuf() << std::endl;

    int iMaxLines = 13999;
    int iLines = 1;
    for( std::string line; std::getline( f_in, line ); )
    {
        if(iLines > iMaxLines){
            break;
        }
        ss << line << std::endl;
        iLines++;
    }
    std::string strText2Parse = ss.str();

    if( cDriver.m_Parse(strText2Parse) == 0 ){
        //Result
        std::cout << "OK" << std::endl;

        //Complete references
        int iIteration = 1;
        int iUndefines = 0;
        while(true){
            for(auto& it:cDriver.m_cApp.m_mapcCells){
                auto& cell = it.first;
                auto& expr = it.second;
                auto* exprRef = expr.m_pcExpr_ReferenceForExprDataType;

                if( expr.m_IsFinalDataType() ){
                    continue;
                }
                if(expr.m_encExprDataType == cExpr_t::encExprDataType_t::REFERENCE){
                    if( exprRef->m_IsFinalDataType() ){
                        expr.m_encExprDataType = exprRef->m_encExprDataType;
                    }
                    else if(exprRef->m_encExprDataType == cExpr_t::encExprDataType_t::UNDEFINED &&
                            exprRef->m_encExprType == cExpr_t::encExprType_t::CELL_WITH_SHEET){
                        std::string cellWithSheet = exprRef->m_strSheet + "!" + exprRef->m_strCell;
                        if(cDriver.m_cApp.m_mapcCells.count(cellWithSheet)){
                            if( cDriver.m_cApp.m_mapcCells[cellWithSheet].m_IsFinalDataType() ){
                                exprRef->m_encExprDataType = cDriver.m_cApp.m_mapcCells[cellWithSheet].m_encExprDataType;
                                expr.m_encExprDataType = exprRef->m_encExprDataType;
                            }
                        }
                    }
                }               
            }
            auto iPrevUndefines = iUndefines;
            iUndefines = 0;
            for(auto& it:cDriver.m_cApp.m_mapcCells){
                auto& expr = it.second;
                if(expr.m_encExprDataType == cExpr_t::encExprDataType_t::UNDEFINED){
                    iUndefines++;
                }               
            }
            if(iUndefines == 0){
                break;
            }
            if(iPrevUndefines == iUndefines){
                break;
            }
        }
        std::cout << std::endl;

        if(iUndefines){
            std::cout << "Undefines = " << iUndefines << std::endl;

            for(auto& it:cDriver.m_cApp.m_mapcCells){
                auto& cell = it.first;
                auto& expr = it.second;

                if(expr.m_IsFinalDataType()){
                    continue;
                }

                std::cout << cell << " = " << expr.m_GetXlsStyleCode() << std::endl; 
                std::cout << cell << " = " << expr.m_GetCStyleCode(cDriver.m_cApp.m_mapcCells) << std::endl; 
                std::cout << cell << " => " << expr.m_GetExprDataType(); 
                std::string strDependencies = expr.m_PrintDependencies();
                if(strDependencies.size()){
                    std::cout << ", depends on " << strDependencies << std::endl;
                }
                else{
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
        }

        std::cout << std::endl;

        for(auto& it:cDriver.m_cApp.m_mapcCells){
            auto& cell = it.first;
            auto& expr = it.second;
            std::cout << cell << " = " << expr.m_GetXlsStyleCode() << std::endl; 
            std::cout << cell << " = " << expr.m_GetCStyleCode(cDriver.m_cApp.m_mapcCells) << std::endl; 
            std::cout << cell << " => " << expr.m_GetExprDataType(); 
            std::string strDependencies = expr.m_PrintDependencies();
            if(strDependencies.size()){
            std::cout << ", depends on " << strDependencies << std::endl;
            }
            else{
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
}
