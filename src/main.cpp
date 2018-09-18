#include <iostream>
#include <Search.h>
using namespace std;

// Sample C++ Code
int main()
{

    int playerID, boardSize, timeLimit;
    string move;
    // Get input from server about game specifications
    cin >> playerID >> boardSize >> timeLimit;
    if (playerID == 2)
    {
        // Get other player's move
        cin.ignore();
        getline(cin, move);
        Search search(boardSize, timeLimit, move);
        int a = 2;
        while (true)
        {
            auto start = chrono::high_resolution_clock::now();
            cout << search.nextMove(a);
            auto end = chrono::high_resolution_clock::now();
            auto gap = chrono::duration_cast<chrono::duration<double> >(end - start).count();
            //int maxmoves = 35;
            //double timeTaken = maxmoves * gap;
            std::cerr << "gap " << gap<<" depth = "<<a<<std::endl;
            if (search.branches < 16)
                a = 4;
            cin.ignore();
            getline(cin, move);
            search.playMove(move);
        }
    }
    else if (playerID == 1)
    {
        int a = 3;
        Search search(boardSize, timeLimit);
        cout << "P 0 0" << std::endl;
        while (true)
        {
            cin.ignore();
            getline(cin, move);
            search.playMove(move);
            auto start = chrono::high_resolution_clock::now();
            cerr << search.nextMove(a);
            auto end = chrono::high_resolution_clock::now();
            auto gap = chrono::duration_cast<chrono::duration<double> >(end - start).count();
            //int maxmoves = 35;
            //double timeTaken = maxmoves * gap;
            std::cerr << "gap " << gap<<" depth = "<<a<<std::endl;
            if (search.branches < 16)
                a = 4;
        }
    }
    return 0;
}