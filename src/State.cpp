#include "State.h"
#include <cmath>
State::move::move(int dir, int ind, std::pair<int, int> pos1, std::pair<int, int> pos2)
{
    startPos = pos1;
    finalPos = pos2;
    direction = dir;
    index = ind;
}
State::move::move(int ind, std::pair<int, int> pos1, std::pair<int, int> pos2)
{
    startPos = pos1;
    finalPos = pos2;
    direction = -1;
    index = ind;
}
State::move::move(std::pair<int, int> pos1, std::pair<int, int> pos2)
{
    startPos = pos1;
    finalPos = pos2;
    index = -1;
    direction = -1;
}
State::move::move(int dir)
{
    direction = dir;
    index = -1;
    finalPos = std::pair<int, int>(0, -1);
    startPos = std::pair<int, int>(0, -1);
}
State::move::move()
{
    direction = -1;
    index = -1;
    finalPos = std::pair<int, int>(0, -1);
    startPos = std::pair<int, int>(0, -1);
}
State::State(const State &state) : move0(-1)
{
    playerID = state.playerID;
    boardSize = state.boardSize;
    numMarkers1 = state.numMarkers1;
    numMarkers2 = state.numMarkers2;
    score1 = state.score1;
    score2 = state.score2;
    markerPlayer1 = state.markerPlayer1;
    markerPlayer2 = state.markerPlayer2;
    ringPlayer1 = state.ringPlayer1;
    ringPlayer2 = state.ringPlayer2;
    ringPos1 = state.ringPos1;
    ringPos2 = state.ringPos2;
    removeBeforeMove = 0;
    lastRowMoveBeforeMove = 0;
    lastRowMove = currentMove = reachability1 = reachability2 = 0;
}
State::State(int playerID, int boardSize) : move0(-1)
{
    this->playerID = 3 - playerID;
    this->boardSize = boardSize;
    score1 = score2 = numMarkers2 = numMarkers1 = reachability1 = reachability2 = 0;
    lastRowMove = currentMove = 0;
    initializeMatrix(markerPlayer1, boardSize);
    initializeMatrix(markerPlayer2, boardSize);
    initializeMatrix(ringPlayer1, boardSize);
    initializeMatrix(ringPlayer2, boardSize);
    removeBeforeMove = 0;
    lastRowMoveBeforeMove = 0;
    playMove(std::string("P 0 0\n"));
}
State::State(int playerID, int boardSize, std::string move) : move0(-1)
{
    this->playerID = 3 - playerID;
    this->boardSize = boardSize;
    score1 = score2 = numMarkers2 = numMarkers1 = reachability1 = reachability2 = 0;
    lastRowMove = currentMove = 0;
    initializeMatrix(markerPlayer1, boardSize);
    initializeMatrix(markerPlayer2, boardSize);
    initializeMatrix(ringPlayer1, boardSize);
    initializeMatrix(ringPlayer2, boardSize);
    removeBeforeMove = 0;
    lastRowMoveBeforeMove = 0;
    playMove(move);
}
std::string State::getMove()
{
    playedMove.pop_back();
    playedMove.push_back('\n');
    return playedMove;
}
int State::getScore1()
{
    return score1;
}
int State::getScore2()
{
    return score2;
}
int State::getMarkers1()
{
    return numMarkers1;
}
int State::getMarkers2()
{
    return numMarkers2;
}
int State::numRings1()
{
    return ringPos1.size();
}
int State::numRings2()
{
    return ringPos2.size();
}
std::pair<int, int> State::getRing1(int i)
{
    return coordinateToMove(ringPos1[i].first, ringPos1[i].second);
}
std::pair<int, int> State::getRing2(int i)
{
    return coordinateToMove(ringPos2[i].first, ringPos2[i].second);
}
int State::sumMarkersInControl1()
{
    if (ringPos1.size() < 5 || ringPos2.size() < 5)
        return 0;
    if (reachability1 != 0)
        return reachability1;
    if (playerID == 1)
    {
        playerID = 3 - playerID;
        possibleMoves(0);
        playerID = 3 - playerID;
        return reachability1;
    }
    possibleMoves();
    return reachability1;
}
int State::sumMarkersInControl2()
{
    if (ringPos1.size() < 5 || ringPos2.size() < 5)
        return 0;
    if (reachability2 != 0)
        return reachability2;
    if (playerID == 2)
    {
        playerID = 3 - playerID;
        possibleMoves(0);
        playerID = 3 - playerID;
        return reachability1;
    }
    possibleMoves();
    return reachability2;
}
void State::playMove(std::string str)
{
    std::pair<int, int> coor1, coor2;
    std::stringstream ss(str);
    std::string item;
    reachability1 = 0;
    reachability2 = 0;
    removeBeforeMove = 0;
    lastRowMove = currentMove = 0;
    moves.clear();
    possibleRows.clear();
    possibleRowMoves.clear();
    possibleRowMovesBeforeMove.clear();
    move0 = move(-1);
    std::cerr << "input = " << str << std::endl;
    while (ss >> item)
    {
        if (item == "S")
        {
            ss >> item;
            coor1.first = std::stoi(item);
            ss >> item;
            coor1.second = std::stoi(item);
            coor1 = moveToCoordinate(coor1.first, coor1.second);
            ss >> item;
            if (item != "M")
                std::cerr << "Incorrect Move\n";
            ss >> item;
            coor2.first = std::stoi(item);
            ss >> item;
            coor2.second = std::stoi(item);
            coor2 = moveToCoordinate(coor2.first, coor2.second);
            int index = 0;
            if (playerID == 1)
            {
                for (unsigned int i = 0; i != ringPos2.size(); ++i)
                    if (ringPos2[i] == coor1)
                    {
                        index = i;
                        break;
                    }
            }
            else
            {
                for (unsigned int i = 0; i < ringPos1.size(); ++i)
                    if (ringPos1[i] == coor1)
                    {
                        index = i;
                        break;
                    }
            }
            moves.clear();
            currentMove = 0;
            move0 = move(-1);
            moveRing(move(index, coor1, coor2), *this);
            playerID = 3 - playerID;
            makeRows(*this, flipMarkers(coor1, coor2, *this), coor1, coor2);
            if (possibleRows.empty())
                continue;
            removeBeforeMove = true;
            lastRowMoveBeforeMove = 0;
            makeAllPossibleCombinations(possibleRowMovesBeforeMove);
        }
        else if (item == "P")
        {
            ss >> item;
            coor1.first = std::stoi(item);
            ss >> item;
            coor1.second = std::stoi(item);
            coor1 = moveToCoordinate(coor1.first, coor1.second);
            addRing(coor1, *this);
        }
        else if (item == "RS")
        {
            ss >> item;
            coor1.first = std::stoi(item);
            ss >> item;
            coor1.second = std::stoi(item);
            coor1 = moveToCoordinate(coor1.first, coor1.second);
            ss >> item;
            if (item != "RE")
                std::cerr << "Incorrect Move\n";
            ss >> item;
            coor2.first = std::stoi(item);
            ss >> item;
            coor2.second = std::stoi(item);
            coor2 = moveToCoordinate(coor2.first, coor2.second);
            ss >> item;
            move row(coor1, coor2);
            if (item != "X")
                std::cerr << "Incorrect Move\n";
            ss >> item;
            coor1.first = std::stoi(item);
            ss >> item;
            coor1.second = std::stoi(item);
            int index = 0;
            if (playerID == 1)
            {
                for (unsigned int i = 0; i != ringPos2.size(); ++i)
                    if (ringPos2[i] == coor1)
                    {
                        index = i;
                        break;
                    }
            }
            else
            {
                for (unsigned int i = 0; i != ringPos1.size(); ++i)
                    if (ringPos1[i] == coor1)
                    {
                        index = i;
                        break;
                    }
            }
            row.index = index;
            playerID = 3 - playerID;
            removeRowAndRing(row,*this);
            playerID = 3 - playerID;
        }
        else
            std::cerr << "Incorrect Move " << item << std::endl;
    }
}
void State::initializeMatrix(vector<vector<bool>> &matrix, int size)
{
    matrix.resize(2 * size + 1);
    for (auto &v : matrix)
        v.resize(2 * size + 1);
}

std::pair<int, int> State::moveToCoordinate(int ring, int position)
{
    std::pair<int, int> coordinate(ring, ring);
    if (ring == 0 && position == 0)
        return coordinate;
    switch (int(position / ring))
    {
    case 5:
        coordinate.first += (position % ring - ring);
        break;
    case 4:
        coordinate.first += (position % ring - 2 * ring);
        coordinate.second += (position % ring - ring);
        break;
    case 3:
        coordinate.first -= 2 * ring;
        coordinate.second += (position % ring - 2 * ring);
        break;
    case 2:
        coordinate.first -= (ring + position % ring);
        coordinate.second -= (2 * ring);
        break;
    case 1:
        coordinate.first -= position % ring;
        coordinate.second -= (ring + position % ring);
        break;
    case 0:
        coordinate.second -= position % ring;
        break;
    }
    return coordinate;
}

std::pair<int, int> State::coordinateToMove(int x, int y)
{
    std::pair<int, int> move;
    if (x >= 0 && y >= 0)
    {
        if (x >= y)
        {
            move.first = x;
            move.second = x - y;
        }
        else
        {
            move.first = y;
            move.second = x + 5 * y;
        }
    }
    else if (x <= 0 && y >= 0)
    {
        move.first = y - x;
        move.second = 5 * y - 4 * x;
    }
    else if (x >= 0 && y <= 0)
    {
        move.first = x - y;
        move.second = x - 2 * y;
    }
    else
    {
        if (x <= y)
        {
            move.first = -x;
            move.second = y - 4 * x;
        }
        else
        {
            move.first = -y;
            move.second = -x - 2 * y;
        }
    }
    return move;
}
int State::possibleMoves(bool pushMove)
{
    int movesNum = 0;
    if (ringPos1.size() < 5 || ringPos2.size() < 5)
        movesNum = ((6 + boardSize * 6) * boardSize * 0.5 - 5 - ringPos1.size() - ringPos2.size());
    else if (playerID == 2)
        for (unsigned int i = 0; i != ringPos1.size(); ++i)
            movesNum += addPossibleMoves(ringPos1[i], i, pushMove);
    else if (playerID == 1)
        for (unsigned int i = 0; i != ringPos2.size(); ++i)
            movesNum += addPossibleMoves(ringPos2[i], i, pushMove);
    return movesNum;
}
int State::addPossibleMoves(std::pair<int, int> position, int index, bool pushMove = 1)
{
    int x = position.first, y = position.second;
    int moveCount = 0;
    for (int i = 1; i <= 3; ++i)
    {
        std::pair<int, int> maxInc = maxIncrement(position, i);
        moveCount += addPossibleMovesDir(x, y, maxInc.first, i, index, pushMove);
        moveCount += addPossibleMovesDir(x, y, maxInc.second, i, index, pushMove);
    }
    return moveCount;
}
int State::addPossibleMovesDir(int x, int y, int max, int dir, int index, bool pushMove = 1)
{
    int s = max < 0 ? -1 : 1;
    max = max < 0 ? -max : max;
    int a = dir == 3 ? 1 : dir == 2 ? 0 : 1;
    int b = dir == 3 ? 1 : dir == 2 ? 1 : 0;
    int moveCount = 0;
    // std::cerr<<"x = "<<x<<"y = "<<y<<std::endl;
    // std::cerr<<"x_ex = "<<(x + max * s * a)<<"y_ex = "<<(y + max * s * b)<<std::endl;
    // auto out = coordinateToMove((x + max * s * a), (y + max * s * b));
    // std::cerr<<"r = "<<out.first<<"p = "<<out.second<<"\n\n";
    for (int p = 1; p <= max; ++p)
    {
        if(std::abs(x + p * s * a) > 5 || std::abs(y + p * s * b) > 5){
            std::cerr<<"x = "<<x<<" y = "<<y<<std::endl;
            std::cerr<<"x_ex = "<<(x + p * s * a)<<" y_ex = "<<(y + p * s * b)<<std::endl;
            auto out = coordinateToMove((x + p * s * a), (y + p * s * b));
            std::cerr<<"r = "<<out.first<<" p = "<<out.second<<"\n\n";
        }
        if (ringPlayer1[boardSize + x + p * s * a][boardSize + y + p * s * b] == 1 || ringPlayer2[boardSize + x + p * s * a][boardSize + y + p * s * b] == 1)
            break;
        else if (markerPlayer1[boardSize + x + p * s * a][boardSize + y + p * s * b] == 1 || markerPlayer2[boardSize + x + p * s * a][boardSize + y + p * s * b] == 1)
        {
            int inc1 = 0, inc2 = 0;
            while ((p <= max) && (markerPlayer1[boardSize + x + p * s * a][boardSize + y + p * s * b] == 1 || markerPlayer2[boardSize + x + p * s * a][boardSize + y + p * s * b] == 1))
            {
                ++p;
                if (playerID == 1)
                    inc2++;
                else
                    inc1++;
            }
            if (p <= max && ringPlayer1[boardSize + x + p * s * a][boardSize + y + p * s * b] == 0 && ringPlayer2[boardSize + x + p * s * a][boardSize + y + p * s * b] == 0)
            {
                if(pushMove)
                    moves.push_back(move(index, std::pair<int, int>(x, y), std::pair<int, int>(x + p * s * a, y + p * s * b)));
                moveCount++;
                reachability1 += inc1;
                reachability2 += inc2;
            }
            break;
        }
        if (pushMove)
            moves.push_back(move(index, std::pair<int, int>(x, y), std::pair<int, int>(x + p * s * a, y + p * s * b)));
        moveCount++;
    }
    return moveCount;
}
/* direction = 1 => NE
   direction = 2 => NW
   direction = 3 => N */
std::pair<int, int> State::maxIncrement(std::pair<int, int> position, int direction)
{
    std::pair<int, int> maxInc;
    float x = position.first, y = position.second;
    if (direction == 1)
    {
        double a = (y - 2 * x), b = std::pow(4 * 4.6 * 4.6 - 3 * y * y, 0.5);
        maxInc.first = (a - b) * 0.5;
        maxInc.second = (a + b) * 0.5;
    }
    else if (direction == 2)
    {
        double a = (x - 2 * y), b = std::pow(4 * 4.6 * 4.6 - 3 * x * x, 0.5);
        maxInc.first = (a - b) * 0.5;
        maxInc.second = (a + b) * 0.5;
    }
    else
    {
        double a = -(x + y), b = std::pow(6 * x * y + 4 * 4.6 * 4.6 - 3 * x * x - 3 * y * y, 0.5);
        maxInc.first = (a - b) * 0.5;
        maxInc.second = (a + b) * 0.5;
    }
    return maxInc;
}
void State::appendToCurrentMove(std::string str)
{
    playedMove += str;
}
void State::storeMove(State &nextState, moveType type, std::pair<int, int> coord1, std::pair<int, int> coord2 = std::pair<int, int>(0, 0))
{
    switch (type)
    {
    case AddRing:
        nextState.appendToCurrentMove("P " + std::to_string(coord1.first) + ' ' + std::to_string(coord1.second) + ' ');
        break;
    case MoveRing:
        nextState.appendToCurrentMove("S " + std::to_string(coord1.first) + ' ' + std::to_string(coord1.second) + " M " + std::to_string(coord2.first) + ' ' + std::to_string(coord2.second) + ' ');
        break;
    case RemoveRow:
        nextState.appendToCurrentMove("RS " + std::to_string(coord1.first) + ' ' + std::to_string(coord1.second) + " RE " + std::to_string(coord2.first) + ' ' + std::to_string(coord2.second) + ' ');
        break;
    case RemoveRing:
        nextState.appendToCurrentMove("X " + std::to_string(coord1.first) + ' ' + std::to_string(coord1.second) + ' ');
        break;
    }
}
void State::removeAndStore(std::pair<move, move> &a, State &nextState)
{
    removeRowAndRing(a.first,nextState);
    storeMove(nextState, RemoveRow, coordinateToMove(a.first.startPos.first, a.first.startPos.second), coordinateToMove(a.first.finalPos.first, a.first.finalPos.second));
    if (playerID == 1)
        storeMove(nextState, RemoveRing, coordinateToMove(ringPos2[a.first.index].first, ringPos2[a.first.index].second));
    else
        storeMove(nextState, RemoveRing, coordinateToMove(ringPos1[a.first.index].first, ringPos1[a.first.index].second));
    if (a.second.direction > 0)
    {
        if(a.second.index>a.first.index)
            --a.second.index;
        removeRowAndRing(a.second,nextState);
        storeMove(nextState, RemoveRow, coordinateToMove(a.second.startPos.first, a.second.startPos.second), coordinateToMove(a.second.finalPos.first, a.second.finalPos.second));
        if (playerID == 1)
            storeMove(nextState, RemoveRing, coordinateToMove(ringPos2[a.second.index].first, ringPos2[a.second.index].second));
        else
            storeMove(nextState, RemoveRing, coordinateToMove(ringPos1[a.second.index].first, ringPos1[a.second.index].second));
    }
}
bool State::executeNext(State &nextState)
{
    bool moveExecuted = false;
    if (ringPos1.size() < 5 || ringPos2.size() < 5)
    {
        if (move0.startPos.first <= boardSize)
        {
            for (int p = move0.startPos.second + 1; p < 6 * move0.startPos.first || p == 0; ++p)
            {
                if (!(move0.startPos.first == boardSize) || !((p % boardSize) == 0))
                {
                    std::pair<int, int> coor = moveToCoordinate(move0.startPos.first, p);
                    if (ringPlayer1[boardSize + coor.first][boardSize + coor.second] == 0 && ringPlayer2[boardSize + coor.first][boardSize + coor.second] == 0)
                    {
                        //std::cerr<<"ring = "<<move0.startPos.first<<" pos = "<<p<<std::endl;
                        addRing(coor, nextState);
                        move0 = move(std::pair<int, int>(move0.startPos.first, p), std::pair<int, int>(0, 0));
                        // if (std::abs(coor.first) > 5 || std::abs(coor.second) > 5)
                        // {
                        //     std::cerr << "ring = " << move0.startPos.first << " pos = " << p << std::endl;
                        //     std::cerr << "x = " << coor.first << " y = " << coor.second << std::endl;
                        //     auto out = coordinateToMove(coor.first, coor.first);
                        //     std::cerr << "ring = " << out.first << " pos = " << out.second << std::endl
                        //               << std::endl;
                        // }
                        storeMove(nextState, AddRing, std::pair<int, int>(move0.startPos.first, p));
                        return true;
                        ;
                    }
                }
            }
        }
        for (int r = move0.startPos.first + 1; r <= boardSize; ++r)
        {
            for (int p = 0; p < r * 6; ++p)
            {
                if (!(r == boardSize) || !((p % boardSize) == 0))
                {
                    std::pair<int, int> coor = moveToCoordinate(r, p);
                    if (ringPlayer1[boardSize + coor.first][boardSize + coor.second] == 0 && ringPlayer2[boardSize + coor.first][boardSize + coor.second] == 0)
                    {
                        //std::cerr<<"ring = "<<r<<" pos = "<<p<<std::endl<<std::endl;
                        addRing(coor, nextState);
                        move0 = move(std::pair<int, int>(r, p), std::pair<int, int>(0, 0));
                        // if (std::abs(coor.first) > 5 || std::abs(coor.second) > 5)
                        // {
                        //     std::cerr << "ring = " << r << " pos = " << p << std::endl;
                        //     std::cerr << "x = " << coor.first << " y = " << coor.second << std::endl;
                        //     auto out = coordinateToMove(coor.first, coor.first);
                        //     std::cerr << "ring = " << out.first << " pos = " << out.second << std::endl
                        //               << std::endl;
                        // }
                        storeMove(nextState, AddRing, std::pair<int, int>(r, p));
                        return true;
                    }
                }
            }
        }
    }
    else if ((int)lastRowMove < (int)(possibleRowMoves.size() - 1))
    {
        if (removeBeforeMove)
        {
            removeAndStore(possibleRowMovesBeforeMove[lastRowMoveBeforeMove - 1],nextState);
            if ((score1 == 3 && playerID == 2) || (score2 == 3 && playerID == 1))
                return true;
        }
        storeMove(nextState, MoveRing, coordinateToMove(move0.startPos.first, move0.startPos.second), coordinateToMove(move0.finalPos.first, move0.finalPos.second));
        moveRing(move0, nextState);
        flipMarkers(move0.startPos, move0.finalPos, nextState);
        removeAndStore(possibleRowMoves[++lastRowMove],nextState);
        nextState.playerID = 3 - playerID;
    }
    else
    {
        lastRowMove = 0;
        possibleRows.clear();
        possibleRowMoves.clear();
        if (removeBeforeMove)
        {
            if (currentMove == moves.size() || currentMove = 0)
            {
                if (lastRowMoveBeforeMove < possibleRowMovesBeforeMove.size())
                {
                    removeAndStore(possibleRowMovesBeforeMove[lastRowMoveBeforeMove],nextState);
                    if ((score1 == 3 && playerID == 2) || (score2 == 3 && playerID == 1))
                        return true;
                    moves.clear();
                    currentMove = 0;
                    possibleMoves();
                    ++lastRowMoveBeforeMove;
                }
            }
            else
            {
                removeAndStore(possibleRowMovesBeforeMove[lastRowMoveBeforeMove - 1],nextState);
                if ((score1 == 3 && playerID == 2) || (score2 == 3 && playerID == 1))
                    return true;
            }
        }
        if (moves.empty())
            possibleMoves();
        if (currentMove != moves.size())
        {
            storeMove(nextState, MoveRing, coordinateToMove(moves[currentMove].startPos.first, moves[currentMove].startPos.second), coordinateToMove(moves[currentMove].finalPos.first, moves[currentMove].finalPos.second));
            moveRing(moves[currentMove], nextState);
            move0 = moves[currentMove];
            makeRows(nextState, flipMarkers(moves[currentMove].startPos, moves[currentMove].finalPos, nextState), moves[currentMove].startPos, moves[currentMove].finalPos);
            ++currentMove;
            if (possibleRows.empty())
            {
                nextState.playerID = 3 - playerID;
                return true;
            }
            
            makeAllPossibleCombinations(possibleRowMoves);
            removeAndStore(possibleRowMoves[0],nextState);
            moveExecuted = true;
            nextState.playerID = 3 - playerID;
        }
    }
    return moveExecuted;
}
void State::addRing(std::pair<int, int> coor, State &nextState)
{
    if (playerID == 1)
    {
        nextState.ringPos2.push_back(coor);
        nextState.ringPlayer2[boardSize + coor.first][boardSize + coor.second] = 1;
        nextState.playerID = 2;
    }
    else
    {
        nextState.ringPos1.push_back(coor);
        nextState.ringPlayer1[boardSize + coor.first][boardSize + coor.second] = 1;
        nextState.playerID = 1;
    }
    //std::cerr<<"x = "<<coor.first<<" y = "<<coor.second<<std::endl;
    //std::cerr<<"ring = "<<move0.startPos.first<<" pos = "<<move0.startPos.second<<std::endl<<std::endl;
}
void State::moveRing(move pos, State &nextState)
{
    if (playerID == 1)
    {
        nextState.ringPlayer2[boardSize + pos.startPos.first][boardSize + pos.startPos.second] = 0;
        nextState.ringPlayer2[boardSize + pos.finalPos.first][boardSize + pos.finalPos.second] = 1;
        nextState.markerPlayer2[boardSize + pos.startPos.first][boardSize + pos.startPos.second] = 1;
        nextState.numMarkers2++;
        nextState.ringPos2[pos.index] = pos.finalPos;
    }
    else
    {
        nextState.ringPlayer1[boardSize + pos.startPos.first][boardSize + pos.startPos.second] = 0;
        nextState.ringPlayer1[boardSize + pos.finalPos.first][boardSize + pos.finalPos.second] = 1;
        nextState.markerPlayer1[boardSize + pos.startPos.first][boardSize + pos.startPos.second] = 1;
        nextState.numMarkers1++;
        nextState.ringPos1[pos.index] = pos.finalPos;
    }
}
bool State::checkIntersection(const move &a, const move &b)
{
    if (a.direction == 1 && b.direction == 1)
        if (a.startPos.second == b.startPos.second)
            if (a.startPos.first <= b.finalPos.first)
                return true;
    if (a.direction == 2 && b.direction == 2)
        if (a.startPos.first == b.startPos.first)
            if (a.startPos.second <= b.finalPos.second)
                return true;
    if (a.direction == 3 && b.direction == 3)
        if ((a.startPos.second - a.startPos.first) == (b.startPos.second - b.startPos.first))
            if (a.startPos.first <= b.finalPos.first)
                return true;
    if (a.direction == 1 && b.direction == 2)
        if (a.startPos.first <= b.startPos.first && a.startPos.first >= b.startPos.first && a.startPos.second <= b.finalPos.second && a.startPos.second >= b.startPos.second)
            return true;
    if (a.direction == 2 && b.direction == 3)
        if (b.startPos.first <= a.startPos.first && b.finalPos.first >= a.startPos.first)
        {
            int y3 = b.startPos.second + (a.startPos.first - b.startPos.first);
            if (y3 <= a.finalPos.second && y3 >= a.startPos.second)
                return true;
        }
    if (a.direction == 1 && b.direction == 3)
        if (b.startPos.second <= a.startPos.second && b.finalPos.second >= a.startPos.second)
        {
            int x3 = b.startPos.first + (a.startPos.second - b.startPos.second);
            if (x3 <= a.finalPos.first && x3 >= a.startPos.first)
                return true;
        }
    return false;
}
void State::removeRowAndRing(const move &a, State &nextState)
{
    if (a.direction == 1)
    {
        for (int i = a.startPos.first; i <= a.finalPos.first; ++i)
        {
            if (playerID == 1)
            {
                nextState.markerPlayer2[boardSize + i][boardSize + a.startPos.second] = 0;
                --nextState.numMarkers2;
            }
            else
            {
                nextState.markerPlayer1[boardSize + i][boardSize + a.startPos.second] = 0;
                --nextState.numMarkers1;
            }
        }
    }
    if (a.direction == 2)
    {
        for (int i = a.startPos.second; i <= a.finalPos.second; ++i)
        {
            if (playerID == 1)
            {
                nextState.markerPlayer2[boardSize + a.startPos.first][boardSize + i] = 0;
                --nextState.numMarkers2;
            }
            else
            {
                nextState.markerPlayer1[boardSize + a.startPos.first][boardSize + i] = 0;
                --nextState.numMarkers1;
            }
        }
    }
    if (a.direction == 3)
    {
        for (int i = 0; i <= (a.finalPos.first - a.startPos.first); ++i)
        {
            if (playerID == 1)
            {
                nextState.markerPlayer2[boardSize + a.startPos.first + i][boardSize + a.startPos.second + i] = 0;
                --nextState.numMarkers2;
            }
            else
            {
                nextState.markerPlayer1[boardSize + a.startPos.first + i][boardSize + a.startPos.second + i] = 0;
                --nextState.numMarkers1;
            }
        }
    }
    if (playerID == 1)
    {
        nextState.ringPlayer2[boardSize + ringPos2[a.index].first][boardSize + ringPos2[a.index].second] = 0;
        nextState.ringPos2.erase(nextState.ringPos2.begin() + a.index);
        ++nextState.score2;
    }
    else
    {
        nextState.ringPlayer1[boardSize + ringPos1[a.index].first][boardSize + ringPos1[a.index].second] = 0;
        nextState.ringPos1.erase(nextState.ringPos1.begin() + a.index);
        ++nextState.score1;
    }
}
void State::makeAllPossibleCombinations(vector<std::pair<move, move>> &rowMoves)
{
    if ((playerID == 1 && ringPos2.size() == 3) || (playerID == 2 && ringPos1.size() == 3))
    {
        possibleRows[0].index = 2;
        rowMoves.push_back(std::pair<move, move>(possibleRows[0], move(-1)));
    }
    else if ((playerID == 1 && ringPos2.size() > 3) || (playerID == 2 && ringPos1.size() > 3))
    {
        int maxRings;
        if (playerID == 1)
            maxRings = ringPos2.size();
        else
            maxRings = ringPos1.size();
        for (unsigned int i = 0; i != possibleRows.size(); ++i)
            for (unsigned int j = i + 1; j != possibleRows.size(); ++j)
            {
                move a = possibleRows[i];
                move b = possibleRows[j];
                if (!checkIntersection(a, b) && !checkIntersection(b, a))
                {
                    for (int r1 = maxRings - 1; r1 > 0; --r1)
                        for (int r2 = r1 - 1; r2 >= 0; --r2)
                        {
                            a.index = r1;
                            b.index = r2;
                        }
                    if ((playerID == 1 && ringPos2.size() == 4) || (playerID == 2 && ringPos1.size() == 4))
                    {
                        possibleRows.clear();
                        rowMoves.push_back(std::pair<move, move>(a, b));
                        return;
                    }
                    rowMoves.push_back(std::pair<move, move>(a, b));
                }
            }
        if (rowMoves.size() == 0)
            for (unsigned int i = 0; i != possibleRows.size(); ++i)
                for (int r = maxRings-1; r >= 0; --r)
                {
                    possibleRows[i].index = r;
                    rowMoves.push_back(std::pair<move, move>(possibleRows[i], move(-1)));
                }
    }
}
bool State::flip(int a, int b)
{
    if (markerPlayer1[boardSize + a][boardSize + b] == 1)
    {
        markerPlayer2[boardSize + a][boardSize + b] = 1;
        markerPlayer1[boardSize + a][boardSize + b] = 0;
        return true;
    }
    if (markerPlayer2[boardSize + a][boardSize + b] == 1)
    {
        markerPlayer1[boardSize + a][boardSize + b] = 1;
        markerPlayer2[boardSize + a][boardSize + b] = 0;
        return true;
    }
    return false;
}
void State::makeRows(State &nextState, vector<std::pair<int, int>> flipped, std::pair<int, int> start, std::pair<int, int> end)
{
    bool check = true, once = false;
    if (start.first - start.second == end.first - end.second)
    {
        for (unsigned int f = 0; f != flipped.size(); ++f)
        {
            if (f > 0 && std::abs(flipped[f - 1].first - flipped[f].first) > 1 && !once)
                check = true;

            if (check)
            {
                vector<move> dir = checkRow(flipped[f], 3, nextState);
                possibleRows.insert(possibleRows.end(), dir.begin(), dir.end());
                check ^= 1;
                if (!dir.empty())
                    once = true;
                if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && once == true)
                    return;
            }
            vector<move> dir1 = checkRow(flipped[f], 2, nextState);
            possibleRows.insert(possibleRows.end(), dir1.begin(), dir1.end());
            if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && !dir1.empty())
                return;
            vector<move> dir2 = checkRow(flipped[f], 1, nextState);
            possibleRows.insert(possibleRows.end(), dir2.begin(), dir2.end());
            if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && !dir2.empty())
                return;
        }
    }
    else if (start.first == end.first)
    {
        for (unsigned int f = 0; f != flipped.size(); ++f)
        {
            if (f > 0 && std::abs(flipped[f - 1].second - flipped[f].second) > 1 && !once)
                check = true;
            if (check)
            {
                vector<move> dir = checkRow(flipped[f], 2,nextState);
                possibleRows.insert(possibleRows.end(), dir.begin(), dir.end());
                check ^= 1;
                if (!dir.empty())
                    once = true;
                if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && !dir.empty())
                    return;
            }
            vector<move> dir1 = checkRow(flipped[f], 3, nextState);
            possibleRows.insert(possibleRows.end(), dir1.begin(), dir1.end());
            if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && !dir1.empty())
                return;
            vector<move> dir2 = checkRow(flipped[f], 1,nextState);
            possibleRows.insert(possibleRows.end(), dir2.begin(), dir2.end());
            if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && !dir2.empty())
                return;
        }
    }
    else if (start.second == end.second)
    {
        for (unsigned int f = 0; f != flipped.size(); ++f)
        {
            if (f > 0 && std::abs(flipped[f - 1].first - flipped[f].first) > 1 && !once)
                check = true;
            if (check)
            {
                vector<move> dir = checkRow(flipped[f], 1,nextState);
                possibleRows.insert(possibleRows.end(), dir.begin(), dir.end());
                check ^= 1;
                if (!dir.empty())
                    once = true;
            }
            vector<move> dir1 = checkRow(flipped[f], 3,nextState);
            possibleRows.insert(possibleRows.end(), dir1.begin(), dir1.end());
            if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && !dir1.empty())
                return;
            vector<move> dir2 = checkRow(flipped[f], 2,nextState);
            possibleRows.insert(possibleRows.end(), dir2.begin(), dir2.end());
            if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && !dir1.empty())
                return;
        }
    }
}
vector<std::pair<int, int>> State::flipMarkers(std::pair<int, int> start, std::pair<int, int> end, State &nextState)
{
    nextState.flip(start.first, start.second);
    vector<std::pair<int, int>> flipped;
    if (start.first - start.second == end.first - end.second)
    {
        int startx = std::min(start.first, end.first);
        int endx = std::max(start.first, end.first);
        int starty = std::min(start.second, end.second);
        int endy = std::max(start.second, end.second);
        for (int i = startx, j = starty; i <= endx && j <= endy; ++i, ++j)
            if (nextState.flip(i, j) && ((playerID == 1 && nextState.markerPlayer2[boardSize + i][boardSize + j] == 1) || (playerID == 2 && nextState.markerPlayer1[boardSize + i][boardSize + j] == 1)))
                flipped.push_back(std::pair<int, int>(i, j));
    }
    else if (start.first == end.first)
    {
        int starty = std::min(start.second, end.second);
        int endy = std::max(start.second, end.second);
        for (int i = starty; i <= endy; ++i)
            if (nextState.flip(start.first, i) && ((playerID == 1 && nextState.markerPlayer2[boardSize + start.first][boardSize + i] == 1) || (playerID == 2 && nextState.markerPlayer1[boardSize + start.first][boardSize + i] == 1)))
                flipped.push_back(std::pair<int, int>(start.first, i));
    }
    else if (start.second == end.second)
    {
        int startx = std::min(start.first, end.first);
        int endx = std::max(start.first, end.first);
        for (int i = startx; i <= endx; ++i)
            if (nextState.flip(i, start.second) && ((playerID == 1 && nextState.markerPlayer2[boardSize + i][boardSize + start.second] == 1) || (playerID == 2 && nextState.markerPlayer1[boardSize + i][boardSize + start.second] == 1)))
                flipped.push_back(std::pair<int, int>(i, start.second));
    }
    nextState.flip(start.first, start.second);
    return flipped;
}
vector<State::move> State::checkRow(std::pair<int, int> coor, int dir, State& nextState)
{
    vector<move> moves;
    if (dir == 1)
    {
        int i, end, start;
        for (i = 0; ((playerID == 1) && (boardSize + coor.first + i < 11) && (nextState.markerPlayer2[boardSize + coor.first + i][boardSize + coor.second] == 1)) || ((playerID == 2) && (boardSize + coor.first + i < 11) && (nextState.markerPlayer1[boardSize + coor.first + i][boardSize + coor.second] == 1)); ++i);
        end = i - 1;

        for (i = -1; ((playerID == 1) && (boardSize + coor.first + i > 0) && (nextState.markerPlayer2[boardSize + coor.first + i][boardSize + coor.second] == 1)) || ((playerID == 2) && (boardSize + coor.first + i > 0) && (nextState.markerPlayer1[boardSize + coor.first + i][boardSize + coor.second] == 1)); --i);
        start = i + 1;
        if(end - start > 4) 
            for (i = start; i + 4 <= end; ++i)
                moves.push_back(move(1, -1, std::pair<int, int>(coor.first + i, coor.second), std::pair<int, int>(coor.first + i + 4, coor.second)));
    }
    if (dir == 2)
    {
        int i, end, start;
        for (i = 0; ((playerID == 1) && (boardSize + coor.second + i < 11) && (nextState.markerPlayer2[boardSize + coor.first][boardSize + coor.second + i] == 1)) || ((playerID == 2) && (boardSize + coor.second + i < 11) && (nextState.markerPlayer1[boardSize + coor.first][boardSize + coor.second + i] == 1)); ++i);
        end = i - 1;

        for (i = -1; ((playerID == 1) && (boardSize + coor.second + i > 0) && (nextState.markerPlayer2[boardSize + coor.first][boardSize + coor.second + i] == 1)) || ((playerID == 2) && (boardSize + coor.second + i > 0) && (nextState.markerPlayer1[boardSize + coor.first][boardSize + coor.second + i] == 1)); --i);
        start = i + 1;

        if(end - start > 4)
            for (i = start; i + 4 <= end; ++i)
                moves.push_back(move(2, -1, std::pair<int, int>(coor.first, coor.second + i), std::pair<int, int>(coor.first, coor.second + i + 4)));
    }
    if (dir == 3)
    {
        int i, end, start;
        for (i = 0; ((playerID == 1) && (boardSize + coor.second + i < 11) && (boardSize + coor.first + i < 11) && (nextState.markerPlayer2[boardSize + coor.first + i][boardSize + coor.second + i] == 1)) || ((playerID == 2) && (boardSize + coor.first + i < 11) && (boardSize + coor.second + i < 11) && (nextState.markerPlayer1[boardSize + coor.first + i][boardSize + coor.second + i] == 1)); ++i);
        end = i - 1;

        for (i = -1; ((playerID == 1) && (boardSize + coor.first + i > 0) && (boardSize + coor.second + i > 0) && (nextState.markerPlayer2[boardSize + coor.first + i][boardSize + coor.second + i] == 1)) || ((playerID == 2) && (boardSize + coor.first + i > 0) && (boardSize + coor.second + i > 0) && (nextState.markerPlayer1[boardSize + coor.first + i][boardSize + coor.second + i] == 1)); --i);
        start = i + 1;

        if(end - start > 4)
            for (i = start; i + 4 <= end; ++i)
                moves.push_back(move(3, -1, std::pair<int, int>(coor.first + i, coor.second + i), std::pair<int, int>(coor.first + i+4, coor.second + i + 4)));
    }
    return moves;
}