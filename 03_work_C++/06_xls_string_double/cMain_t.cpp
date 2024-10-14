#include <iostream>
#include <sstream>
#include "cDriver_t.hpp"

int main()
{
  cDriver_t cDriver;

  std::stringstream ss;

  //Input
  ss << "rx_adc_delay_4t4r_rand_min alias inline_constants!B2" << std::endl;
  ss << "rx_adc_delay_4t4r_seq_min alias inline_constants!B3" << std::endl;
  ss << "rx_adc_delay_8t8r_rand_min alias inline_constants!B4" << std::endl;
  ss << "rx_adc_delay_8t8r_seq_min alias inline_constants!B5" << std::endl;
  ss << "A26 = \"4T4R\"" << std::endl;
  ss << "B13 = \"4T4R\"" << std::endl;
  ss << "B14 = 0" << std::endl;
  ss << "B15 = 1" << std::endl;
  ss << "B26 = IF(OR(A26=\"4T4R\",A26=\"4T4R_LS\"),0,1)" << std::endl;
  ss << "B30 = 1" << std::endl;
  ss << "B34 = 0" << std::endl;
  ss << "B35 = 1" << std::endl;
  ss << "D13 = IF(OR(B13=\"4T4R\",B13=\"4T4R_LS\"),0,1)" << std::endl;
  ss << "F13 = IF(D13=0,IF(B14=1,rx_adc_delay_4t4r_rand_min,0)+IF(B15,rx_adc_delay_4t4r_seq_min,0),IF(B14=1,rx_adc_delay_8t8r_rand_min,0)+IF(B15,rx_adc_delay_8t8r_seq_min,0))" << std::endl;
  ss << "F14 = IF(D13=0,IF(B14=1,inline_constants!B2,0)+IF(B15,rx_adc_delay_4t4r_seq_min,0),IF(B14=1,rx_adc_delay_8t8r_rand_min,0)+IF(B15,rx_adc_delay_8t8r_seq_min,0))" << std::endl;
  ss << "I13 = IF(B30=0,IF(SUM(B34:B35)+B26>0,\"ERROR:fddc bmem, fsrc, cfir are to be enabled only in fine enable cases\",\"\"),\"\")" << std::endl;
  ss << "K13 = 3*IF(D13=1,4,8)" << std::endl;
  ss << "K14 = IF(D13=1,4,8)" << std::endl;
  ss << "K15 = IF(D13=1,\"4\",\"8\")" << std::endl;

  if( cDriver.m_Parse(ss.str()) == 0 ){
      //Result
      std::cout << "OK" << std::endl;

      for(auto& a:cDriver.m_cApp.m_vcAssign){
          std::cout << a.m_strCell << " = " << a.m_cExpr.m_GetXlsStyleCode() << std::endl; 
          std::cout << a.m_strCell << " = " << a.m_cExpr.m_GetCStyleCode() << std::endl; 
          std::cout << a.m_strCell << " => " << a.m_cExpr.m_GetExprDataType(); 
          std::string strDependencies = a.m_cExpr.m_PrintDependencies();
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
