#include <iostream>
#include <sstream>
#include "cDriver_t.hpp"

int main()
{
  cDriver_t cDriver;

  std::stringstream ss;

  //Input
  ss << "A2 = 1" << std::endl;
  ss << "A3 = 2" << std::endl;
  ss << "A4 = 3" << std::endl;

  //Functions
  ss << "C2 = A2+A3*A4" << std::endl;
  ss << "B2 = SUM(C2,4)" << std::endl;
  ss << "B3 = 23-SUM(3+4,A2)" << std::endl;
  ss << "B4 = SUM(A1:A4)" << std::endl;
  ss << "B5 = SUM(3+4*A2,A2,A1,SUM(2,3))" << std::endl;

  if( cDriver.m_Parse(ss.str()) == 0 ){
      //Result
      std::cout << "OK" << std::endl;

      for (auto& c : cDriver.m_cApp.m_mapCells) {
          std::cout << c.first << " = " << c.second << std::endl;
      }
  }
}