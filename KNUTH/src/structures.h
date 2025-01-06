#ifndef _str
#define _str 

#include <set>
#include <vector>
#include <utility>
#include <cstdint>

typedef struct {
    std::set<int> ors;
    std::vector<std::pair<uint32_t, uint32_t>> d;


}entry;

struct ssa_utility {
  long nf;
  std::vector < std::vector < uint32_t >> table;
  std::vector < std::pair < long double, bool >> CL;
  // CL.first -> cost 
  // CL.second -> bool

};


#endif