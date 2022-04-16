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
    static void addWhitePawnMove(int from, int to, MoveList *list);

    Board board;
};


#endif //BLUE_MOVEGEN_H
