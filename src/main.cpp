#include <iostream>
#include <Search.h>
#include <chrono>
#include <fstream>
using namespace std;

// Sample C++ Code
int main()
{
    int playerID, boardSize, timeLimit, seqSize, depth = 4;
    string move;
    // Get input from server about game specifications
    ifstream fin;
    fin.open("input.txt");
    auto &f = cin;
    f >> playerID >> boardSize >> timeLimit >> seqSize;
    //std::cerr<<playerID<<boardSize<<timeLimit<<seqSize<<std::endl;
    f.ignore();
    if (playerID == 2)
    {
        // Get other player's move
        getline(f, move);
        Search search(boardSize, seqSize, move);
        while (true)
        {
            // if(search.timeToGetAggressive())
            //     depth = 5;
            cout << search.nextMove(depth);
            getline(f, move);
            search.playMove(move);
        }
    }
    else if (playerID == 1)
    {
        Search search(boardSize, seqSize);
        cout << "P 0 0" << std::endl;
        while (true)
        {
            getline(f, move);
            search.playMove(move);
            // if(search.timeToGetAggressive())
            //     depth = 5;
            cout << search.nextMove(depth);
        }
    }
    return 0;
}