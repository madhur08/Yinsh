#include "Search.h"
#include <float.h>
#define winningScore 3
Search::Search(int boardSize, int seqSize, std::string move)
    : state(1, boardSize, seqSize, move)
{
    playerID = 2;
    w1 = 1;
    w2 = 1;
    w3 = 1000;
    w4 = 10;
    w5 = 1;
    w6 = 1;
    w7 = 1;
    w8 = -1;
}
Search::Search(int boardSize, int seqSize)
    : state(1, boardSize, seqSize)
{
    playerID = 1;
    w1 = 1;
    w2 = 1;
    w3 = 1000;
    w4 = 10;
    w5 = 1;
    w6 = 1;
    w7 = 1;
    w8 = -1;
}
void Search::playMove(std::string str)
{
    state.playMove(str);
}
std::string Search::nextMove(int depth)
{
    //int n = state.possibleMoves();
    //int nextstate;
    double alpha = -1 * DBL_MAX, beta = DBL_MAX, value = -1 * DBL_MAX;
    double fvalue;
    nextstate = state;
    //for(int i=0; i < n; i++)
    branches = 0;
    while (true)
    {
        State newState(nextstate);
        if (!nextstate.executeNext(newState))
            break;
        if (playerID == 1 && newState.getScore1() == winningScore && (newState.getScore2() - nextstate.getScore2()) == 0) //
            return newState.getMove();
        else if (playerID == 2 && newState.getScore2() == winningScore && (newState.getScore1() - nextstate.getScore1()) == 0)
            return newState.getMove();
        fvalue = funcMin(alpha, beta, depth - 1, newState);
        branches++;
        if (fvalue > value)
        {
            state = newState;
            value = fvalue;
        }
        if (value > alpha)
            alpha = value;

        // if( alpha > beta )
        //     break;
    }
    //state=nextState;
    return state.getMove();
}
double Search::funcMin(double alpha, double beta, int depth, State &current)
{
    if (depth == 0 || current.getScore1() == winningScore || current.getScore2() == winningScore) // >= karna hai kya??
        return evalFunction(current);
    else
    {
        //int n = state.possibleMoves();
        double value = DBL_MAX;
        double fvalue;
        //for(int i=0; i < n; i++)
        while (true)
        {
            State newState(current);
            if (!current.executeNext(newState))
                break;
            fvalue = funcMax(alpha, beta, depth - 1, newState);
            if (fvalue < value)
                value = fvalue;

            if (value < beta)
                beta = value;

            if (alpha >= beta)
                break;
        }
        return value;
    }
}

double Search::funcMax(double alpha, double beta, int depth, State &current)
{
    if (depth == 0 || current.getScore1() == winningScore || current.getScore2() == winningScore)
        return evalFunction(current);
    else
    {
        //int n = state.possibleMoves();
        double value = -1 * DBL_MAX;
        double fvalue;
        //for(int i=0; i < n; i++)
        while (true)
        {
            State newState(current);
            if (!current.executeNext(newState))
                break;
            if (playerID == 1 && newState.getScore1() == winningScore && (newState.getScore2() - current.getScore2()) == 0) //
                return funcMin(alpha, beta, depth - 1, newState);
            else if (playerID == 2 && newState.getScore2() == winningScore && (newState.getScore1() - current.getScore1()) == 0)
                return funcMin(alpha, beta, depth - 1, newState);
            fvalue = funcMin(alpha, beta, depth - 1, newState);
            if (fvalue > value)
                value = fvalue;

            if (value > alpha)
                alpha = value;

            if (alpha >= beta)
                break;
        }
        return value;
    }
}

double Search::evalFunction(State &current)
{
    if (playerID == 1) //our player is 1
    {

        if (current.getScore1() == 3)
        {
            return 10000000 - w1 * current.getScore2(); //can multiply current.getScore2 with some factor
        }
        if (current.getScore2() == 3)
        {
            return -10000000 + w2 * current.getScore1(); //can multiply current.getScore1 with some factor
        }

        else
        {

            return w3 * (current.getScore1() - current.getScore2()) + w4 * (current.getMarkers1() - current.getMarkers2()) + w6 * (current.sumMarkersInControl1() - current.sumMarkersInControl2()) + w8 * 0.004 * (sumDistFromCenter(1, current) - 0 * sumDistFromCenter(2, current)); // + w5*(current.validMoves1()-current.validMoves2()) + w7*(current.allMoves1()-current.allMoves2())
        }
    }
    else //our player is 2
    {

        if (current.getScore2() == 3)
        {
            return 10000000 - w1 * current.getScore1(); //can multiply current.getScore1 with some factor
        }
        if (current.getScore1() == 3)
        {
            return -10000000 + w2 * current.getScore2();
        }

        else
        {

            return -1 * (w3 * (current.getScore1() - current.getScore2()) + w4 * (current.getMarkers1() - current.getMarkers2()) + w6 * (current.sumMarkersInControl1() - current.sumMarkersInControl2()) + w8 * 0.004 * (0 * sumDistFromCenter(1, current) - sumDistFromCenter(2, current))); // + w5*(current.validMoves1()-current.validMoves2())+ w7*(current.allMoves1()-current.allMoves2()) );
        }
    }
}

double Search::sumDistFromCenter(int player, State &current)
{
    if (player == 1)
    {
        int n = current.numRings1();
        double distance = 0;
        for (int i = 0; i < n; ++i)
        {
            std::pair<int, int> am = current.getRing1(i);
            distance += am.first;
        }
        return distance;
    }
    else
    {
        int n = current.numRings2();
        double distance = 0;
        for (int i = 0; i < n; ++i)
        {
            std::pair<int, int> am = current.getRing2(i);
            distance += am.first;
        }
        return distance;
    }
}