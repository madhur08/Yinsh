#include <iostream>
#include <fstream>
using namespace std;

// Sample C++ Code 
int main(int argc, char** argv) {
    
    int player_id, board_size, time_limit,seqNum;
    string move;
    ifstream f;
    f.open("input.txt");

    // Get input from server about game specifications
    cin >> player_id >> board_size >> time_limit >> seqNum;
    std::cerr<<player_id<<board_size<<time_limit<<seqNum<<std::endl;
    cin.ignore();
    if(player_id == 2) {
        // Get other player's move
        getline(cin,move); 
       
        getline(f,move);
        while(true) {
            cout<<move<<endl;
            getline(cin,move); 
            getline(f,move);
        }
    }   
    else if(player_id == 1) {
        while(true) {
            getline(f,move);
            cout<<move<<endl; 
            getline(cin,move); 
        }
    }
    f.close();
    return 0;
}