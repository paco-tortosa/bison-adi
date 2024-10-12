#include <iostream>
#include "cDriver_t.hpp"

int main()
{
  cDriver_t cDriver;

  std::string strOperations;
  strOperations += "three := 3\n";
  strOperations += "seven := one + two * three\n";
  strOperations += "seven * seven";

  if( cDriver.m_Parse(strOperations) == 0 ){
      //Result
      std::cout << "Result = " << cDriver.m_cApp.m_iResult;
  }
}