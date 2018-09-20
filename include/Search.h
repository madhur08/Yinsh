#ifndef SEARCH_H
#define SEARCH_H
#include "State.h"
class Search
{
private:
  State state, nextstate;
  int playerID;
  double  w1,w2,w3,w4,w5,w6,w7,w8;
public:
  int branches;
  Search(int, std::string);
  Search(int);
  std::string nextMove (int depth );
  double funcMin( double alpha, double beta, int depth, State& current );
  double  sumDistFromCenter (int player, State& current);
  double funcMax( double alpha, double beta, int depth, State& current );
  double evalFunction(State& current);
  void playMove(std::string);
};

#endif