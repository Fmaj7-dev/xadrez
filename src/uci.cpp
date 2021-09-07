#include "uci.h"
#include "log.h"

#include <iostream>

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
        etlog(read);

        // process commands
        if ( read == uci )
        {
            cout << "id name " << engine_.getName() << endl;
            cout << "id author " << engine_.getAuthor() << endl;
            cout << "option name Debug Log File type string default" << endl;
            cout << uciok << endl;
        }
        else if ( read == isready )
        {
            cout << readyok << endl;
        }
        else if ( read == "go movetime 6000")
        {
            cout << "bestmove e7e5 ponder g1f3" << endl;
        }

        else if ( read == exitt ||
                read == stop ||
                read == quit )
        return 0;
    }
}