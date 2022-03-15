//
// Created by Alex Gisi on 3/15/22.
//

/*
 * How we represent a game move in a (32-bit) integer:
 *
 * 0000 0000 0000 0000 0000 0111 1111 -> From square
 * 0000 0000 0000 0011 1111 1000 0000 -> To square
 * 0000 0000 0011 1100 0000 0000 0000 -> Captured piece
 * 0000 0000 0100 0000 0000 0000 0000 -> En Passant capture
 * 0000 0000 1000 0000 0000 0000 0000 -> Was pawn start
 * 0000 1111 0000 0000 0000 0000 0000 -> Indicate promotion: piece and side
 * 0001 0000 0000 0000 0000 0000 0000 -> Was a castle
 */

#include <sstream>
#include "Move.h"
#include "Board.h"

Move::Move() {
    move = 0;
}

Move::Move(int m) {
    move = m;
}

Move::Move(const Move &rhs) {
    move = rhs.move;
}

Move& Move::operator=(const Move& rhs) = default;

int Move::from() const {
    return move & 0x7F;
}

int Move::to() const {
    return (move >> 7) & 0x7F;
}

int Move::captured() const {
    return (move >> 14) & 0xF;
}

int Move::promoted() const {
    return (move >> 20) & 0xF;
}

bool Move::ep_capture() const {
    return move & 0x40000;
}

bool Move::pawn_start() const {
    return move & 0x80000;
}

bool Move::castle() const {
    return move & 0x1000000;
}

bool Move::capture() const {
    return move & 0x7C000;
}

bool Move::promotion() const {
    return move & 0xF00000;
}

std::string Move::toStr() const {
    std::stringstream ss;
    std::string s;

    char ff = char(Board::filesBrd[from()]+'a');
    char rf = char(Board::ranksBrd[from()]+'1');
    char ft = char(Board::filesBrd[to()]+'a');
    char rt = char(Board::ranksBrd[to()]+'1');

    ss << ff << rf << ft << rt;

    int promoted = this->promoted();
    if(promoted) {
        char pchar = 'q';
        if(isKn(promoted))
            pchar = 'n';
        else if(isRQ(promoted) && !isBQ(promoted))
            pchar = 'r';
        else if(!isRQ(promoted) && isBQ(promoted))
            pchar = 'b';
        ss << pchar;
    }

    ss >> s;

    return s;
}
