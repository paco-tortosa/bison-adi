#include <iostream>
#include <sstream>
#include <string>

bool ParseBuffer(const std::string& _strBuffer, std::string& _strOutput);

int main()
{
    std::stringstream ss;
    ss << "var = 12" << std::endl;
    ss << "var1 = 3.14159" << std::endl;
    ss << "quit" << std::endl;

    std::string strOutput;
    if( ParseBuffer(ss.str(), strOutput)){
        std::cout << strOutput;
    }
}
