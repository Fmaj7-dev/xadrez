#include <iostream>
#include <fstream>

using namespace std;

// strings
const string author("FMaj7");
const string name("xadrez");

// commands
const string stop("stop");
const string quit("quit");
const string exitt("exit");
const string uci("uci");
const string isready("isready");

// output strings
const string uciok("uciok");
const string readyok("readyok");


int main(int argc, char** argv)
{
  cout << name << " " << author << endl;
  while( true )
  {
    // read input
    string read;
    getline(cin, read);
    
    // write input to log
    {
      ofstream outfile;

      outfile.open("/Users/enrique/projects/xadrez/log.txt", ios_base::app);
      outfile << read << endl; 
      outfile.close();
      outfile.flush();
    }

    // process commands
    if ( read == uci )
    {
      cout << "id name " << name << endl;
      cout << "id author " << author << endl;
      cout << "option name Debug Log File type string default" << endl;
      cout << uciok << endl;
    }
    else if ( read == isready )
    {
      cout << readyok << endl;
    }
    else if ( read == "go movetime 6000")
    {
      cout << "bestmove d2d4 ponder d7d5" << endl;
    }

    else if ( read == exitt ||
              read == stop ||
              read == quit )
      return 0;
  }
}
