#include <iostream>
#include <Search.h>
#include <chrono>
#include<fstream>
using namespace std;

// Sample C++ Code
int main()
{
    int playerID, boardSize, timeLimit, depth = 4;
    string move;
    // Get input from server about game specifications
    cin >> playerID >> boardSize >> timeLimit;
    cin.ignore();
    if (playerID == 2)
    {
        // Get other player's move
        getline(cin, move);
        Search search(boardSize, move);
        while (true)
        {
            cout << search.nextMove(depth);
            getline(cin, move);
            search.playMove(move);
        }
    }
    else if (playerID == 1)
    {
        Search search(boardSize);
        cout << "P 0 0" << std::endl;
        while (true)
        {
            getline(cin, move);
            search.playMove(move);
            cout << search.nextMove(depth);
        }
    }
    return 0;
}