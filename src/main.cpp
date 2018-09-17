#include <iostream>
#include <Search.h>
using namespace std;

// Sample C++ Code
int main(int argc, char **argv)
{

    int playerID, boardSize, timeLimit;
    string move;
    // Get input from server about game specifications
    cin >> playerID >> boardSize >> timeLimit;

    if (playerID == 2)
    {
        // Get other player's move
        getline(cin,move);
        Search search(boardSize, timeLimit, move);
        while (true)
        {
            cout<<search.nextMove();
            getline(cin,move);
            search.playMove(move);
        }
    }
    else if (playerID == 1)
    {
        Search search(boardSize, timeLimit);
        cout<<"P 0 0"<<std::endl;
        while (true)
        {
            getline(cin,move);
            search.playMove(move);
            cout<<search.nextMove();
        }
    }
    return 0;
}