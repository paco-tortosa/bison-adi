#include <iostream>
#include "driver.hh"

int main()
{
  driver drv;

  std::string strOperations;
  strOperations += "three := 3\n";
  strOperations += "seven := one + two * three\n";
  strOperations += "seven * seven";
  drv.m_Parse(strOperations);
}