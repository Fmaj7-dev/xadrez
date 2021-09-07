#ifndef LOG_H
#define LOG_H

#include <fstream>

static void etlog(std::string s)
{
    std::ofstream outfile;

    outfile.open("/Users/enrique/projects/xadrez/log.txt", std::ios_base::app);
    outfile << s << std::endl; 
    outfile.close();
    outfile.flush();
}

#endif