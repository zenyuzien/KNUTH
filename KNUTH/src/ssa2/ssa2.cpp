
#include "../config/config.h"
#include "ssa2.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
void
PRE_SSA (struct ssa_utility &ssa_g, std::vector<entry> &db)
{
    long ic = 0;
    long nI = 0;
    long double I1;
    auto itr = db.begin ();
    while ((*itr).ors.size () == 1)
        itr++;

    for (; itr < db.end (); ++itr)
        {

            ++nI;
            I1 = 0;

            for (auto itr_ = (*itr).ors.begin (); itr_ != (*itr).ors.end ();
                 ++itr_)
                {
                    ssa_g.table[*(itr_)].emplace_back (ic);
                    I1++;
                }
            ic += 2;
            I1 = 1 / std::pow (2, I1);
            if (I1)
                {
                    ssa_g.CL.push_back ({ I1, true });
                    ssa_g.CL.push_back ({ I1, true });
                }
        }
}

void
writeSet (const std::vector<int> &vec, const std::string &filename)
{
    std::ofstream outFile (filename);

    if (!outFile)
        {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
    for (const auto &value : vec)
        outFile << value << "\n";

    outFile.close ();
}

void
SSA_HYBRID (struct ssa_utility &g, std::vector<int> &s1, std::vector<int> &s2,
            std::vector<int> &s3, bool eq)
{

    std::vector<bool> visit (g.nf, false);
    long done = g.nf;

    while (done)
        {
            long double psum = 0, nsum = 0;
            long max_i = -1;
            long double max_ratio = -1;
            for (long i = g.nf - 1; i >= 0; i--)
                {
                    psum = 0, nsum = 0;
                    if (visit[i] == true)
                        continue;
                    for (auto j : g.table[i])
                        {

                            if (g.CL[j].second)
                                psum += g.CL[j].first;
                            if (g.CL[j + 1].second)
                                nsum += g.CL[j + 1].first;
                        }

                    long double ratio;
                    if (nsum == 0)
                        ratio = psum;
                    else
                        ratio = psum / nsum;

                    if (eq)
                        {
                            if ((ratio > max_ratio)
                                || (ratio == max_ratio
                                    && (g.table[i].size ()
                                        < g.table[max_i].size ())))
                                {
                                    max_i = i;
                                    max_ratio = ratio;
                                }
                        }
                    else
                        { // eq false ->nclauses max
                            if ((ratio > max_ratio)
                                || (ratio == max_ratio
                                    && (g.table[i].size ()
                                        > g.table[max_i].size ())))
                                {
                                    max_i = i;
                                    max_ratio = ratio;
                                }
                        }
                }

            long i = max_i;
            visit[max_i] = true;
            done--;

            psum = 0;
            nsum = 0;
            if (g.table[i].size () == 0)
                {
                    s3.emplace_back (i + 1);
                    continue;
                }

            for (auto j : g.table[i])
                {

                    if (g.CL[j].second)
                        psum += g.CL[j].first;
                    if (g.CL[j + 1].second)
                        nsum += g.CL[j + 1].first;
                }

            if (psum > nsum)
                {
                    s1.emplace_back (i + 1);

                    for (auto j : g.table[i])
                        g.CL[j].second = false;
                    for (auto j : g.table[i])
                        g.CL[j + 1].first *= 2;
                }
            else
                {
                    s2.emplace_back (i + 1);

                    for (auto j : g.table[i])
                        g.CL[j + 1].second = false;
                    for (auto j : g.table[i])
                        g.CL[j].first *= 2;
                }
        }
}

void
writeIntersections (const std::vector<entry> &db, const std::string path)
{
    std::cout << path;
    std::ofstream wr (path);
    if (!wr)
        {
            std::cerr << "unable to open int.txt \n";
        }
    auto itr = db.begin ();
    while ((*itr).ors.size () == 1)
        itr++;
    for (; itr < db.end (); ++itr)
        {
            for (auto itr_ = (*itr).ors.begin (); itr_ != (*itr).ors.end ();
                 ++itr_) //{
                wr << *(itr_) + 1 << " ";
            // std::cout << * (itr_) + 1 << " ";}
            wr << "\n";
        }
    wr.close ();
}

void
SSA_method (struct ssa_utility &ssa_g, std::vector<int> &s1,
            std::vector<int> &s2, std::vector<int> &s3, std::ofstream &wr,
            long nR, std::vector<entry> &db, int method, bool printInt)
{
    ssa_g.nf = 0;
    ssa_g.table.clear ();
    ssa_g.CL.clear ();
    ssa_g.nf = nR;
    ssa_g.table.resize (ssa_g.nf);
    std::string tmp;
    s1.clear ();
    s2.clear ();
    s3.clear ();

    std::string path_to_int = PATH_TO_PROJECT + "output/GI/int.txt";
    writeIntersections (db, path_to_int);
    PRE_SSA (ssa_g, db);
    /*
      if (method == 1) {
        tmp =
      "C:/Users/rushi/OneDrive/Desktop/IOT_RESEARCH/SSA-2_updated/linear_asc/";
        SSA_Linear(ssa_g, s1, s2, s3, true);
      } else if (method == 2) {
        tmp =
      "C:/Users/rushi/OneDrive/Desktop/IOT_RESEARCH/SSA-2_updated/linear_dsc/";
        SSA_Linear(ssa_g, s1, s2, s3, false);
      } else if (method == 3) {
        tmp =
      "C:/Users/rushi/OneDrive/Desktop/IOT_RESEARCH/SSA-2_updated/no_clauses_mim/";
        SSA_nClauses(ssa_g, s1, s2, s3, true);
      } else if (method == 4) {
        tmp =
      "C:/Users/rushi/OneDrive/Desktop/IOT_RESEARCH/SSA-2_updated/no_clauses_max/";
        SSA_nClauses(ssa_g, s1, s2, s3, false);
      } else if (method == 5) {
        tmp =
      "C:/Users/rushi/OneDrive/Desktop/IOT_RESEARCH/SSA-2_updated/ratio_1/";
        SSA_ratioMethod(ssa_g, s1, s2, s3, true);
      } else if (method == 6) {
        tmp =
      "C:/Users/rushi/OneDrive/Desktop/IOT_RESEARCH/SSA-2_updated/ratio_2/";
        SSA_ratioMethod(ssa_g, s1, s2, s3, false);
      } else if(method == 7) {
        tmp =
      "C:/Users/rushi/OneDrive/Desktop/IOT_RESEARCH/SSA-2_updated/HYBRID_min/";
        SSA_HYBRID(ssa_g, s1, s2, s3, true);
      } else if(method == 8) {
        tmp =
      "CC:/Users/rushi/OneDrive/Desktop/IOT_RESEARCH/SSA-2_updated/HYBRID_max/";
        SSA_HYBRID(ssa_g, s1, s2, s3, false);
      } else if(method == 9)  {
        tmp =
      "C:/Users/rushi/OneDrive/Desktop/IOT_RESEARCH/SSA-2_updated/mySSA/";
        mySSA(ssa_g, 5, s1, s2, s3, wr,tmp);
        return;
      }*/
    SSA_HYBRID (ssa_g, s1, s2, s3, false);
    std::sort (s1.begin (), s1.end ());
    std::sort (s2.begin (), s2.end ());
    wr << "\nafter SSA-2 " << tmp << std::endl
       << "set1: " << s1.size () << " rules \nset2: " << s2.size ()
       << " rules \nset3(any set): " << s3.size () << " rules\n";
    std::cout << "\nafter SSA-2 " << tmp << std::endl
              << "set1: " << s1.size () << " rules \nset2: " << s2.size ()
              << " rules \nset3(any set): " << s3.size () << " rules\n";

    writeSet (s1, PATH_TO_PROJECT + "output/SSA2/set1.txt");
    writeSet (s2, PATH_TO_PROJECT + "output/SSA2/set2.txt");
    writeSet (s3, PATH_TO_PROJECT + "output/SSA2/set3.txt");
    /*
      writestd::vectorToFile(s1, tmp + "set1.txt");
      writestd::vectorToFile(s2, tmp + "set2.txt");
      writestd::vectorToFile(s3, tmp + "set3.txt");*/
    /*

      if (printInt == true)
        intfor(s1.size(),s2.size(), wr, tmp);*/
}
