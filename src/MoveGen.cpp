//
// Created by Alex Gisi on 3/23/22.
//

#include <cassert>
#include "MoveGen.h"
#include "Move.h"
#include "Board.h"
#include "assertions.cpp"

MoveGen::MoveGen(Board* b) {
    board = b;
}

void MoveGen::addQuietMove(const Move& m, const MoveListPtr& list) {
    assert(Board::is_on_board(m.from()));
    assert(Board::is_on_board(m.to()));

    int score = 0;

    if(board->searchKillers[0][board->ply] == m) {
        score = 900000;
    } else if(board->searchKillers[1][board->ply] == m) {
        score = 800000;
    } else {
        score = board->searchHistory[board->pieces[m.from()]][m.to()];
    }

    list->emplace_back(m, score);
}

void MoveGen::addCaptureMove(const Move& m, const MoveListPtr& list) {
    assert(Board::is_on_board(m.from()));
    assert(Board::is_on_board(m.to()));
    assert(pieceValid(m.captured()));

    list->emplace_back(m, mvvLvaScores[m.captured()][board->pieces[m.from()]] + 1000000);
}

void MoveGen::addEnPassantMove(const Move& m, const MoveListPtr& list) {
    assert(Board::is_on_board(m.from()));
    assert(Board::is_on_board(m.to()));

    list->emplace_back(m, 105+1000000);
}

MoveListPtr MoveGen::generateAllMoves() {
    assert(board->check_board());

    MoveListPtr list = std::make_shared<std::vector<SearchMove>>();

    int pce = EMPTY;
    int side = board->side;
    int sq = 0, t_sq = 0, pceNum = 0;
    int dir = 0, index = 0, pceIndex = 0;

    if(side == WHITE) {
        for(pceNum = 0; pceNum < board->pceNum[wP]; ++pceNum) {
            sq = board->pceList[wP][pceNum];
            assert(Board::is_on_board(sq));

            // Pawns.
            // Generate non-capture moves.
            if(board->pieces[sq+10] == EMPTY) {
                addWhitePawnMove(sq, sq+10, list);
                if(Board::ranksBrd[sq] == RANK_2 && board->pieces[sq+20] == EMPTY)
                    addQuietMove(Move(sq, sq+20, EMPTY, EMPTY, MFLAGPS), list);
            }

            // Generate captures.
            if(!Board::is_off_board(sq) && pieceCol[board->pieces[sq+9]]==BLACK)
                addWhitePawnCapMove(sq, sq+9, board->pieces[sq+9], list);
            if(!Board::is_off_board(sq) && pieceCol[board->pieces[sq+11]]==BLACK)
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
                if(!board->sq_attacked(E1, BLACK) && !board->sq_attacked(F1, BLACK))
                    addQuietMove(Move(E1, G1, EMPTY, EMPTY, MFLAGCA), list);
            }
        }

        // Generate long castle.
        if(board->castlePerm & WQCA) {
            if(board->pieces[D1] == EMPTY && board->pieces[C1] == EMPTY && board->pieces[B1] == EMPTY) {
                if(!board->sq_attacked(E1, BLACK) && !board->sq_attacked(D1, BLACK))
                    addQuietMove(Move(E1, C1, EMPTY, EMPTY, MFLAGCA), list);
            }
        }
    } else {
        for(pceNum = 0; pceNum < board->pceNum[bP]; ++pceNum) {
            sq = board->pceList[bP][pceNum];
            assert(Board::is_on_board(sq));

            // Pawns.
            // Generate non-capture moves.
            if(board->pieces[sq-10] == EMPTY) {
                addBlackPawnMove(sq, sq-10, list);
                if(Board::ranksBrd[sq] == RANK_7 && board->pieces[sq-20] == EMPTY)
                    addQuietMove(Move(sq, sq-20, EMPTY, EMPTY, MFLAGPS), list);
            }

            // Generate captures.
            if(!Board::is_off_board(sq-9) && pieceCol[board->pieces[sq-9]] == WHITE)
                addBlackPawnCapMove(sq, sq-9, board->pieces[sq-9], list);
            if(!Board::is_off_board(sq-11) && pieceCol[board->pieces[sq-11]] == WHITE)
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
                if(!board->sq_attacked(E8, WHITE) && !board->sq_attacked(F8, WHITE))
                    addQuietMove(Move(E8, G8, EMPTY, EMPTY, MFLAGCA), list);
            }
        }

        // Generate long castle.
        if(board->castlePerm & BQCA) {
            if(board->pieces[D8] == EMPTY && board->pieces[C8] == EMPTY && board->pieces[B8] == EMPTY) {
                if(!board->sq_attacked(E8, WHITE) && !board->sq_attacked(D8, WHITE))
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
            assert(Board::is_on_board(sq));

            for(index = 0; index < numDir[pce]; ++index) {
                dir = pceDir[pce][index];
                t_sq = sq + dir;

                while(!Board::is_off_board(t_sq)) {
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
            assert(Board::is_on_board(sq));

            for(index = 0; index < numDir[pce]; ++index) {
                dir = pceDir[pce][index];
                t_sq = sq + dir;

                if(Board::is_off_board(t_sq))
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

    return list;
}

/*
 * TODO: Make more efficient/better?
 */
MoveListPtr MoveGen::generateAllCaps() {
    assert(board->check_board());

    MoveListPtr list = std::make_shared<std::vector<SearchMove>>();

    int pce = EMPTY;
    int side = board->side;
    int sq = 0, t_sq = 0, pceNum = 0;
    int dir = 0, index = 0, pceIndex = 0;

    if(side == WHITE) {
        for(pceNum = 0; pceNum < board->pceNum[wP]; ++pceNum) {
            sq = board->pceList[wP][pceNum];
            assert(Board::is_on_board(sq));

            // Pawns.
            // Generate captures.
            if(!Board::is_off_board(sq) && pieceCol[board->pieces[sq+9]]==BLACK)
                addWhitePawnCapMove(sq, sq+9, board->pieces[sq+9], list);
            if(!Board::is_off_board(sq) && pieceCol[board->pieces[sq+11]]==BLACK)
                addWhitePawnCapMove(sq, sq+11, board->pieces[sq+11], list);
            // En passant captures.
            if(board->enPas != NO_SQ) {
                if(sq+9 == board->enPas)
                    addEnPassantMove(Move(sq, sq+9, board->pieces[sq-1], EMPTY, MFLAGEP), list);
                if(sq+11 == board->enPas)
                    addEnPassantMove(Move(sq, sq+11, board->pieces[sq+1], EMPTY, MFLAGEP), list);
            }
        }
    } else {
        for(pceNum = 0; pceNum < board->pceNum[bP]; ++pceNum) {
            sq = board->pceList[bP][pceNum];
            assert(Board::is_on_board(sq));

            // Pawns.
            // Generate captures.
            if(!Board::is_off_board(sq-9) && pieceCol[board->pieces[sq-9]] == WHITE)
                addBlackPawnCapMove(sq, sq-9, board->pieces[sq-9], list);
            if(!Board::is_off_board(sq-11) && pieceCol[board->pieces[sq-11]] == WHITE)
                addBlackPawnCapMove(sq, sq-11, board->pieces[sq-11], list);

            // En passant captures.
            if(board->enPas != NO_SQ) {
                if(sq-9 == board->enPas)
                    addEnPassantMove(Move(sq, sq-9, board->pieces[sq+1], EMPTY, MFLAGEP), list);
                if(sq-11 == board->enPas)
                    addEnPassantMove(Move(sq, sq-11, board->pieces[sq-1], EMPTY, MFLAGEP), list);
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
            assert(Board::is_on_board(sq));

            for(index = 0; index < numDir[pce]; ++index) {
                dir = pceDir[pce][index];
                t_sq = sq + dir;

                while(!Board::is_off_board(t_sq)) {
                    if(board->pieces[t_sq] != EMPTY) {
                        if(pieceCol[board->pieces[t_sq]] == !side)
                            addCaptureMove(Move(sq, t_sq, board->pieces[t_sq], EMPTY, 0), list);
                        break;
                    }
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
            assert(Board::is_on_board(sq));

            for(index = 0; index < numDir[pce]; ++index) {
                dir = pceDir[pce][index];
                t_sq = sq + dir;

                if(Board::is_off_board(t_sq))
                    continue;

                if(board->pieces[t_sq] != EMPTY) {
                    if(pieceCol[board->pieces[t_sq]] == !side) {
                        addCaptureMove(Move(sq, t_sq, board->pieces[t_sq], EMPTY, 0), list);
                    }
                    continue;
                }
            }
        }

        pce = loopNonSlidePce[pceIndex++];
    }

    return list;
}

void MoveGen::addWhitePawnCapMove(int from, int to, int cap, const MoveListPtr& list) {
    assert(pieceValidEmpty(cap));
    assert(Board::is_on_board(from));
    assert(Board::is_on_board(to));

    if(Board::ranksBrd[from] == RANK_7) {
        addCaptureMove(Move(from, to, cap, wQ, 0), list);
        addCaptureMove(Move(from, to, cap, wR, 0), list);
        addCaptureMove(Move(from, to, cap, wB, 0), list);
        addCaptureMove(Move(from, to, cap, wN, 0), list);
    } else {
        addCaptureMove(Move(from, to, cap, EMPTY, 0), list);
    }
}

void MoveGen::addBlackPawnCapMove(int from, int to, int cap, const MoveListPtr& list) {
    assert(pieceValidEmpty(cap));
    assert(Board::is_on_board(from));
    assert(Board::is_on_board(to));

    if(Board::ranksBrd[from] == RANK_2) {
        addCaptureMove(Move(from, to, cap, bQ, 0), list);
        addCaptureMove(Move(from, to, cap, bR, 0), list);
        addCaptureMove(Move(from, to, cap, bB, 0), list);
        addCaptureMove(Move(from, to, cap, bN, 0), list);
    } else {
        addCaptureMove(Move(from, to, cap, EMPTY, 0), list);
    }
}

void MoveGen::addWhitePawnMove(int from, int to, const MoveListPtr& list) {
    assert(Board::is_on_board(from));
    assert(Board::is_on_board(to));

    if(Board::ranksBrd[from] == RANK_7) {
        addQuietMove(Move(from, to, EMPTY, wQ, 0), list);
        addQuietMove(Move(from, to, EMPTY, wR, 0), list);
        addQuietMove(Move(from, to, EMPTY, wB, 0), list);
        addQuietMove(Move(from, to, EMPTY, wN, 0), list);
    } else {
        addQuietMove(Move(from, to, EMPTY, EMPTY, 0), list);
    }
}

void MoveGen::addBlackPawnMove(int from, int to, const MoveListPtr& list) {
    assert(Board::is_on_board);
    assert(Board::is_on_board);

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
    auto ml = generateAllMoves();

    for(auto & i : *ml) {
        Move mv = i.move;
        if (!board->make_move(mv))
            continue;
        board->take_move();

        if (i.move == mv)
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
