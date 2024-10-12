#include <iostream>
#include "cDriver_t.hpp"

int main()
{
  cDriver_t cDriver;

  std::string strOperations;
  strOperations += "1+2\n";
  strOperations += "\n\n\n";
  strOperations += "1-2\n";
  strOperations += "-5+2\n";
  strOperations += "-(5+2)\n";
  strOperations += "3-(5+2)\n";

  if( cDriver.m_Parse(strOperations) == 0 ){
      //Result
      std::cout << "OK";
  }
}