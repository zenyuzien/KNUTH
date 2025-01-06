#include "parse.h"
#include "../config/config.h"
void
init_wr (std::ofstream &wr)
{
    std::time_t now = std::time (nullptr);
    std::tm *localTime = std::localtime (&now);

    char buffer[64];
    std::strftime (buffer, sizeof (buffer), "%Y-%m-%d %H:%M:%S", localTime);

    wr << "*************************** report **************************\n"
       << ">> " << std::string (buffer) << "\n";
}

bool
parse_cb (const std::string &ip, const std::string &op)
{
    std::ifstream inputFile (ip);
    std::ofstream wr (op);

    if (!inputFile.is_open ())
        {
            std::cerr << "parse_cb: Error opening ip file " << ip << std::endl;
            return 0;
        }
    if (!wr.is_open ())
        {
            std::cerr << "parse_cb: Error opening op file " << op << std::endl;
            return 0;
        }

    int ip1_1, ip1_2, ip1_3, ip1_4, mask1, mask2, hexd1, hexd2;
    char chr;
    std::string line, hex1;

    while (getline (inputFile, line))
        {
            if (line[0] != '@')
                continue;
            std::istringstream iss (line);

            uint32_t IP1, IP2, r1l = 0, r2l = 0, ONES_1 = UINT32_MAX, ONES_2 = UINT32_MAX, ZER_1, ZER_2;

            iss >> chr >> ip1_1 >> chr >> ip1_2 >> chr >> ip1_3 >> chr >> ip1_4 >> chr >> mask1;
            IP1 = (ip1_1 << 24) | (ip1_2 << 16) | (ip1_3 << 8) | (ip1_4);
            iss >> ip1_1 >> chr >> ip1_2 >> chr >> ip1_3 >> chr >> ip1_4 >> chr >> mask2;
            IP2 = (ip1_1 << 24) | (ip1_2 << 16) | (ip1_3 << 8) | (ip1_4);
            iss >> ip1_1 >> chr >> ip1_2 >> ip1_3 >> chr >> ip1_4;
            iss >> hex1;

            std::istringstream hexer (hex1);
            getline (hexer, hex1, '/');
            hexd1 = stoul (hex1, nullptr, 16);
            getline (hexer, hex1, '/');
            hexd2 = stoul (hex1, nullptr, 16);
            ONES_1 = (mask1 == 0) ? 0 : ONES_1 << (32 - mask1);
            ONES_2 = (mask2 == 0) ? 0 : ONES_2 << (32 - mask2);

            ZER_1 = (~ONES_1);
            ZER_2 = (~ONES_2);
            r1l = IP1 & ONES_1;
            IP1 = IP1 | ZER_1;
            r2l = IP2 & ONES_2;
            IP2 = IP2 | ZER_2;

            wr << r1l << ":" << IP1 << " " << r2l << ":" << IP2 << " " << ip1_1 << ":" << ip1_2 << " " << ip1_3 << ":" << ip1_4 << " ";
            if (hexd2 == 255)
                wr << hexd1 << ":" << hexd1 << std::endl;
            else
                wr << "0:255\n";
        }

    wr.close ();
    inputFile.close ();
    return 1;
}

int
fill_from_file (std::vector<entry> &db, const std::string path_to_ruleset, bool needtoparse)
{
    if (needtoparse && !parse_cb (path_to_ruleset, PATH_TO_PROJECT + "dataset/parsed_ip.txt"))
        {
            std::cerr << "failed to parse ! \n";
            return 0;
        }
    std::ifstream f (PATH_TO_PROJECT + "dataset/parsed_ip.txt");
    if (!f.is_open ())
        {
            std::cerr << "in fillfromfile: Error opening the file." << std::endl;
            return 0;
        }

    std::string line;
    int nR = 0;

    while (getline (f, line))
        {
            std::istringstream iss (line);

            entry tmp;
            tmp.d.resize (5);

            char delimiter;
            uint32_t first, second;

            int dim = 0;
            while (iss >> first >> delimiter >> second)
                tmp.d[dim++] = { first, second };

            tmp.ors.insert (nR++);
            db.push_back (tmp);
        }
    f.close ();
    // printVector(db);
    return nR;
}

bool
appendFileContent (const std::string &file1, const std::string &file2)
{
    std::ifstream inFile (file1, std::ios::in);
    if (!inFile)
        {
            std::cerr << "Error opening " << file1 << " for reading.\n";
            return false;
        }

    std::ofstream outFile (file2, std::ios::app);
    if (!outFile)
        {
            std::cerr << "Error opening " << file2 << " for appending.\n";
            return false;
        }

    outFile << inFile.rdbuf () << "\n\n";

    inFile.close ();
    outFile.close ();
    return true;
}

/*

void printVector(const std::vector<entry>& vec) {
    for (const auto& e : vec) {
        std::cout << "ors: ";
        for (const auto& val : e.ors) {
            std::cout << val << " ";
        }
        std::cout << "\nd: ";
        for (const auto& p : e.d) {
            std::cout << "(" << p.first << ", " << p.second << ") ";
        }
        std::cout << "\n";
    }
}

*/