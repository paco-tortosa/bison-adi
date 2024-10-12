#ifndef cApp_t_hh
#define cApp_t_hh

#include <string>
#include <map>

class cApp_t
{
public:
    cApp_t() 
        : m_iResult (0)
    {
        m_mapVariables["one"] = 1;
        m_mapVariables["two"] = 2;
    }

    std::map<std::string, int> m_mapVariables;
    int m_iResult;                      
};
#endif
