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
    index = ind;
}
State::move::move(std::pair<int, int> pos1, std::pair<int, int> pos2)
{
    startPos = pos1;
    finalPos = pos2;
}
State::move::move(int dir)
{
    direction = -1;
}
State::State(int playerID, int boardSize)
{
    this.playerID = 3 - playerID;
    this.boardSize = boardSize;
    intializeMatrix(markerPlayer1, boardSize);
    intializeMatrix(markerPlayer2, boardSize);
    intializeMatrix(ringPlayer1, boardSize);
    intializeMatrix(ringPlayer2, boardSize);
    playMove(std::string("P 0 0\n"));
}
State::State(int playerID, int boardSize, std::string move)
{
    this.playerID = playerID;
    this.boardSize = boardSize;
    intializeMatrix(markerPlayer1, boardSize);
    intializeMatrix(markerPlayer2, boardSize);
    intializeMatrix(ringPlayer1, boardSize);
    intializeMatrix(ringPlayer2, boardSize);
    playMove(move);
}
int State::getScore1(){
    return score1;
}
int State::getScore2(){
    return score2;
}
int State::numMarkers1(){
    return numMarkers1;
}
int State::numMarkers2(){
    return numMarkers2;
}
int State::getReachability1(){
    if(reachability1 != 0)
        return reachability1;
    if(playerID == 1){
        playerID = 3 - playerID;
        possibleMoves();
        playerID = 3 - playerID;
        return reachability1;
    }
    possibleMoves();
    return reachability1;
}
int State::getReachability1(){
    if(reachability2 != 0)
        return reachability2;
    if(playerID == 2){
        playerID = 3 - playerID;
        possibleMoves();
        playerID = 3 - playerID;
        return reachability1;
    }
    possibleMoves();
    return reachability2;
}
State::State(State &state)
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
}
void State::playMove(std::string str)
{
    previousMove = str;
    std::pair<int, int> coor1, coor2;
    std::stringstream ss(move);
    std::string item;
    reachability1 = 0;
    reachability2 = 0;
    while (ss >> item)
    {
        if (item == "S")
        {
            ss >> item;
            coor1.first = std::to_integer(item);
            ss >> item;
            coor1.second = std::to_integer(item);
            coor1 = moveToCoordinate(coor1.first, coor1.second);
            ss >> item;
            if (item != "M")
                std::cerr << "Incorrect Move\n";
            ss >> item;
            coor2.first = std::to_integer(item);
            ss >> item;
            coor2.second = std::to_integer(item);
            coor2 = moveToCoordinate(coor2.first, coor2.second);
            int index;
            if (playerID == 1)
            {
                for (int i = 0; i < ringPos2.size(); ++i)
                    if (ringPos2[i] == coor1)
                    {
                        index = i;
                        break;
                    }
            }
            else
            {
                for (int i = 0; i < ringPos1.size(); ++i)
                    if (ringPos1[i] == coor1)
                    {
                        index = i;
                        break;
                    }
            }
            moves.clear();
            currentMove = 0;
            move0 = move(-1);
            moveRing(move(index, coor1, coor2), this);
            playerID = 3 - playerID;
            makeRows(this, flipMarkers(pos.startPos, pos.finalPos, nextState), pos.startPos, pos.finalPos);
            if (possibleRows.empty())
                return;
            makeAllRowCombinations(nextState);
        }
        else if (item == "P")
        {
            ss >> item;
            coor1.first = std::to_integer(item);
            ss >> item;
            coor1.second = std::to_integer(item);
            coor1 = moveToCoordinate(coor1.first, coor1.second);
            addRing(coor1, this);
        }
        else if (item == "RS")
        {
            ss >> item;
            coor1.first = std::to_integer(item);
            ss >> item;
            coor1.second = std::to_integer(item);
            coor1 = moveToCoordinate(coor1.first, coor1.second);
            ss >> item;
            if (item != "RE")
                std::cerr << "Incorrect Move\n";
            ss >> item;
            coor2.first = std::to_integer(item);
            ss >> item;
            coor2.second = std::to_integer(item);
            coor2 = moveToCoordinate(coor2.first, coor2.second);
            ss >> item;
            move row(coor1, coor2);
            if (item != "X")
                std::cerr << "Incorrect Move\n";
            ss >> item;
            coor1.first = std::to_integer(item);
            ss >> item;
            coor1.second = std::to_integer(item);
            int index;
            if (playerID == 1)
            {
                for (int i = 0; i < ringPos2.size(); ++i)
                    if (ringPos2[i] == coor1)
                    {
                        index = i;
                        break;
                    }
            }
            else
            {
                for (int i = 0; i < ringPos1.size(); ++i)
                    if (ringPos1[i] == coor1)
                    {
                        index = i;
                        break;
                    }
            }
            row.index = index;
            removeRowAndRing(move);
        }
        else
            std::cerr << "Incorrect Move\n";
    }
}
void State::intializeMatrix(std::vector<std::vector<bool>> &matrix, int size)
{
    matrix.resize(2 * size + 1);
    for (auto &v : matrix)
        v.resize(2 * size + 1);
}

std::pair<int, int> State::moveToCoordinate(int ring, int position)
{
    std::pair<int, int> coordinate(ring, ring);
    switch (int(position / ring))
    {
    case 5:
        coordinate.first += (position % ring - ring);
        coordinate.second += 2 * ring;
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
    if (x > 0 && y > 0)
    {
        if (x > y)
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
    else if (x < 0 && y > 0)
    {
        move.first = y - x;
        move.second = 5 * y - 4 * x;
    }
    else if (x > 0 && y < 0)
    {
        move.first = x - y;
        move.second = x - 2 * y;
    }
    else
    {
        if (x < y)
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
int State::possibleMoves()
{
    int movesNum = 0;
    if (ringPos1.size() < 5 || ringPos2.size() < 5)
        movesNum = ((6 + boardSize * 6) * boardSize * 0.5 - 5 - ringPos1.size() - ringPos2.size());
    if (playerID = 2)
        for (int i = 0; i < 5; ++i)
            movesNum += addPossibleMoves(ringPos1[i], i);
    if (playerID = 1)
        for (int i = 0; i < 5; ++i)
            movesNum += addPossibleMoves(ringPos2[i], i);
    return movesNum;
}
int State::addPossibleMoves(std::pair<int, int> position, int index)
{
    int x = position.first, y = position.second;
    int moveCount = 0;
    for (int i = 1; i <= 3; ++i)
    {
        std::pair maxInc = maxIncrement(position, i);
        moveCount += addPossibleMovesDir(x, y, maxInc.first, i, index);
        moveCount += addPossibleMovesDir(x, y, maxInc.second, i, index);
    }
    return moveCount;
}
int State::addPossibleMovesDir(int x, int y, int max, int dir, int index)
{
    int s = max < 0 ? -1 : 1;
    max = max < 0 ? -max : max;
    int a = dir == 3 ? 1 : dir == 2 ? 0 : 1;
    int b = dir == 3 ? 1 : dir == 2 ? 1 : 0;
    int moveCount = 0;
    for (int p = 1; p <= max; ++p)
    {
        if (ringPlayer1[boardSize + x + p * s * a][boardSize + y + p * s * b] == 1 || ringPlayer2[boardSize + x + p * s * a][boardSize + y + p * s * b] == 1)
            break;
        else if (movePlayer1[boardSize + x + p * s * a][boardSize + y + p * s * b] == 1 || movePlayer2[boardSize + x + p * s * a][boardSize + y + p * s * b] == 1)
        {
            while ((p <= max) && (movePlayer1[boardSize + x + p * s * a][boardSize + y + p * s * b] == 1 || movePlayer2[boardSize + x + p * s * a][boardSize + y + p * s * b] == 1)){
                ++p;
                if(playerID == 1)
                    reachability2++;
                else   
                    reachability1++;
            }
            if (p <= max)
            {
                moves.push_back(move(index, std::pair<int, int>(x, y), std::pair<int, int>(x + p * s * a, y + p * s * b)));
                moveCount++;
                break;
            }
        }
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
    case RemoveRow:
        nextState.appendToCurrentMove("RS " + std::to_string(coord1.first) + ' ' + std::to_string(coord1.second) + " RE " + std::to_string(coord2.first) + ' ' + std::to_string(coord2.second) + ' ');
    case Remove Ring:
        nextState.appendToCurrentMove("X " + std::to_string(coord1.first) + ' ' + std::to_string(coord1.second) + ' ');
    }
}

bool State::executeNext(State &nextState)
{
    bool moveExecuted = false;
    if (ringPos1.size() < 5 || ringPos2.size() < 5)
    {
        for (int p = move0.startPos.second + 1; p < 6 * move0.startPos.first; ++p)
        {
            if (!move0.startPos.first == boardSize || !p % boardSize == 0)
            {
                std::pair<int, int> coor = moveToCoordinate(move0.startPos.first, p);
                if (ringPlayer1[boardSize + coor.first][boardSize + coor.second] == 0)
                {
                    moveExecuted = true;
                    addRing(coor, nextState);
                    storeMove(nextState, AddRing, std::pair<r, p>);
                }
            }
        }
        for (int r = move0.startPos.first + 1; r <= boardSize; ++r)
        {
            for (int p = 0; p < r * 6; ++p)
            {
                if (!r == boardSize || !p % boardSize == 0)
                {
                    std::pair<int, int> coor = moveToCoordinate(r, p);
                    if (ringPlayer1[boardSize + coor.first][boardSize + coor.second] == 0)
                    {
                        moveExecuted = true;
                        addRing(coor, nextState);
                        storeMove(nextState, AddRing, std::pair<r, p>);
                    }
                }
            }
        }
    }
    else if (lastRowMove < possibleRowMoves.size() - 1 && (moves.size() == 0 || currentMove == move.size()))
    {
        move a = possibleRowMoves[++lastRowMove].first;
        move b = possibleRowMoves[lastRowMove].second;
        nextState.removeRowAndRing(a);
        storeMove(nextState, RemoveRow, coordinateToMove(a.startPos.first, a.startPos.second), coordinateToMove(a.finalPos.first, a.finalPos.second));
        if (playerID == 1)
            storeMove(nextState, RemoveRing, coordinateToMove(ringPos2[a.index].first, ringPos2[a.index].second));
        else
            storeMove(nextState, RemoveRing, coordinateToMove(ringPos1[a.index].first, ringPos1[a.index].second));
        if (b.direction > 0)
        {
            nextState.removeRowAndRing(b);
            storeMove(nextState, RemoveRow, coordinateToMove(b.startPos.first, b.startPos.second), coordinateToMove(b.finalPos.first, b.finalPos.second));
            if (playerID == 1)
                storeMove(nextState, RemoveRing, coordinateToMove(ringPos2[b.index].first, ringPos2[b.index].second));
            else
                storeMove(nextState, RemoveRing, coordinateToMove(ringPos1[b.index].first, ringPos1[b.index].second));
        }
        if ((playerID == 1 && score2 == 3) || (playerID == 2 && score1 == 3))
            return true;
        possibleMoves();
        currentMove = 0;
        if (currentMove != moves.size())
        {
            storeMove(nextState, MoveRing, coordinateToMove(moves[currentMove].startPos.first, moves[currentMove].startPos.second), coordinateToMove(moves[currentMove].finalPos.first, moves[currentMove].finalPos.second));
            moveRing(moves[currentMove], nextState);
            makeRows(nextState, flipMarkers(pos.startPos, pos.finalPos, nextState), pos.startPos, pos.finalPos);
            if (possibleRows.empty())
                return true;
            makeAllRowCombinations(nextState);
            nextState.removeRowAndRing(possibleRowMoves[lastRowMove].first);
            move m = possibleRowMoves[lastRowMove].first;
            storeMove(nextState, RemoveRow, coordinateToMove(m.startPos.first, m.startPos.second), coordinateToMove(m.finalPos.first, m.finalPos.second));
            if (playerID == 1)
                storeMove(nextState, RemoveRing, coordinateToMove(ringPos2[m.index].first, ringPos2[m.index].second));
            else
                storeMove(nextState, RemoveRing, coordinateToMove(ringPos1[m.index].first, ringPos1[m.index].second));
            if (possibleRowMoves[lastRowMove].second.direction > 0)
            {
                m = possibleRowMoves[lastRowMove].second;
                nextState.removeRowAndRing(m);
                storeMove(nextState, RemoveRow, coordinateToMove(m.startPos.first, m.startPos.second), coordinateToMove(m.finalPos.first, m.finalPos.second));
                if (playerID == 1)
                    storeMove(nextState, RemoveRing, coordinateToMove(ringPos2[m.index].first, ringPos2[m.index].second));
                else
                    storeMove(nextState, RemoveRing, coordinateToMove(ringPos1[m.index].first, ringPos1[m.index].second));
            }
            currentMove++;
            moveExecuted = true;
            nextState.playerID = 3 - playerID;
        }
    }
    else if (lastRowMove < possibleRowMoves.size() - 1 && move0.startPos == std::pair<int, int>(0, 0) && move0.finalPos == std::pair<int, int>(0, 0) && currentMove < moves.size())
    {
        move a = possibleRowMoves[lastRowMove].first;
        move b = possibleRowMoves[lastRowMove].second;
        nextState.removeRowAndRing(a);
        storeMove(nextState, RemoveRow, coordinateToMove(a.startPos.first, a.startPos.second), coordinateToMove(a.finalPos.first, a.finalPos.second));
        if (playerID == 1)
            storeMove(nextState, RemoveRing, coordinateToMove(ringPos2[a.index].first, ringPos2[a.index].second));
        else
            storeMove(nextState, RemoveRing, coordinateToMove(ringPos1[a.index].first, ringPos1[a.index].second));
        if (b.direction > 0)
        {
            nextState.removeRowAndRing(b);
            storeMove(nextState, RemoveRow, coordinateToMove(b.startPos.first, b.startPos.second), coordinateToMove(b.finalPos.first, b.finalPos.second));
            if (playerID == 1)
                storeMove(nextState, RemoveRing, coordinateToMove(ringPos2[b.index].first, ringPos2[b.index].second));
            else
                storeMove(nextState, RemoveRing, coordinateToMove(ringPos1[b.index].first, ringPos1[b.index].second));
        }

        storeMove(nextState, MoveRing, coordinateToMove(moves[currentMove].startPos.first, moves[currentMove].startPos.second), coordinateToMove(moves[currentMove].finalPos.first, moves[currentMove].finalPos.second));
        moveRing(moves[currentMove], nextState);
        makeRows(nextState, flipMarkers(pos.startPos, pos.finalPos, nextState), pos.startPos, pos.finalPos);
        if (possibleRows.empty())
            return true;
        makeAllRowCombinations(nextState);
        nextState.removeRowAndRing(possibleRowMoves[lastRowMove].first);
        move m = possibleRowMoves[lastRowMove].first;
        storeMove(nextState, RemoveRow, coordinateToMove(m.startPos.first, m.startPos.second), coordinateToMove(m.finalPos.first, m.finalPos.second));
        if (playerID == 1)
            storeMove(nextState, RemoveRing, coordinateToMove(ringPos2[m.index].first, ringPos2[m.index].second));
        else
            storeMove(nextState, RemoveRing, coordinateToMove(ringPos1[m.index].first, ringPos1[m.index].second));
        if (possibleRowMoves[lastRowMove].second.direction > 0)
        {
            m = possibleRowMoves[lastRowMove].second;
            nextState.removeRowAndRing(m);
            storeMove(nextState, RemoveRow, coordinateToMove(m.startPos.first, m.startPos.second), coordinateToMove(m.finalPos.first, m.finalPos.second));
            if (playerID == 1)
                storeMove(nextState, RemoveRing, coordinateToMove(ringPos2[m.index].first, ringPos2[m.index].second));
            else
                storeMove(nextState, RemoveRing, coordinateToMove(ringPos1[m.index].first, ringPos1[m.index].second));
        }
        currentMove++;
        moveExecuted = true;
        nextState.playerID = 3 - playerID;
    }
    else if (lastRowMove < possibleRowMoves.size() - 1)
    {
        if (playerID == 1)
        {
            nextState.ringPlayer2[boardSize + move0.startPos.first][boardSize + move0.startPos.second] = 0;
            nextState.ringPlayer2[boardSize + move0.finalPos.first][boardSize + move0.finalPos.second] = 1;
            nextState.markerPlayer2[boardSize + move0.startPos.first][boardSize + move0.startPos.second] = 1;
            nextState.numMarkers2++;
            nextState.ringPos2[move0.index] = move0.finalPos;
            nextState.playerID = 2;
            flipMarkers(move0.startPos, move0.finalPos, nextState);
        }
        else
        {
            nextState.ringPlayer1[boardSize + move0.startPos.first][boardSize + move0.startPos.second] = 0;
            nextState.ringPlayer1[boardSize + move0.finalPos.first][boardSize + move0.finalPos.second] = 1;
            nextState.markerPlayer1[boardSize + move0..startPos.first][boardSize + move0.startPos.second] = 1;
            nextState.numMarkers1++;
            nextState.ringPos1[move0.index] = move0.finalPos;
            nextState.playerID = 1;
            flipMarkers(move0.startPos, move0.finalPos, nextState);
        }
        move a = possibleRowMoves[++lastRowMove].first;
        move b = possibleRowMoves[lastRowMove].second;
        nextState.removeRowAndRing(a);
        storeMove(nextState, RemoveRow, coordinateToMove(a.startPos.first, a.startPos.second), coordinateToMove(a.finalPos.first, a.finalPos.second));
        if (playerID == 1)
            storeMove(nextState, RemoveRing, coordinateToMove(ringPos2[a.index].first, ringPos2[a.index].second));
        else
            storeMove(nextState, RemoveRing, coordinateToMove(ringPos1[a.index].first, ringPos1[a.index].second));
        if (b.direction > 0)
        {
            nextState.removeRowAndRing(b);
            storeMove(nextState, RemoveRow, coordinateToMove(b.startPos.first, b.startPos.second), coordinateToMove(b.finalPos.first, b.finalPos.second));
            if (playerID == 1)
                storeMove(nextState, RemoveRing, coordinateToMove(ringPos2[b.index].first, ringPos2[b.index].second));
            else
                storeMove(nextState, RemoveRing, coordinateToMove(ringPos1[b.index].first, ringPos1[b.index].second));
        }
    }
    else
    {
        lastRowMove = 0;
        possibleRows.clear();
        possibleRowMoves.clear();
        if (moves.empty())
            possibleMoves();
        if (currentMove < moves.size())
        {
            storeMove(nextState, MoveRing, coordinateToMove(moves[currentMove].startPos.first, moves[currentMove].startPos.second), coordinateToMove(moves[currentMove].finalPos.first, moves[currentMove].finalPos.second));
            moveRing(moves[currentMove], nextState);
            makeRows(nextState, flipMarkers(pos.startPos, pos.finalPos, nextState), pos.startPos, pos.finalPos);
            if (possibleRows.empty())
                return true;
            makeAllRowCombinations(nextState);
            nextState.removeRowAndRing(possibleRowMoves[lastRowMove].first);
            move m = possibleRowMoves[lastRowMove].first;
            storeMove(nextState, RemoveRow, coordinateToMove(m.startPos.first, m.startPos.second), coordinateToMove(m.finalPos.first, m.finalPos.second));
            if (playerID == 1)
                storeMove(nextState, RemoveRing, coordinateToMove(ringPos2[m.index].first, ringPos2[m.index].second));
            else
                storeMove(nextState, RemoveRing, coordinateToMove(ringPos1[m.index].first, ringPos1[m.index].second));
            if (possibleRowMoves[lastRowMove].second.direction > 0)
            {
                m = possibleRowMoves[lastRowMove].second;
                nextState.removeRowAndRing(m);
                storeMove(nextState, RemoveRow, coordinateToMove(m.startPos.first, m.startPos.second), coordinateToMove(m.finalPos.first, m.finalPos.second));
                if (playerID == 1)
                    storeMove(nextState, RemoveRing, coordinateToMove(ringPos2[m.index].first, ringPos2[m.index].second));
                else
                    storeMove(nextState, RemoveRing, coordinateToMove(ringPos1[m.index].first, ringPos1[m.index].second));
            }
            currentMove++;
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
        nextState.ringPlayer2[boardSize + coor.fisrt][boardSize + coor.second] = 1;
        nextState.playerID = 2;
    }
    else
    {
        nextState.ringPos1.push_back(coor);
        nextState.ringPlayer1[boardSize + coor.fisrt][boardSize + coor.second] = 1;
        nextState.playerID = 1;
    }
    move0 = move(coordinateToMove(coor.first, coor.second), std::pair<int, int>(0, 0));
    nextState.moves.clear();
    nextState.currentMove = 0;
    nextState.move0 = move(std::pair<int, int>(0, 0), std::pair<int, int>(0, 0));
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
        if (a.startPos.first <= b.startPos.first && a.startPosPos.first >= b.startPos.first && a.startPos.second <= b.finalPos.second && a.startPos.second >= b.startPos.second)
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
}
void State::removeRowAndRing(const move &a)
{
    if (a.direction == 1)
    {
        for (int i = a.startPos.first; i <= a.finalPos.first; ++i)
        {
            if (playerID == 1)
            {
                markerPlayer2[boardSize + i][boardSize + a.startPos.second] = 0;
                --numMarkers2;
            }
            else
            {
                markerPlayer1[boardSize + i][boardSize + a.startPos.second] = 0;
                --numMarkers1;
            }
        }
    }
    if (a.direction == 2)
    {
        for (int i = a.startPos.second; i <= a.finalPos.second; ++i)
        {
            if (playerID == 1)
            {
                markerPlayer2[boardSize + a.startPos.first][boardSize + i] = 0;
                --numMarkers2;
            }
            else
            {
                markerPlayer1[boardSize + a.startPos.first][boardSize + i] = 0;
                --numMarkers1;
            }
        }
    }
    if (a.direction == 3)
    {
        for (int i = 0; i <= (a.finalPos.first - a.startPos.first); ++i)
        {
            if (playerID == 1)
            {
                markerPlayer2[boardSize + a.startPos.first + i][boardSize + a.startPos.second + i] = 0;
                --numMarkers2;
            }
            else
            {
                markerPlayer1[boardSize + a.startPos.first + i][boardSize + a.startPos.second + i] = 0;
                --numMarkers1;
            }
        }
    }
    if (playerID == 1)
    {
        ringPlayer2[boardSize + ringPos2[a.index].first][boardSize + ringPos2[a.index].second] = 0;
        ringPos2.erase(ringPos2.begin() + a.index);
        ++score1;
    }
    else
    {
        ringPlayer1[boardSize + ringPos1[a.index].first][boardSize + ringPos1[a.index].second] = 0;
        ringPos1.erase(ringPos1.begin() + a.index);
        ++score2;
    }
}
void State::makeAllRowCombinations(State &nextState)
{
    if ((playerID == 1 && ringPos2.size() == 3) || (playerID == 2 && ringPos1.size() == 3))
    {
        possibleRows[0].index = 2;
        possibleRowMoves.push_back(std::pair<move, move>(possibleRows[0], move(-1)));
    }
    else if ((playerID == 1 && ringPos2.size() > 3) || (playerID == 2 && ringPos1.size() > 3))
    {
        int maxRings;
        if (playerID == 1)
            maxRings = ringPos2.size();
        else
            maxRings = ringPos1.size();
        for (int i = 0; i < possibleRows.size(); ++i)
            for (int j = i + 1; j < possibleRows.size(); ++i)
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
                        possibleRowMoves.push_back(std::pair<move, move>(a, b));
                        return;
                    }
                    possibleRowMoves.push_back(std::pair<move, move>(a, b));
                }
            }
        if (possibleRowMoves.size() == 0)
            for (int i = 0; i < possibleRows.size(); ++i)
                for (int r = maxRings; r >= 0; --r)
                {
                    possibleRows[i].index = r;
                    possibleRowMoves.push_back(std::pair<move,move>(possibleRows[0],move(-1));
                }
    }
}
void State::moveRing(move pos, State &nextState)
{
    move0 = move(coordinateToMove(pos.startPos.first, pos.startPos.second), coordinateToMove(pos.finalPos.first, pos.finalPos.second));
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
        for (int f = 0; f != flipped.size(); ++f)
        {
            if (f > 0 && std::abs(flipped[f - 1].first - flipped[f].first) > 1 && !once)
                check = true;

            if (check)
            {
                vector<move> dir = nextState.checkRow(flipped[f], 3);
                possibleRows.insert(possibleRows.end(), dir.begin(), dir.end());
                check ^= 1;
                if (!dir.empty())
                    once = true;
                if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && once == true)
                    return;
            }
            vector<move> dir1 = nextState.checkRow(flipped[f], 2);
            possibleRows.insert(possibleRows.end(), dir1.begin(), dir1.end());
            if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && !dir1.empty())
                return;
            vector<move> dir2 = nextState.checkRow(flipped[f], 1);
            possibleRows.insert(possibleRows.end(), dir2.begin(), dir2.end());
            if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && !dir2.empty())
                return;
        }
    }
    else if (start.first == end.first)
    {
        for (int f = 0; f != flipped.size(); ++f)
        {
            if (f > 0 && std::abs(flipped[f - 1].second - flipped[f].second) > 1 && !once)
                check = true;
            if (check)
            {
                vector<move> dir = nextState.checkRow(flipped[f], 2);
                possibleRows.insert(possibleRows.end(), dir.begin(), dir.end());
                check ^= 1;
                if (!dir.empty())
                    once = true;
                if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && !dir.empty())
                    return;
            }
            vector<move> dir1 = nextState.checkRow(flipped[f], 3);
            possibleRows.insert(possibleRows.end(), dir1.begin(), dir1.end());
            if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && !dir1.empty())
                return;
            vector<move> dir2 = nextState.checkRow(flipped[f], 1);
            possibleRows.insert(possibleRows.end(), dir2.begin(), dir2.end());
            if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && !dir2.empty())
                return;
        }
    }
    else if (start.second == end.second)
    {
        for (int f = 0; f != flipped.size(); ++f)
        {
            if (f > 0 && std::abs(flipped[f - 1].first - flipped[f].first) > 1 && !once)
                check = true;
            if (check)
            {
                vector<move> dir = nextState.checkRow(flipped[f], 1);
                possibleRows.insert(possibleRows.end(), dir.begin(), dir.end());
                check ^= 1;
                if (!dir.empty())
                    once = true;
            }
            vector<move> dir1 = nextState.checkRow(flipped[f], 3);
            possibleRows.insert(possibleRows.end(), dir1.begin(), dir1.end());
            if (((ringPos1.size() == 3 && playerID == 2) || (ringPos2.size() == 3 && playerID == 1)) && !dir1.empty())
                return;
            vector<move> dir2 = nextState.checkRow(flipped[f], 2);
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
    bool check = true, once = false;
    if (start.first - start.second == end.first - end.second)
    {
        int startx = std::min(start.first, end.first);
        int endx = std::max(start.first, end.first);
        int starty = std::min(start.second, end.second);
        int endy = std::max(start.second, end.second);
        for (int i = startx, j = starty; i < endx; ++i, ++j)
            if (nextState.flip(i, j) && (playerID == 1 && nextState.markerPlayer2[boardSize + i][boardSize + j] == 1) || (playerID == 2 && nextState.markerPlayer1[boardSize + i][boardSize + j] == 1))
                flipped.push_back(std::pair<int, int>(i, j));
    }
    else if (start.first == end.first)
    {
        int start = std::min(start.second, end.second);
        int end = std::max(start.second, end.second);
        for (int i = start, i < end; ++i)
            if (nextState.flip(start.first, i) && (playerID == 1 && nextState.markerPlayer2[boardSize + start.first][boardSize + i] == 1) || (playerID == 2 && nextState.markerPlayer1[boardSize + start.first][boardSize + i] == 1))
                flipped.push_back(std::pair<int, int>(start.first, i));
    }
    else if (start.second == end.second)
    {
        int start = std::min(start.first, end.first);
        int end = std::max(start.first, end.first);
        for (int i = start, i < end; ++i)
            if (nextState.flip(i, start.second) && (playerID == 1 && nextState.markerPlayer2[boardSize + i][boardSize + start.second] == 1) || (playerID == 2 && nextState.markerPlayer1[boardSize + i][boardSize + start.second] == 1))
                flipped.push_back(std::pair<int, int>(i, start.second));
    }
    return
}
vector<move> checkRow(std::pair<int, int> coor, int dir)
{
    flip(coor.first, coor.second);
    vector<move> moves;
    if (dir == 1)
    {
        int i, end, start;
        for (i = 0; ((playerID == 1) && (boardSize + coor.first + i < 11) && (markerPlayer1[boardSize + coor.first + i][boardSize + coor.second] == 1)) || ((playerID == 2) && (boardSize + coor.first + i < 11) && (markerPlayer2[boardSize + coor.first + i][boardSize + coor.second] == 1)); ++i)
            ;
        end = i - 1;

        for (i = -1; ((playerID == 1) && (boardSize + coor.first + i < 11) && (markerPlayer1[boardSize + coor.first + i][boardSize + coor.second] == 1)) || ((playerID == 2) && (boardSize + coor.first + i < 11) && (markerPlayer2[boardSize + coor.first + i][boardSize + coor.second] == 1)); --i)
            ;
        start = i + 1;

        for (i = start; i + 4 <= end; ++i)
            moves.push_back(move(1, -1, std::pair<int, int>(coor.first + i, coor.second), std::pair<int, int>(coor.first + i + 4, coor.second)));
    }
    if (dir == 2)
    {
        int i, end, start;
        for (i = 0; ((playerID == 1) && (boardSize + coor.second + i < 11) && (markerPlayer2[boardSize + coor.first][boardSize + coor.second + i] == 1)) || ((playerID == 2) && (boardSize + coor.second + i < 11) && (markerPlayer1[boardSize + coor.first][boardSize + coor.second + i] == 1)); ++i)
            ;
        end = i - 1;

        for (i = -1; ((playerID == 1) && (boardSize + coor.second + i < 11) && (markerPlayer2[boardSize + coor.first][boardSize + coor.second + i] == 1)) || ((playerID == 2) && (boardSize + coor.second + i < 11) && (markerPlayer1[boardSize + coor.first][boardSize + coor.second + i] == 1)); --i)
            ;
        start = i + 1;

        for (i = start; i + 4 <= end; ++i)
            moves.push_back(move(2, -1, std::pair<int, int>(coor.first, coor.second + i), std::pair<int, int>(coor.first, coor.second + i + 4)));
    }
    if (dir == 3)
    {
        int i, end, start;
        for (i = 0; ((playerID == 1) && (boardSize + coor.second + i < 11) && (boardSize + coor.first + i < 11) && (markerPlayer2[boardSize + coor.first + i][boardSize + coor.second + i] == 1)) || ((playerID == 2) && (boardSize + coor.first + i < 11) && (boardSize + coor.second + i < 11) && (markerPlayer1[boardSize + coor.first + i][boardSize + coor.second + i] == 1)); ++i)
            ;
        end = i - 1;

        for (i = -1; ((playerID == 1) && (boardSize + coor.first + i < 11) && (boardSize + coor.second + i < 11) && (markerPlayer2[boardSize + coor.first + i][boardSize + coor.second + i] == 1)) || ((playerID == 2) && (boardSize + coor.first + i < 11) && (boardSize + coor.second + i < 11) && (markerPlayer1[boardSize + coor.first + i][boardSize + coor.second + i] == 1)); --i)
            ;
        start = i + 1;

        for (i = start; i + 4 <= end; ++i)
            moves.push_back(move(3, -1, std::pair<int, int>(coor.first + i, coor.second + i), std::pair<int, int>(coor.first + i, coor.second + i + 4)));
    }
    flip(coor.first, coor.second);
}