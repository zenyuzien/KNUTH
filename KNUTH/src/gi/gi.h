#ifndef _gi
#define _gi

#include <fstream>

#include "../structures.h"

#define IS_POINT(r) \
    ({ \
        bool result = true; \
        for (int dim = 0; dim <= 4; dim++) { \
            if (r.d[dim].first != r.d[dim].second) { \
                result = false; \
                break; \
            } \
        } \
        result; \
    })

#define IS_INT(r1, r2) \
    ({ \
        bool result = true; \
        for (int dim = 0; dim <= 4; dim++) { \
            if ((r1.d[dim].first > r2.d[dim].first ? r1.d[dim].first : r2.d[dim].first) > \
                (r1.d[dim].second < r2.d[dim].second ? r1.d[dim].second : r2.d[dim].second)) { \
                result = false; \
                break; \
            } \
        } \
        result; \
    })

int INTERSECTIONS( std::vector <entry> & db, std::ofstream & wr);
#endif 