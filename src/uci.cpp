#include "uci.h"
#include "log.h"

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

// commands
const string stop("stop");
const string quit("quit");
const string exitt("exit");
const string uci("uci");
const string isready("isready");

// output strings
const string uciok("uciok");
const string readyok("readyok");

UCI::UCI()
{
    etlog("------------- NEW EXECUTION -----------------");
}

int UCI::run()
{
    cout << engine_.getName() << " " << engine_.getAuthor() << endl;
    while( true )
    {
        // read input
        string read;
        getline(cin, read);

        // write input to log
        etlogread(read);

        // split line into an array
        stringstream ssin(read);
        std::vector< std::string > command;

        while ( ssin.good() )
        {
            std::string word;
            ssin >> word;
            command.push_back(word);
        }

        // process commands
        if ( command[0] == uci )
        {
            std::string name_string = "id name " + engine_.getName();
            std::string author_string = "id author " + engine_.getAuthor();

            cout << name_string << endl;
            cout << author_string << endl;
            cout << uciok << endl;

            etlogwrite( name_string );
            etlogwrite( author_string );
            etlogwrite( uciok );
        }
        else if ( command[0] == isready )
        {
            cout << readyok << endl;

            etlogwrite( readyok );
        }
        else if ( command[0] == "go")
        {
            std::string movement = "bestmove " + engine_.findBestMove(10) + " ponder g1f3";

            cout << movement << endl;
            
            etlogwrite( movement );
        }

        else if ( read == exitt ||
                read == stop ||
                read == quit )
        return 0;
    }
}