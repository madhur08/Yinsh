#include "Search.h"

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
State Search::nextMove ( State current ,int depth)
{
    //int n = state.possibleMoves();   
    //int nextstate;
    double alpha=-1*DBL_MAX, beta=DBL_MAX, value=-1*DBL_MAX;
    double fvalue;
    State nextstate;
    //for(int i=0; i < n; i++)
    while(true)
    {
        State newState(current);
        if (!current.executeNext(newState))
            break;
        fvalue=funcMin( alpha, beta, depth-1, newState )
        if( fvalue > value )
        {
            nextstate = newState;
            value = fvalue;
        }
        if ( value > alpha )
            alpha = value;

        // if( alpha > beta )
        //     break;
    }
    return nextstate;
}
double Search::funcMin( double alpha, double beta, int depth, State current )
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
            fvalue=funcMax( alpha, beta, depth-1, newState )
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

double Search::funcMax( double alpha, double beta, int depth, state current )
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
            fvalue=funcMin( alpha, beta, depth-1, newState )
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

double Search::evalFunction(State current,int player)
{
    if ( player == 1)  //our player is 1
    {
        double w1=1,w2=1;
        if (current.getScore1 == 3)
        {
            return DBL_MAX-w1*current.getScore2;   //can multiply current.getScore2 with some factor 
        }
        if (current.getScore2 == 3)
        {
            return -1*DBL_MAX + w2*current.getScore1;   //can multiply current.getScore1 with some factor    
        }

        else
        {
            double w3=1000,w4=1,w5=1,w6=1;
            return w3*(getScore1-getScore2) + w4*(getMarkers1-getMarkers2) + w5*(validMoves1-validMoves2) + w6*(sumMarkersInControl1-sumMarkersInControl2) + w7*(allMoves1-allMoves2);
        }
    }
    else                //our player is 2 
    {
        double w1=1,w2=1;
        if (current.getScore2 == 3)
        {
            return DBL_MAX-w1*current.getScore1;   //can multiply current.getScore1 with some factor 
        }
        if (current.getScore1 == 3)
        {
            return -1*DBL_MAX + w2*current.getScore2;
        }

        else
        {
            double w3=1000,w4=1,w5=1,w6=1;
            return -1*( w3*(getScore1-getScore2) + w4*(getMarkers1-getMarkers2) + w5*(validMoves1-validMoves2) + w6*(sumMarkersInControl1-sumMarkersInControl2) + w7*(allMoves1-allMoves2) );
        }
    }
}