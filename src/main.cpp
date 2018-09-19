#include <iostream>
#include <Search.h>
#include <chrono>
#include<fstream>
using namespace std;

// Sample C++ Code
int main()
{
    fstream fin;
    //auto &fin = cin;
    fin.open("/Users/madhur/Documents/Sem 7/COL333/Assignments/Assignment 2/Yinsh/Python/input.txt",fstream::in);
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
        int count = 0;
        while (true)
        {
            getline(fin, move);
            search.playMove(move);
            // std::cerr<<"Move number = "<<++count<<std::endl;
            // auto m1 = search.state.markerPlayer1;
            // auto m2 = search.state.markerPlayer2;
            // auto r1 = search.state.ringPlayer1;
            // auto r2 = search.state.ringPlayer2;
            // std::cerr<<"Orange markers: ";
            // for(unsigned int i=0;i<m1.size();++i)
            //     for(unsigned int j=0; j<m1[i].size();++j)
            //     if(m1[i][j] == 1){
            //         auto point = search.state.coordinateToMove(i-5,j-5);
            //         std::cerr<<"("<<point.first<<","<<point.second<<") ";
            //     }
            // std::cerr<<std::endl;
            // std::cerr<<"Blue markers: ";
            // for(unsigned int i=0;i<m2.size();++i)
            //     for(unsigned int j=0; j<m2[i].size();++j)
            //     if(m2[i][j] == 1){
            //         auto point = search.state.coordinateToMove(i-5,j-5);
            //         std::cerr<<"("<<point.first<<","<<point.second<<") ";
            //     }
            // std::cerr<<"\n\n";
            // std::cerr<<"Orange rings: ";
            // for(unsigned int i=0;i<r1.size();++i)
            //     for(unsigned int j=0; j<r1[i].size();++j)
            //         if(r1[i][j] == 1){
            //             auto point = search.state.coordinateToMove(i-5,j-5);
            //             std::cerr<<"("<<point.first<<","<<point.second<<") ";
            //         }
            // std::cerr<<"\n";
            // std::cerr<<"Blue rings: ";
            // for(unsigned int i=0;i<r2.size();++i)
            //     for(unsigned int j=0; j<r2[i].size();++j)
            //     if(r2[i][j] == 1){
            //         auto point = search.state.coordinateToMove(i-5,j-5);
            //         std::cerr<<"("<<point.first<<","<<point.second<<") ";
            //     }
            // std::cerr<<"\n\n";
            // auto start = chrono::high_resolution_clock::now();
            cout << search.nextMove(a);
            // m1 = search.state.markerPlayer1;
            // m2 = search.state.markerPlayer2;
            // r1 = search.state.ringPlayer1;
            // r2 = search.state.ringPlayer2;
            // std::cerr<<"Orange markers: ";
            // for(unsigned int i=0;i<m1.size();++i)
            //     for(unsigned int j=0; j<m1[i].size();++j)
            //     if(m1[i][j] == 1){
            //         auto point = search.state.coordinateToMove(i-5,j-5);
            //         std::cerr<<"("<<point.first<<","<<point.second<<") ";
            //     }
            // std::cerr<<std::endl;
            // std::cerr<<"Blue markers: ";
            // for(unsigned int i=0;i<m2.size();++i)
            //     for(unsigned int j=0; j<m2[i].size();++j)
            //     if(m2[i][j] == 1){
            //         auto point = search.state.coordinateToMove(i-5,j-5);
            //         std::cerr<<"("<<point.first<<","<<point.second<<") ";
            //     }
            // std::cerr<<"\n\n";
            // std::cerr<<"Orange rings: ";
            // for(unsigned int i=0;i<r1.size();++i)
            //     for(unsigned int j=0; j<r1[i].size();++j)
            //         if(r1[i][j] == 1){
            //             auto point = search.state.coordinateToMove(i-5,j-5);
            //             std::cerr<<"("<<point.first<<","<<point.second<<") ";
            //         }
            // std::cerr<<"\n";
            // std::cerr<<"Blue rings: ";
            // for(unsigned int i=0;i<r2.size();++i)
            //     for(unsigned int j=0; j<r2[i].size();++j)
            //     if(r2[i][j] == 1){
            //         auto point = search.state.coordinateToMove(i-5,j-5);
            //         std::cerr<<"("<<point.first<<","<<point.second<<") ";
            //     }
            // std::cerr<<"\n\n";
            // // auto end = chrono::high_resolution_clock::now();
            // // auto gap = chrono::duration_cast<chrono::duration<double> >(end - start).count();
            // // //int maxmoves = 35;
            // // //double timeTaken = maxmoves * gap;
            // // std::cerr << "gap " << gap<<" depth = "<<a<<std::endl;
            // // if (search.branches < 16)
            // //     a = 4;
        }
    }
    return 0;
}