//
// Created by Alex Gisi on 3/23/22.
//

#include "MoveGen.h"
#include "assertions.cpp"
#include "iostream"

MoveGen::MoveGen(const Board& b) {
    board = b;
}

void MoveGen::addQuietMove(const Move& m, MoveList *list) {
    assert(sqOnBoard(m.from()));
    assert(sqOnBoard(m.to()));

    list->moves[list->count].move = m;
    list->moves[list->count].score = 0;
    list->count++;
}

void MoveGen::addCaptureMove(const Move& m, MoveList *list) {
    assert(sqOnBoard(m.from()));
    assert(sqOnBoard(m.to()));
    assert(pieceValid(m.captured()));

    list->moves[list->count].move = m;
    list->moves[list->count].score = 0;
    list->count++;
}

void MoveGen::addEnPassantMove(const Move& m, MoveList *list) {
    assert(sqOnBoard(m.from()));
    assert(sqOnBoard(m.to()));

    list->moves[list->count].move = m;
    list->moves[list->count].score = 0;
    list->count++;
}

void MoveGen::generateAllMoves(MoveList *list) const {
    assert(board.checkBoard());

    list->count = 0;

    int pce = EMPTY;
    int side = board.side;
    int sq = 0, t_sq = 0, pceNum = 0;

    if(side == WHITE) {
        for(pceNum = 0; pceNum < board.pceNum[wP]; ++pceNum) {
            sq = board.pceList[wP][pceNum];
            assert(sqOnBoard(sq));

            // Pawns.
            // Generate non-capture moves.
            if(board.pieces[sq+10] == EMPTY) {
                addWhitePawnMove(sq, sq+10, list);
                if(Board::ranksBrd[sq] == RANK_2 && board.pieces[sq+20] == EMPTY)
                    addQuietMove(Move(sq, sq+20, EMPTY, EMPTY, MFLAGEP), list);
            }

            // Generate captures.
            if(!sqOffBoard(sq) && pieceCol[board.pieces[sq+9]]==BLACK)
                addWhitePawnCapMove(sq, sq+9, board.pieces[sq+9], list);
            if(!sqOffBoard(sq) && pieceCol[board.pieces[sq+11]]==BLACK)
                addWhitePawnCapMove(sq, sq+11, board.pieces[sq+11], list);
            // En passant captures.
            if(board.enPas != NO_SQ) {
                if(sq+9 == board.enPas)
                    addCaptureMove(Move(sq, sq+9, board.pieces[sq-1], EMPTY, MFLAGEP), list);
                if(sq+11 == board.enPas)
                    addCaptureMove(Move(sq, sq+11, board.pieces[sq+1], EMPTY, MFLAGEP), list);
            }

            // Etc.
        }
    } else {

    }
}

void MoveGen::addWhitePawnCapMove(int from, int to, int cap, MoveList *list) {
    if(Board::ranksBrd[from] == RANK_7) {
        addCaptureMove(Move(from, to, cap, wQ, 0), list);
        addCaptureMove(Move(from, to, cap, wR, 0), list);
        addCaptureMove(Move(from, to, cap, wB, 0), list);
        addCaptureMove(Move(from, to, cap, wN, 0), list);
    } else {
        addCaptureMove(Move(from, to, cap, EMPTY, 0), list);
    }
}

void MoveGen::addWhitePawnMove(int from, int to, MoveList *list) {
    if(Board::ranksBrd[from] == RANK_7) {
        addQuietMove(Move(from, to, EMPTY, wQ, 0), list);
        addQuietMove(Move(from, to, EMPTY, wR, 0), list);
        addQuietMove(Move(from, to, EMPTY, wB, 0), list);
        addQuietMove(Move(from, to, EMPTY, wN, 0), list);
    } else {
        addQuietMove(Move(from, to, EMPTY, EMPTY, 0), list);
    }
}
