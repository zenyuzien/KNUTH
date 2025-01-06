#ifndef PARSE_H
#define PARSE_H

#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../structures.h"
int fill_from_file (std::vector<entry> &db, const std::string path_to_file, bool needtoparse = true);
void init_wr (std::ofstream &wr);
bool appendFileContent (const std::string &file1, const std::string &file2);
/*

ORS (Original Rule Set):
The ORS value determines the type of rule entry:

    ORS == 1: The entry is considered an original rule.
    ORS > 1: The entry is considered an intersection of multiple rules.

D (Dimensions):
The d value refers to the number of dimensions or parameters being considered.
For example, when d = 5, the entry refers to a 5-tuple: (SIP, DIP, SP, DP,
PTRCL) — representing Source IP (SIP), Destination IP (DIP), Source Port (SP),
Destination Port (DP), and Protocol (PTRCL).

Range and Point Rules:
The "pair" represents the range for each dimension. It defines the "from" and
"to" values for the corresponding tuple entry.

    If the range pair (for each dimension) has the same "from" and "to" value,
then the entry is a point rule (also called a non-range rule). This means there
is no variation in that dimension; it’s a specific value rather than a range.

    If the range pair has different values for "from" and "to," then the entry
represents a range rule (where the value can vary between the "from" and "to"
values).
*/

#endif
