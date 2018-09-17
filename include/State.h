#ifndef STATE_H
#define STATE_H
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
using std::vector;

enum moveType
{
  AddRing,
  MoveRing,
  RemoveRow,
  RemoveRing
};

class State
{
private:
  struct move
  {
    int direction;
    int index;
    std::pair<int, int> startPos;
    std::pair<int, int> finalPos;
    move(int, int, std::pair<int, int>, std::pair<int, int>);
    move(int, std::pair<int, int>, std::pair<int, int>);
    move(std::pair<int, int>, std::pair<int, int>);
    move(int);
  };
  unsigned int playerID, boardSize;

  unsigned int numMarkers1, numMarkers2;
  unsigned int score1, score2;
  vector<vector<bool> > markerPlayer1;
  vector<vector<bool> > markerPlayer2;
  vector<vector<bool> > ringPlayer1;
  vector<vector<bool> > ringPlayer2;
  unsigned int reachability1, reachability2;
  vector<std::pair<int, int> > ringPos1;
  vector<std::pair<int, int> > ringPos2;

  vector<move> moves;
  vector<move> possibleRows;
  vector<std::pair<move, move> > possibleRowMoves;
  unsigned int lastRowMove;
  std::string playedMove;
  unsigned int currentMove;
  move move0;

  void appendToCurrentMove(std::string);
  void initializeMatrix(vector< vector<bool> > &, int);
  std::pair<int, int> moveToCoordinate(int, int);
  std::pair<int, int> coordinateToMove(int, int);
  int possibleMoves();
  int addPossibleMoves(std::pair<int, int>, int);
  std::pair<int, int> maxIncrement(std::pair<int, int>, int);
  int addPossibleMovesDir(int, int, int, int, int);
  void addRing(std::pair<int, int>, State &);
  void moveRing(move, State &);
  vector<std::pair<int, int> > flipMarkers(std::pair<int, int>, std::pair<int, int>, State &);
  void storeMove(State &, moveType, std::pair<int, int>, std::pair<int, int>);
  bool flip(int, int);
  vector<move> checkRow(std::pair<int, int>, int);
  void makeAllPossibleCombinations();
  void removeRowAndRing(const move &);
  void makeRows(State &, vector<std::pair<int, int>>, std::pair<int, int>, std::pair<int, int>);
  bool checkIntersection(const move&, const move&);

public:
  std::string getMove();
  int getScore1();
  int getScore2();
  int getMarkers1();
  int getMarkers2();
  int sumMarkersInControl1();
  int sumMarkersInControl2();
  void playMove(std::string);
  bool executeNext(State &);
  State() = default;
  State(int, int);
  State(int, int, std::string);
  State(const State &);
};

#endif