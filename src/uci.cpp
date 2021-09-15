#include "uci.h"
#include "log.h"

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

// example
// uci
// isready
// position fen rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1

// commands
const string stop("stop");
const string quit("quit");
const string exitt("exit");
const string uci("uci");
const string isready("isready");
const string position("position");
const string fen("fen");

// output strings
const string uciok("uciok");
const string readyok("readyok");

// debugging commands
const string init("init");

UCI::UCI()
{
    etlog("------------- NEW EXECUTION -----------------");
}

bool BothAreSpaces(char lhs, char rhs) 
{ 
    return (lhs == rhs) && (lhs == ' '); 
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

        std::string::iterator new_end = std::unique(read.begin(), read.end(), BothAreSpaces);
        read.erase(new_end, read.end());

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
        else if (command[0] == position && command[1] == fen)
        {
            std::string fen_str = read.substr(13);
            engine_.loadFen(fen_str);
        }
        else if ( command[0] == "go")
        {
            std::string movement = "bestmove " + engine_.findBestMove(10) + " ponder g1f3";

            cout << movement << endl;
            
            etlogwrite( movement );
        }
        else if ( command[0] == init )
        {
            //engine_.loadFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
            engine_.loadFen("3ppp2/3pkp2/4pp2/8/1B6/K7/8/8 b - - 0 1");
            
            cout<<"done"<<endl;
        }

        else if ( read == exitt ||
                  read == stop ||
                  read == quit )
        return 0;
    }
}