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
    move();
  };
  int *hits;
  int playerID, boardSize;
  int moveCount;
  int numMarkers1, numMarkers2;
  int score1, score2;
  int reachability1, reachability2;
  vector<std::pair<int, int> > ringPos1;
  vector<std::pair<int, int> > ringPos2;
  vector<move> moves;
  vector<move> possibleRows;
  vector<std::pair<move, move> > possibleRowMoves;
  vector<std::pair<move, move> > possibleRowMovesBeforeMove;
  unsigned int lastRowMove;
  unsigned int currentMove;
  unsigned int lastRowMoveBeforeMove;
  bool removeBeforeMove;
  move move0;
  std::string playedMove;
  vector<vector<bool> > markerPlayer1;
  vector<vector<bool> > markerPlayer2;
  vector<vector<bool> > ringPlayer1;
  vector<vector<bool> > ringPlayer2;

  std::pair<int, int> moveToCoordinate(int, int);
  std::pair<int, int> coordinateToMove(int, int);
  void appendToCurrentMove(std::string);
  void initializeMatrix(vector<vector<bool> > &, int);
  int possibleMoves(State&,bool pushMove = 1);
  int addPossibleMoves(State&, std::pair<int, int>, int, bool pushMove);
  std::pair<int, int> maxIncrement(std::pair<int, int>, int);
  int addPossibleMovesDir(State&, int, int, int, int, int, bool pushMove);
  void addRing(std::pair<int, int>, State &);
  void moveRing(move, State &);
  vector<std::pair<int, int> > flipMarkers(std::pair<int, int>, std::pair<int, int>, State &, bool Flip = 1);
  void storeMove(State &, moveType, std::pair<int, int>, std::pair<int, int>);
  bool flip(int, int, bool);
  vector<move> checkRow(std::pair<int, int>, int, State&);
  void makeAllPossibleCombinations(vector<std::pair<move,move> > &, State&);
  void removeRowAndRing(const move &, State&);
  void removeAndStore(std::pair<move,move> &, State&);
  void makeRows(State &, vector<std::pair<int, int>>, std::pair<int, int>, std::pair<int, int>);
  bool checkIntersection(const move&, const move&);

public:
  std::string getMove();
  std::pair<int,int> getRing1(int);
  std::pair<int,int> getRing2(int);
  int numRings1();
  int numRings2();
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