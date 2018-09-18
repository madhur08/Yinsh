#include <iostream>
#include <Search.h>
#include <chrono>
#include<fstream>
using namespace std;

// Sample C++ Code
int main()
{
    fstream fin;
    fin.open("/home/ishu/power7thsem/ai/assignment2/Yinsh/Python/input.txt",fstream::in);
    bool ncm = fin.is_open();

    int playerID, boardSize, timeLimit;
    string move;
    // Get input from server about game specifications
    fin >> playerID >> boardSize >> timeLimit;
    fin.ignore();
    if (playerID == 2)
    {
        // Get other player's move
        getline(fin, move);
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
            // if (search.branches < 16)
            //     a = 4;
            getline(fin, move);
            fin.ignore();
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
            getline(fin, move);
            search.playMove(move);
            // auto start = chrono::high_resolution_clock::now();
            cout << search.nextMove(a);
            // auto end = chrono::high_resolution_clock::now();
            // auto gap = chrono::duration_cast<chrono::duration<double> >(end - start).count();
            // //int maxmoves = 35;
            // //double timeTaken = maxmoves * gap;
            // std::cerr << "gap " << gap<<" depth = "<<a<<std::endl;
            // if (search.branches < 16)
            //     a = 4;
        }
    }
    return 0;
}