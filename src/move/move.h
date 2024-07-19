//
// Created by Alex Gisi on 3/15/22.
//

#ifndef ATHENE_MOVE_H
#define ATHENE_MOVE_H

#include <string>
#include "../defs.h"
#include "../util/assertions.cpp"

class Board;

class Move {
public:
    Move();
    explicit Move(int m);
    Move(int from, int to, int cap, int pro, int f1);

    Move(const Move& rhs);
    Move& operator=(const Move& rhs);
    bool operator==(const Move& rhs) const;

    [[nodiscard]] int from() const;
    [[nodiscard]] int to() const;
    [[nodiscard]] int captured() const;
    [[nodiscard]] int promoted() const;
    [[nodiscard]] bool ep_capture() const;
    [[nodiscard]] bool pawn_start() const;
    [[nodiscard]] bool castle() const;
    [[nodiscard]] bool capture() const;
    [[nodiscard]] bool promotion() const;

    [[nodiscard]] std::string to_str() const;
    static Move from_str(const std::string & move_str, Board & board);

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
