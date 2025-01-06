#ifndef _ssa
#define _ssa

#include "../structures.h"
#include <string>

void PRE_SSA (ssa_utility &ssa, std::vector<entry> &entries);
void SSA_HYBRID (ssa_utility &ssa, std::vector<int> &vec1, std::vector<int> &vec2, std::vector<int> &vec3, bool flag);
void SSA_method (ssa_utility &ssa, std::vector<int> &vec1, std::vector<int> &vec2, std::vector<int> &vec3, std::ofstream &outFile, long num, std::vector<entry> &entries, int param, bool flag);

#endif