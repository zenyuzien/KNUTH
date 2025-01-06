#include "gi.h"

#include <iostream>
#include <map>
#include <time.h>

int
INTERSECTIONS (std::vector<entry> &db, std::ofstream &wr)
{

    struct timespec start, end;
    clock_gettime (CLOCK_REALTIME, &start);
    wr << "no.of original rules: " << db.size () << std::endl;

    uint32_t nR = db.size ();
    int starter = db.size () - 1;
    int dbptr = starter;

    std::map<std::vector<std::pair<uint32_t, uint32_t> >, std::pair<std::set<int>, int> > box;
    bool change;

    do
        {
            change = false;
            starter = db.size () - 1;

            std::cout << "new iteration, new starter value: " << starter << std::endl;
            // cout<<".";
            for (int i = starter; i > 0; i--)
                {
                    // if( (i > 1000) && ((i % 1000) == 0) ) std::cout<<"*";// progress bar inefficient
                    for (int j = i - 1; j >= 0; j--)
                        {

                            bool flag = false;
                            for (const auto &element : db[i].ors)
                                {
                                    if (db[j].ors.find (element) == db[j].ors.end ())
                                        {
                                            flag = true;
                                        }
                                }

                            bool different = false;
                            for (const auto &element : db[j].ors)
                                {
                                    if ((db[i].ors.find (element) == db[i].ors.end ()) && (flag))
                                        {
                                            different = true;
                                            break;
                                        }
                                }

                            if (!different || !IS_INT (db[i], db[j]))
                                {
                                    continue;
                                }
                            entry tmp;
                            tmp.d.resize (5);

                            for (int dim = 0; dim <= 4; dim++)
                                {
                                    tmp.d[dim].second = db[i].d[dim].second < db[j].d[dim].second ? db[i].d[dim].second : db[j].d[dim].second;

                                    tmp.d[dim].first = db[i].d[dim].first > db[j].d[dim].first ? db[i].d[dim].first : db[j].d[dim].first;
                                }

                            tmp.ors.insert (db[i].ors.begin (), db[i].ors.end ());
                            tmp.ors.insert (db[j].ors.begin (), db[j].ors.end ());

                            auto isThere = box.find (tmp.d);
                            if (isThere == box.end ())
                                {
                                    db.push_back (tmp);
                                    dbptr++;
                                    change = true;
                                    box.insert ({ tmp.d, { tmp.ors, dbptr } });
                                }
                            else
                                {
                                    int bef = (*isThere).second.first.size ();
                                    (*isThere).second.first.insert (tmp.ors.begin (), tmp.ors.end ());
                                    if ((*isThere).second.first.size () > static_cast<std::set<int>::size_type> (bef))
                                        change = true;
                                    db[(*isThere).second.second].ors.insert (tmp.ors.begin (), tmp.ors.end ());
                                }
                        }
                }
        }
    while (change);

    clock_gettime (CLOCK_REALTIME, &end);
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    wr << "number of intersections: " << db.size () - nR << "\ntime taken to find the intersections: " << time_spent << "\n";

    return db.size () - nR;
}
