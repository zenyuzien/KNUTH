
/*

coding style: GNU
@author Rushikesh Muraharisetty

*/

#include "config/config.h"
#include "gi/gi.h"
#include "parse/parse.h"
#include "regroup/regroup.h"
#include "ssa2/ssa2.h"
#include <cstdlib>
#include <memory>

std::string
browseFile ()
{
    char buffer[1024];
    std::string filePath;

    std::string command = "zenity --file-selection --title=\"Select a File\" 2>/dev/null";

    FILE *pipe = popen (command.c_str (), "r");
    if (!pipe)
        {
            std::cerr << "Failed to open Zenity dialog. Check if Zenity is installed.\n";
            return "";
        }

    if (fgets (buffer, sizeof (buffer), pipe) != nullptr)
        {
            filePath = buffer;
            filePath.erase (filePath.find_last_not_of ("\n") + 1); // Trim newline
        }

    int status = pclose (pipe);
    if (status == -1)
        {
            std::cerr << "Error closing the pipe.\n";
            return "";
        }

    if (filePath.empty ())
        {
            std::cerr << "No file selected or dialog was canceled.\n";
        }

    return filePath;
}

int
main ()
{
    std::ofstream wr ("output/result.txt");

    init_wr (wr);

    std::vector<entry> entries, original_rules;
    bool intdone = false, ssadone = false, regrpdone = false, isfile = false;
    int nR;

    int choice;
again:
    std::cout << "\n\nchoose operation: \n1. choose a file\n2. Find "
                 "intersections\n3. Apply SSA-2\n4. Apply re-grouping\n5. exit\n";
    std::cin >> choice;
    if (choice == 1)
        {
            wr << "\nchoosing a file.. \n";
        again2:
            
            std::cout << "enter 1 to choose existing parsed textfile.\nenter 2 to "
                         "choose the default ruleset provided in the "
                         "src/config/config.cpp file.\nenter 3 to browse new file\n";
            std::cin >> choice;
            if (choice == 1)
                {
                    nR = fill_from_file (entries, PATH_TO_PROJECT+ RULESET, false);
                    original_rules = entries;
                }
            else if (choice == 2)
                {
                    nR = fill_from_file (entries, PATH_TO_PROJECT+ RULESET);
                    original_rules = entries;
                }
            else if (choice == 3)
                {
                    std::string filePath = browseFile();
                    if (filePath.empty()) 
                    {
                        std::cout << "No file selected or operation canceled." << std::endl;
                        goto again2;
                    }
                    
                    nR = fill_from_file (entries, filePath);
                    original_rules = entries;
                }
            else
                {
                    std::cout << "invalid choice, try again \n";
                    goto again2;
                }
            if (!nR)
                return 1;
            wr << "activating file: " << PATH_TO_PROJECT << RULESET << " \n";
            isfile = true;
            ssadone = false;
            regrpdone = false;
            intdone = false;
            goto again;
        }
    else if (choice == 2)
        {
            if (!isfile)
                {
                    std::cout << "choose file first ! \n";
                    goto again;
                }
            if (intdone)
                {
                    std::cout << "already intersections found for " << RULESET << ". Refer output/result.txt for more information\n";
                    goto again;
                }
            wr << "\napplying GI.. \n";
            auto nI = INTERSECTIONS (entries, wr);
            intdone = true;
            std::cout << "found " << nI
                      << " intersections ! Refer output/result.txt for more "
                         "information\n";
            goto again;
        }
    else if (choice == 3)
        {
            if (!isfile)
                {
                    std::cout << "choose file first ! \n";
                    goto again;
                }
            if (!intdone)
                {
                    std::cout << "Need to find intersections first for "
                                 "applying SSA-2 ! \n";
                    goto again;
                }
            if (ssadone)
                {
                    std::cout << "Already applied SSA-2! Check result.txt for "
                                 "more information \n";
                    goto again;
                }
            struct ssa_utility ssa_g;
            std::vector<int> s1, s2, s3;
            std::cout << "apply ssa-2 ! \n";
            wr << "\nApplying SSA-2 ! \n";
            SSA_method (ssa_g, s1, s2, s3, wr, nR, entries, 0, false);
            goto again;
        }
    else if (choice == 4)
        {
            if (!isfile)
                {
                    std::cout << "choose file first ! \n";
                    goto again;
                }
            if (regrpdone)
                {
                    std::cout << "already regrouping done for this file, check "
                                 "result.txt for the result, or choose file again \n";
                    goto again;
                }

            wr << "\nPerforming re-grouping algo: \n";
            std::cout << "re grouping ! \n";
            regroup (original_rules, wr, false);
            std::cout << "re-grouping done! \n";
            regrpdone = true;
            goto again;
        }
    else if (choice == 5)
        {
            std::cout << "adios amigo ! \n";
        }
    else
        {
            std::cout << "invalid choice, please try again ! \n";
            goto again;
        }

    wr << "\n*************************************************************\n";
    wr.close ();
    appendFileContent (PATH_TO_PROJECT + "output/result.txt", PATH_TO_PROJECT + "output/history.txt");

    return 0;
}
