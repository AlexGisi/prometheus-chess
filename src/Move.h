//
// Created by Alex Gisi on 3/15/22.
//

#ifndef ATHENE_MOVE_H
#define ATHENE_MOVE_H

#include <string>
#include "Defs.h"


class Move {
public:
    Move();
    explicit Move(int m);
    Move(int from, int to, int cap, int pro, int f1);

    Move(const Move& rhs);
    Move& operator=(const Move& rhs);
    bool operator==(const Move& rhs) const;

    int from() const;
    int to() const;
    int captured() const;
    int promoted() const;
    bool ep_capture() const;
    bool pawn_start() const;
    bool castle() const;
    bool capture() const;
    bool promotion() const;

    std::string to_str() const;

    int move;
};

typedef struct {
    Move move;
    int score;
} SearchMove;


#endif //ATHENE_MOVE_H
