#ifndef _regrp
#define _regrp
#include "../structures.h"
#include <fstream>

int regroup (const std::vector<entry> db, std::ofstream &wr, const bool debug = false);

#endif