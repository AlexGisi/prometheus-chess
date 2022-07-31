//
// Created by Alex Gisi on 3/24/22.
//

#ifndef ATHENE_MOVELIST_H
#define ATHENE_MOVELIST_H

#include "Move.h"


class MoveList {
public:
    MoveList();
    MoveList(const MoveList &rhs);
    MoveList& operator=(const MoveList &rhs);
    ~MoveList();

    void print() const;

    SearchMove moves[MAX_POSITION_MOVES];
    int count;
};


#endif //ATHENE_MOVELIST_H
