#include "../config/config.h"
#include "../gi/gi.h"
#include "regroup.h"
#include <cmath>

#include <iostream>

int
ceilLog2 (int n)
{
    if (n <= 1)
        return 0;
    return static_cast<int> (std::ceil (std::log2 (n)));
}
void
printUtil (const std::vector<std::vector<entry> > &util)
{
    int i = 1;
    std::ofstream wr (PATH_TO_PROJECT + "output/regroup/MMR.txt");
    for (const auto &innerVec : util)
        {
            if (wr)
                wr << "subset " << i++ << ": ";
            else
                std::cout << "subset " << i++ << ": ";
            for (const auto &ent : innerVec)
                {
                    if (!ent.ors.empty ())
                        {
                            if (wr)
                                wr << *ent.ors.begin () << " ";
                            else
                                std::cout << *ent.ors.begin () << " ";
                        }
                }
            if (wr)
                wr << "\n";
            else
                std::cout << std::endl; // New line after each inner vector
        }
    wr.close ();
}

int
bitsforMMR (std::vector<entry> db)
{
    int bits = 0, i, j, k;
    bool flag_skiprow = false, flag_skiprule = false;
    std::vector<std::vector<entry> > util;
    for (i = 0; i < static_cast<int> (db.size ()); i++)
        {
            flag_skiprule = false;
            for (j = 0; j < static_cast<int> (util.size ()); j++)
                {
                    flag_skiprow = false;
                    for (k = 0; k < static_cast<int> (util[j].size ()); k++)
                        {
                            if (IS_INT (db[i], util[j][k]))
                                {
                                    flag_skiprow = true;
                                    break;
                                }
                        }
                    if (flag_skiprow)
                        continue;
                    util[j].push_back (db[i]);
                    flag_skiprule = true;
                }
            if (flag_skiprule)
                continue;
            // assuming no append in those
            // create new row
            util.emplace_back ();
            util[j].push_back (db[i]);
        }
    for (i = 0; i < static_cast<int> (util.size ()); i++)
        bits += ceilLog2 (util[i].size ());
    printUtil (util);
    return bits;
}

int
regroup (const std::vector<entry> db, std::ofstream &wr, const bool debug)
{
    std::ofstream deb (PATH_TO_PROJECT + "/output/debug.txt");
    std::vector<int> HBR, LMR;
    std::vector<entry> MMR;
    int bits = 0, intersections = 0;
    bool flag;
    // HBR: point rules, isolated rules
    // MMR: <= 4 intersections
    // LMR: else
    for (int i = 0; i < static_cast<int> (db.size ()); i++)
        {
            if (debug)
                deb << ">> " << i + 1 << std::endl;
            if (IS_POINT (db[i]))
                {
                    if (debug)
                        {
                            std::cout << i + 1 << " is a point rule >> HBR \n";
                            deb << "\n point rule >> HBR \n";
                        }
                    HBR.push_back (i + 1);
                    continue;
                }
            intersections = 0;
            // TODO OPTIMISE, DO BUBBLE COMPARE AND USE VISIT ARRAY MAYBE?
            flag = false;
            for (int j = 0; j < static_cast<int> (db.size ()); j++)
                {
                    if (IS_INT (db[i], db[j]) && (i != j))
                        {
                            intersections++;
                            if (debug)
                                deb << j + 1 << ", ";
                        }
                    if (intersections > 4)
                        {
                            if (!debug)
                                {
                                    LMR.push_back (i);
                                    flag = true;
                                    break;
                                }
                        }
                }
            if (flag)
                continue;
            if (intersections == 0)
                {
                    if (debug)
                        {
                            deb << "\n isolated rule >> HBR \n";
                            std::cout << i + 1 << " isolated rule >> HBR \n";
                        }
                    HBR.push_back (i);
                }
            else
                {
                    if (intersections < 5)
                        {
                            if (debug)
                                {
                                    std::cout << i + 1 << " <4 int >> MMR \n";
                                    deb << "\n <4 int >> MMR \n";
                                }
                            MMR.push_back (db[i]);
                        }
                    else
                        {
                            if (debug)
                                {
                                    std::cout << i + 1 << " >4 int >> LMR \n";
                                    deb << "\n >4 int >> LMR \n";
                                }
                            LMR.push_back (i);
                        }
                }
        }
    int bits_for_HBR = ceilLog2 ((int)HBR.size ()), bits_for_MMR = bitsforMMR (MMR);
    bits = bits_for_HBR + bits_for_MMR + LMR.size ();
    std::cout << "from a total of " << db.size () << " original rules\nHBR: " << HBR.size () << " MMR: " << MMR.size () << " LMR: " << LMR.size () << " \n";
    wr << "from a total of " << HBR.size () + MMR.size () + LMR.size () << " original rules\nHBR: " << HBR.size () << " MMR: " << MMR.size () << " LMR: " << LMR.size () << " \n";
    std::cout << "bits required to represent HBR: " << bits_for_HBR << ", MMR: " << bits_for_MMR << ", LMR: " << LMR.size () << "\nTotally " << bits << " to represent the " << db.size () << " original rules \n";
    wr << "bits required to represent HBR: " << bits_for_HBR << ", MMR: " << bits_for_MMR << ", LMR: " << LMR.size () << "\nTotally " << bits << " to represent the " << db.size () << " original rules \n";

    return bits;
}