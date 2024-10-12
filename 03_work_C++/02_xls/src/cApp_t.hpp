#ifndef cApp_t_hh
#define cApp_t_hh

#include <string>
#include <vector>
#include <map>

class cApp_t
{
public:
    cApp_t() 
    {
    }

    std::map<std::string, double> m_mapCells;
    std::vector<double> m_args;
};
#endif
