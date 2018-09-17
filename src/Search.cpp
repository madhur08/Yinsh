#include "Search.h"
#include <float.h>
#define winningScore 3
Search::Search(int boardSize, int timeLimit, std::string move)
    : state(1, boardSize, move)
{
    playerID = 2;
    w1=1;
    w2=1;
    w3=1000;
    w4=1;
    w5=1;
    w6=1;
}
Search::Search(int boardSize, int timeLimit)
    : state(1, boardSize)
{
    playerID = 1;
    w1=1;
    w2=1;
    w3=1000;
    w4=1;
    w5=1;
    w6=1;
}
void Search::playMove(std::string str){
    state.playMove(str);
}
std::string Search::nextMove (int depth)
{
    //int n = state.possibleMoves();   
    //int nextstate;
    double alpha=-1*DBL_MAX, beta=DBL_MAX, value=-1*DBL_MAX;
    double fvalue;
    State nextstate(state);
    //for(int i=0; i < n; i++)
    branches=0;
    while(true)
    {
        State newState(nextstate);
        if (!nextstate.executeNext(newState))
            break;
        fvalue=funcMin( alpha, beta, depth-1, newState );
        branches++;
        if( fvalue > value )
        {
            state = newState;
            value = fvalue;
        }
        if ( value > alpha )
            alpha = value;

        // if( alpha > beta )
        //     break;
    }
    //state=nextState;
    return state.getMove();
}
double Search::funcMin( double alpha, double beta, int depth, State& current )
{
    if(depth == 0 || current.getScore1() == winningScore || current.getScore2() == winningScore )   // >= karna hai kya??
        return evalFunction( current );
    else
    {
        //int n = state.possibleMoves();   
        double value=DBL_MAX;
        double fvalue;
        //for(int i=0; i < n; i++)
        while(true)
        {
            State newState(current);
            if (!current.executeNext(newState))
                break;
            fvalue=funcMax( alpha, beta, depth-1, newState );
            if( fvalue < value )
                value = fvalue;

            if ( value < beta )
                beta = value;

            if( alpha >= beta )
                 break;
        }
        return value;
    }
}

double Search::funcMax( double alpha, double beta, int depth, State& current )
{
    if(depth == 0 || current.getScore1() == winningScore || current.getScore2() == winningScore )
        return evalFunction( current );
    else
    {
        //int n = state.possibleMoves();   
        double value=-1*DBL_MAX;
        double fvalue;
        //for(int i=0; i < n; i++)
        while(true)
        {
            State newState(current);
            if (!current.executeNext(newState))
                break;
            fvalue=funcMin( alpha, beta, depth-1, newState );
            if( fvalue > value )
                value = fvalue;
        
            if ( value > alpha )
                alpha = value;

            if( alpha >= beta )
                 break;
        }
        return value;
    }
}

double Search::evalFunction(State& current)
{
    if ( playerID == 1)  //our player is 1
    {
        
        if (current.getScore1() == 3)
        {
            return DBL_MAX-w1*current.getScore2();   //can multiply current.getScore2 with some factor 
        }
        if (current.getScore2() == 3)
        {
            return -1*DBL_MAX + w2*current.getScore1();   //can multiply current.getScore1 with some factor    
        }

        else
        {
            
            return w3*(current.getScore1()-current.getScore2()) + w4*(current.getMarkers1()-current.getMarkers2())  + w6*(current.sumMarkersInControl1()-current.sumMarkersInControl2()) ;  // + w5*(current.validMoves1()-current.validMoves2()) + w7*(current.allMoves1()-current.allMoves2())
        }
    }
    else                //our player is 2 
    {
        
        if (current.getScore2() == 3)
        {
            return DBL_MAX-w1*current.getScore1();   //can multiply current.getScore1 with some factor 
        }
        if (current.getScore1() == 3)
        {
            return -1*DBL_MAX + w2*current.getScore2();
        }

        else
        {
            
            return -1*( w3*(current.getScore1()-current.getScore2()) + w4*(current.getMarkers1()-current.getMarkers2())  + w6*(current.sumMarkersInControl1()-current.sumMarkersInControl2()) ); // + w5*(current.validMoves1()-current.validMoves2())+ w7*(current.allMoves1()-current.allMoves2()) );
        }
    }
}