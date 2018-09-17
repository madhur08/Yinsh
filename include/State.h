#ifndef STATE_H
#define STATE_H
#include <vector>
using std::vector;

enum moveType{AddRing, MoveRing, RemoveRow, RemoveRing};

class State
{
private:
  struct move
  {
    int direction;
    int index;
    std::pair<int,int> startPos;
    std::pair<int,int> finalPos;
    move(int,int,startPos,finalPos);
    move(int,startPos,finalPos);
    move(int,startPos,finalPos);
  };
  int playerID, boardSize;

  int numMarkers1, numMarkers2;
  int score1,score2;
  vector<vector<bool>> markerPlayer1;
  vector<vector<bool>> markerPlayer2;
  vector<vector<bool>> ringPlayer1;
  vector<vector<bool>> ringPlayer2;
  int reachability1, reachability2;
  vector<std::pair<int, int>> ringPos1;
  vector<std::pair<int, int>> ringPos2;

  vector<move> moves;
  vector<move> possibleRows;
  vector<std::pair<move,move>> possibleRowMoves;
  int lastRowMove;
  std::string playedMove;
  int currentMove;
  std::pair<int, int> move0;


  int getScore1();
  int getScore2();
  int State::numMarkers1();
  int State::numMarkers2();
  void appendToCurrentMove(std::string);
  void intializeMatrix(vector<vector<bool>>, int);
  std::pair<int, int> moveToCoordinate(int, int);
  std::pair<int, int> coordinateToMove(int, int);
  int possibleMoves();
  int addPossibleMoves(std::pair<int, int>,int);
  std::pair<int, int> maxIncrement(std::pair<int, int>, int);
  int State::addPossibleMovesDir(int, int, int, int,int);
  bool executeNext(State&);
  void addRing(std::pair<int,int>,&State);
  void moveRing(move,&State);
  vector<std::pair<int,int>> flipMarkers(std::pair<int,int>,std::pair<int,int>, State&);
  void storeMove(State&,moveType, std::pair<int,int>, std::pair<int,int>);
  bool flip(int,int);
  vector<move> checkRow(std::pair<int,int>,int);
  void makeAllPossibleCombinations(State&);
  void removeRowAndRing(const move&);
  void makeRows(State&,vector<std::pair<int, int>>,std::pair<int, int>,std::pair<int, int>);
  bool checkIntersection(const &move, const &move);
  void playMove(std::string);
public:
  State(int, int);
  State(int, int,std::string);
};

#endif