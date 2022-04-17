//
// Created by Alex Gisi on 3/23/22.
//

#ifndef BLUE_MOVEGEN_H
#define BLUE_MOVEGEN_H

#include <vector>
#include "Board.h"
#include "Move.h"
#include "MoveList.h"

class MoveGen {
public:
    explicit MoveGen(const Board& b);
    void generateAllMoves(MoveList *list) const;

private:
    static void addQuietMove(const Move& m, MoveList *list);
    static void addCaptureMove(const Move& m, MoveList *list);
    static void addEnPassantMove(const Move& m, MoveList *list);

    static void addWhitePawnCapMove(int from, int to, int cap, MoveList *list);
    static void addBlackPawnCapMove(int from, int to, int cap, MoveList *list);

    static void addWhitePawnMove(int from, int to, MoveList *list);
    static void addBlackPawnMove(int from, int to, MoveList *list);

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

    Board board;
};


#endif //BLUE_MOVEGEN_H
