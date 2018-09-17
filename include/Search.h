#ifndef SEARCH_H
#define SEARCH_H
#include "State.h"
class Search
{
private:
  State state;
  int playerID;
  double  w1,w2,w3,w4,w5,w6;

public:
  Search(int, int, std::string);
  Search(int, int);
  next_move ( State current ,int depth );
  funcMin( double alpha, double beta, int depth, State current );
  funcMax( double alpha, double beta, int depth, State current );
  evalFunction(State current,int player);
};

#endif