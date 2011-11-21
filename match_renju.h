#ifndef MATCH_RENJU_H
#define MATCH_RENJU_H

#include "match.h"

const int RENJU_SIZE = 15;

class MatchRenju : Match {
public:
    int CheckWinner();
    int MakeMove(Move *move);
    int Start();
private:
    int m_board[RENJU_SIZE][RENJU_SIZE];
};

#endif
