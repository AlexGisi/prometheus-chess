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

#ifndef ATHENE_MOVE_H
#define ATHENE_MOVE_H

#include <string>
#include "../defs.h"
#include "../util/assertions.cpp"

class Board;

class Move {
public:
    Move();
    explicit Move(uint32_t m);
    Move(int from, int to, int cap, int pro, int f1);

    Move(const Move& rhs);
    Move& operator=(const Move& rhs);
    bool operator==(const Move& rhs) const;

    [[nodiscard]] inline bool is_no_move() const {
	    return move == 0;
    }

    [[nodiscard]] inline int from() const {
        return move & 0x7F;
    }

    [[nodiscard]] inline int to() const {
        return (move >> 7) & 0x7F;
    }

    [[nodiscard]] inline int captured() const {
        return (move >> 14) & 0xF;
    }

    [[nodiscard]] inline int promoted() const {
        return (move >> 20) & 0xF;
    }

    [[nodiscard]] inline bool ep_capture() const {
        return move & MFLAGEP;
    }

    [[nodiscard]] inline bool pawn_start() const {
        return move & MFLAGPS;
    }

    [[nodiscard]] inline bool castle() const {
        return move & MFLAGCA;
    }

    [[nodiscard]] inline bool capture() const {
        return move & 0x7C000;
    }

    [[nodiscard]] int to_int() const {
	    return move;
    }

    [[nodiscard]] std::string to_str() const;
    static Move from_str(const std::string & move_str, Board & board);

private:
    int move;
};

class SearchMove {
public:
    SearchMove() : move(Move()), score(0) {};
    SearchMove(const Move& p_move, int p_score)
        : move(p_move), score(p_score) {};
    SearchMove(const SearchMove& rhs) : move(rhs.move), score(rhs.score) {};
    SearchMove(SearchMove&& rhs) noexcept
            : move(rhs.move), score(rhs.score) {};

    SearchMove& operator=(const SearchMove& rhs) {
        if (this != &rhs) {  // Self-assignment check
            move = rhs.move;
            score = rhs.score;
        }
        return *this;
    }

    SearchMove& operator=(SearchMove&& rhs) noexcept {
        if (this != &rhs) {  // Self-assignment check
            move = rhs.move;
            score = rhs.score;
        }
        return *this;
    }

    Move move;
    int score;
};

#endif //ATHENE_MOVE_H
