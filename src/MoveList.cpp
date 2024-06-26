//
// Created by Alex Gisi on 3/24/22.
//

#include "MoveList.h"

MoveList::MoveList() {
    for(auto & move : moves)
        move = SearchMove();
    count = 0;
}

MoveList::MoveList(const MoveList &rhs) {
    for(int i=0; i < MAX_POSITION_MOVES; ++i)
        moves[i] = rhs.moves[i];
    count = rhs.count;
}

MoveList &MoveList::operator=(const MoveList &rhs) {
    if(&rhs == this)
        return *this;

    for(int i=0; i < MAX_POSITION_MOVES; ++i)
        moves[i] = rhs.moves[i];
    count = rhs.count;

    return *this;
}

MoveList::~MoveList() = default;


void MoveList::print() const {
    int score = 0;
    Move mv;

    printf("MoveList (%d):\n", count);
    for(int i = 0; i < count; ++i) {
        mv = moves[i].move;
        score = moves[i].score;
        printf("Move %d: %s (score: %d)\n", i+1, mv.to_str().c_str(), score);
    }
}
