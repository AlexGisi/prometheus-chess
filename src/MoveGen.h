//
// Created by Alex Gisi on 3/23/22.
//

#ifndef ATHENE_MOVEGEN_H
#define ATHENE_MOVEGEN_H

#include <vector>
#include "Defs.h"
#include "Move.h"

class Board;

using MoveListPtr = std::shared_ptr<std::vector<SearchMove>>;

class MoveGen {
public:
    explicit MoveGen(Board* b);
    MoveListPtr generateAllMoves();
    MoveListPtr generateAllCaps();
    bool moveValid(const Move& move);

    static int initMvvLva();

    // Move ordering -- most valuable victim, least valuable attacker.
    constexpr static int victimScore[13] = { 0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600 };
    static int mvvLvaScores[13][13];

private:
    void addQuietMove(const Move& m, const MoveListPtr& list);
    void addCaptureMove(const Move& m, const MoveListPtr& list);
    static void addEnPassantMove(const Move& m, const MoveListPtr& list);

    void addWhitePawnCapMove(int from, int to, int cap, const MoveListPtr& list);
    void addBlackPawnCapMove(int from, int to, int cap, const MoveListPtr& list);

    void addWhitePawnMove(int from, int to, const MoveListPtr& list);
    void addBlackPawnMove(int from, int to, const MoveListPtr& list);

    const int loopSlidePce[8] = { wB, wR, wQ, 0, bB, bR, bQ, 0 };
    int loopSlideIdx[2] = { 0, 4 };  // Index the above array by side.

    const int loopNonSlidePce[6] = { wN, wK, 0, bN, bK, 0 };
    const int loopNonSlideIdx[2] = { 0, 3 };  // Index the above array by side.

    // What squares can a piece move to.
    const int pceDir[13][8] = {
            { 0, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0, 0 },
            { -8, -19,	-21, -12, 8, 19, 21, 12 },
            { -9, -11, 11, 9, 0, 0, 0, 0 },
            { -1, -10,	1, 10, 0, 0, 0, 0 },
            { -1, -10,	1, 10, -9, -11, 11, 9 },
            { -1, -10,	1, 10, -9, -11, 11, 9 },
            { 0, 0, 0, 0, 0, 0, 0 },
            { -8, -19,	-21, -12, 8, 19, 21, 12 },
            { -9, -11, 11, 9, 0, 0, 0, 0 },
            { -1, -10,	1, 10, 0, 0, 0, 0 },
            { -1, -10,	1, 10, -9, -11, 11, 9 },
            { -1, -10,	1, 10, -9, -11, 11, 9 }
    };

    // How many directions does a piece have.
    const int numDir[13] = {
            0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8
    };

    Board* board;
};


#endif //ATHENE_MOVEGEN_H
