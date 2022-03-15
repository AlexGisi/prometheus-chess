//
// Created by Alex Gisi on 3/15/22.
//

#ifndef BLUE_MOVE_H
#define BLUE_MOVE_H

#include <string>


class Move {
public:
    Move();
    explicit Move(int m);

    Move(const Move& rhs);
    Move& operator=(const Move& rhs);

    int from() const;
    int to() const;
    int captured() const;
    int promoted() const;
    bool ep_capture() const;
    bool pawn_start() const;
    bool castle() const;
    bool capture() const;
    bool promotion() const;

    std::string toStr() const;

private:
    int move;
};


#endif //BLUE_MOVE_H
