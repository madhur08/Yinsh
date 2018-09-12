#ifndef SEARCH_H
#define SEARCH_H
#include "Game.h"
class Search
{
  private:
    Game game;

  public:
    Search(int playerID, int boardSize, int timeLimit);
};

#endif