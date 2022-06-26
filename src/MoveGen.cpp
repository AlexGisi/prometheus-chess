//
// Created by Alex Gisi on 3/23/22.
//

#include <cassert>
#include "MoveGen.h"
#include "assertions.cpp"
#include "iostream"

MoveGen::MoveGen(Board* b) {
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
    list->moves[list->count].score = mvvLvaScores[m.captured()][board->pieces[m.from()]];
    list->count++;
}

void MoveGen::addEnPassantMove(const Move& m, MoveList *list) {
    assert(sqOnBoard(m.from()));
    assert(sqOnBoard(m.to()));

    list->moves[list->count].move = m;
    list->moves[list->count].score = 105;
    list->count++;
}

void MoveGen::generateAllMoves(MoveList *list) {
    assert(board->checkBoard());

    list->count = 0;

    int pce = EMPTY;
    int side = board->side;
    int sq = 0, t_sq = 0, pceNum = 0;
    int dir = 0, index = 0, pceIndex = 0;

    if(side == WHITE) {
        for(pceNum = 0; pceNum < board->pceNum[wP]; ++pceNum) {
            sq = board->pceList[wP][pceNum];
            assert(sqOnBoard(sq));

            // Pawns.
            // Generate non-capture moves.
            if(board->pieces[sq+10] == EMPTY) {
                addWhitePawnMove(sq, sq+10, list);
                if(Board::ranksBrd[sq] == RANK_2 && board->pieces[sq+20] == EMPTY)
                    addQuietMove(Move(sq, sq+20, EMPTY, EMPTY, MFLAGPS), list);
            }

            // Generate captures.
            if(!sqOffBoard(sq) && pieceCol[board->pieces[sq+9]]==BLACK)
                addWhitePawnCapMove(sq, sq+9, board->pieces[sq+9], list);
            if(!sqOffBoard(sq) && pieceCol[board->pieces[sq+11]]==BLACK)
                addWhitePawnCapMove(sq, sq+11, board->pieces[sq+11], list);
            // En passant captures.
            if(board->enPas != NO_SQ) {
                if(sq+9 == board->enPas)
                    addEnPassantMove(Move(sq, sq+9, board->pieces[sq-1], EMPTY, MFLAGEP), list);
                if(sq+11 == board->enPas)
                    addEnPassantMove(Move(sq, sq+11, board->pieces[sq+1], EMPTY, MFLAGEP), list);
            }
        }

        // Generate short castle.
        if(board->castlePerm & WKCA) {
            if(board->pieces[F1] == EMPTY && board->pieces[G1] == EMPTY) {
                if(!board->sqAttacked(E1, BLACK) && !board->sqAttacked(F1, BLACK))
                    addQuietMove(Move(E1, G1, EMPTY, EMPTY, MFLAGCA), list);
            }
        }

        // Generate long castle.
        if(board->castlePerm & WQCA) {
            if(board->pieces[D1] == EMPTY && board->pieces[C1] == EMPTY && board->pieces[B1] == EMPTY) {
                if(!board->sqAttacked(E1, BLACK) && !board->sqAttacked(D1, BLACK))
                    addQuietMove(Move(E1, C1, EMPTY, EMPTY, MFLAGCA), list);
            }
        }
    } else {
        for(pceNum = 0; pceNum < board->pceNum[bP]; ++pceNum) {
            sq = board->pceList[bP][pceNum];
            assert(sqOnBoard(sq));

            // Pawns.
            // Generate non-capture moves.
            if(board->pieces[sq-10] == EMPTY) {
                addBlackPawnMove(sq, sq-10, list);
                if(Board::ranksBrd[sq] == RANK_7 && board->pieces[sq-20] == EMPTY)
                    addQuietMove(Move(sq, sq-20, EMPTY, EMPTY, MFLAGPS), list);
            }

            // Generate captures.
            if(!sqOffBoard(sq-9) && pieceCol[board->pieces[sq-9]] == WHITE)
                addBlackPawnCapMove(sq, sq-9, board->pieces[sq-9], list);
            if(!sqOffBoard(sq-11) && pieceCol[board->pieces[sq-11]] == WHITE)
                addBlackPawnCapMove(sq, sq-11, board->pieces[sq-11], list);

            // En passant captures.
            if(board->enPas != NO_SQ) {
                if(sq-9 == board->enPas)
                    addEnPassantMove(Move(sq, sq-9, board->pieces[sq+1], EMPTY, MFLAGEP), list);
                if(sq-11 == board->enPas)
                    addEnPassantMove(Move(sq, sq-11, board->pieces[sq-1], EMPTY, MFLAGEP), list);
            }
        }

        // Generate short castle.
        if(board->castlePerm & BKCA) {
            if(board->pieces[F8] == EMPTY && board->pieces[G8] == EMPTY) {
                if(!board->sqAttacked(E8, WHITE) && !board->sqAttacked(F8, WHITE))
                    addQuietMove(Move(E8, G8, EMPTY, EMPTY, MFLAGCA), list);
            }
        }

        // Generate long castle.
        if(board->castlePerm & BQCA) {
            if(board->pieces[D8] == EMPTY && board->pieces[C8] == EMPTY && board->pieces[B8] == EMPTY) {
                if(!board->sqAttacked(E8, WHITE) && !board->sqAttacked(D8, WHITE))
                    addQuietMove(Move(E8, C8, EMPTY, EMPTY, MFLAGCA), list);
            }
        }
    }

    // Loop for slide pieces.
    pceIndex = loopSlideIdx[side];
    pce = loopSlidePce[pceIndex];
    while(pce != 0) {
        assert(pieceValid(pce));
        pce = loopSlidePce[pceIndex++];

        for(pceNum = 0; pceNum < board->pceNum[pce]; ++pceNum) {
            sq = board->pceList[pce][pceNum];
            assert(sqOnBoard(sq));

            for(index = 0; index < numDir[pce]; ++index) {
                dir = pceDir[pce][index];
                t_sq = sq + dir;

                while(!sqOffBoard(t_sq)) {
                    if(board->pieces[t_sq] != EMPTY) {
                        if(pieceCol[board->pieces[t_sq]] == !side)
                            addCaptureMove(Move(sq, t_sq, board->pieces[t_sq], EMPTY, 0), list);
                        break;
                    }
                    addQuietMove(Move(sq, t_sq, EMPTY, EMPTY, 0), list);
                    t_sq += dir;
                }
            }
        }
    }

    // Loop for non-slide pieces.
    pceIndex = loopNonSlideIdx[side];
    pce = loopNonSlidePce[pceIndex++];
    while(pce != 0) {
        assert(pieceValid(pce));

        for(pceNum = 0; pceNum < board->pceNum[pce]; ++pceNum) {
            sq = board->pceList[pce][pceNum];
            assert(sqOnBoard(sq));

            for(index = 0; index < numDir[pce]; ++index) {
                dir = pceDir[pce][index];
                t_sq = sq + dir;

                if(sqOffBoard(t_sq))
                    continue;

                if(board->pieces[t_sq] != EMPTY) {
                    if(pieceCol[board->pieces[t_sq]] == !side) {
                        addCaptureMove(Move(sq, t_sq, board->pieces[t_sq], EMPTY, 0), list);
                    }
                    continue;
                }
                addQuietMove(Move(sq, t_sq, EMPTY, EMPTY, 0), list);
            }
        }

        pce = loopNonSlidePce[pceIndex++];
    }
}

void MoveGen::addWhitePawnCapMove(int from, int to, int cap, MoveList *list) {
    assert(pieceValidEmpty(cap));
    assert(sqOnBoard(from));
    assert(sqOnBoard(to));

    if(Board::ranksBrd[from] == RANK_7) {
        addCaptureMove(Move(from, to, cap, wQ, 0), list);
        addCaptureMove(Move(from, to, cap, wR, 0), list);
        addCaptureMove(Move(from, to, cap, wB, 0), list);
        addCaptureMove(Move(from, to, cap, wN, 0), list);
    } else {
        addCaptureMove(Move(from, to, cap, EMPTY, 0), list);
    }
}

void MoveGen::addBlackPawnCapMove(int from, int to, int cap, MoveList *list) {
    assert(pieceValidEmpty(cap));
    assert(sqOnBoard(from));
    assert(sqOnBoard(to));

    if(Board::ranksBrd[from] == RANK_2) {
        addCaptureMove(Move(from, to, cap, bQ, 0), list);
        addCaptureMove(Move(from, to, cap, bR, 0), list);
        addCaptureMove(Move(from, to, cap, bB, 0), list);
        addCaptureMove(Move(from, to, cap, bN, 0), list);
    } else {
        addCaptureMove(Move(from, to, cap, EMPTY, 0), list);
    }
}

void MoveGen::addWhitePawnMove(int from, int to, MoveList *list) {
    assert(sqOnBoard(from));
    assert(sqOnBoard(to));

    if(Board::ranksBrd[from] == RANK_7) {
        addQuietMove(Move(from, to, EMPTY, wQ, 0), list);
        addQuietMove(Move(from, to, EMPTY, wR, 0), list);
        addQuietMove(Move(from, to, EMPTY, wB, 0), list);
        addQuietMove(Move(from, to, EMPTY, wN, 0), list);
    } else {
        addQuietMove(Move(from, to, EMPTY, EMPTY, 0), list);
    }
}

void MoveGen::addBlackPawnMove(int from, int to, MoveList *list) {
    assert(sqOnBoard(from));
    assert(sqOnBoard(to));

    if(Board::ranksBrd[from] == RANK_2) {
        addQuietMove(Move(from, to, EMPTY, bQ, 0), list);
        addQuietMove(Move(from, to, EMPTY, bR, 0), list);
        addQuietMove(Move(from, to, EMPTY, bB, 0), list);
        addQuietMove(Move(from, to, EMPTY, bN, 0), list);
    } else {
        addQuietMove(Move(from, to, EMPTY, EMPTY, 0), list);
    }
}

/*
 * Returns whether the move is possible for the current board->
 */
bool MoveGen::moveValid(const Move& move) {
    MoveList ml;
    generateAllMoves(&ml);

    for(int i=0; i < ml.count; ++i) {
        Move mv = ml.moves[i].move;
        if (!board->makeMove(mv))
            continue;
        board->takeMove();

        if (ml.moves[i].move == mv)
            return true;
    }

    return false;
}

int MoveGen::initMvvLva() {
    for(int attacker = wP; attacker <= bK; ++attacker) {
        for(int victim = wP; victim <= bK; ++victim)
            mvvLvaScores[victim][attacker] = victimScore[victim] + 6 - (victimScore[attacker] / 100);
    }

    return 0;
}

int MoveGen::mvvLvaScores[13][13];
