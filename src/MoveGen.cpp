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
    int dir = 0, index = 0, pceIndex = 0;

    printf("\n\nSide:%d\n", side);

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
        }

        // Generate short castle.
        if(board.castlePerm & WKCA) {
            if(board.pieces[F1] == EMPTY && board.pieces[G1] == EMPTY) {
                if(!board.sqAttacked(E1, BLACK) && !board.sqAttacked(F1, BLACK))
                    printf("WKCA movegen\n");
            }
        }

        // Generate long castle.
        if(board.castlePerm & WQCA) {
            if(board.pieces[D1] == EMPTY && board.pieces[C1] == EMPTY && board.pieces[B1] == EMPTY) {
                if(!board.sqAttacked(E1, BLACK) && !board.sqAttacked(D1, BLACK))
                    printf("WQCA movegen\n");
            }
        }
    } else {
        for(pceNum = 0; pceNum < board.pceNum[bP]; ++pceNum) {
            sq = board.pceList[bP][pceNum];
            assert(sqOnBoard(sq));

            // Pawns.
            // Generate non-capture moves.
            if(board.pieces[sq-10] == EMPTY) {
                addBlackPawnMove(sq, sq-10, list);
                if(Board::ranksBrd[sq] == RANK_7 && board.pieces[sq-20] == EMPTY)
                    addQuietMove(Move(sq, sq-20, EMPTY, EMPTY, MFLAGEP), list);
            }

            // Generate captures.
            if(!sqOffBoard(sq) && pieceCol[board.pieces[sq-9]] == WHITE)
                addBlackPawnCapMove(sq, sq-9, board.pieces[sq-9], list);
            if(!sqOffBoard(sq) && pieceCol[board.pieces[sq-11]] == WHITE)
                addBlackPawnCapMove(sq, sq-11, board.pieces[sq-11], list);

            // En passant captures.
            if(board.enPas != NO_SQ) {
                if(sq-9 == board.enPas)
                    addCaptureMove(Move(sq, sq-9, board.pieces[sq+1], EMPTY, MFLAGEP), list);
                if(sq-11 == board.enPas)
                    addCaptureMove(Move(sq, sq-11, board.pieces[sq-1], EMPTY, MFLAGEP), list);
            }
        }

        // Generate short castle.
        if(board.castlePerm & BKCA) {
            if(board.pieces[F8] == EMPTY && board.pieces[G8] == EMPTY) {
                if(!board.sqAttacked(E8, WHITE) && !board.sqAttacked(F8, WHITE))
                    printf("BKCA movegen\n");
            }
        }

        // Generate long castle.
        if(board.castlePerm & BQCA) {
            if(board.pieces[D8] == EMPTY && board.pieces[C8] == EMPTY && board.pieces[B8] == EMPTY) {
                if(!board.sqAttacked(E8, WHITE) && !board.sqAttacked(D8, WHITE))
                    printf("BQCA movegen\n");
            }
        }
    }

    // Loop for slide pieces.
    pceIndex = loopSlideIdx[side];
    pce = loopSlidePce[pceIndex];
    while(pce != 0) {
        assert(pieceValid(pce));
        pce = loopSlidePce[pceIndex++];

        for(pceNum = 0; pceNum < board.pceNum[pce]; ++pceNum) {
            sq = board.pceList[pce][pceNum];
            assert(sqOnBoard(sq));
            printf("Piece: %c on %s\n", pceChar[pce], Board::sqToStr(sq).c_str());

            for(index = 0; index < numDir[pce]; ++index) {
                dir = pceDir[pce][index];
                t_sq = sq + dir;

                while(!sqOffBoard(t_sq)) {
                    if(board.pieces[t_sq] != EMPTY) {
                        if(pieceCol[board.pieces[t_sq]] == !side) {
                            std::cout << "\t\tCapture on " << Board::sqToStr(t_sq) << std::endl;
                        }
                        break;
                    }
                    std::cout << "\t\tNormal on " << Board::sqToStr(t_sq) << std::endl;
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
        printf("Non-sliders pceIndex:%d pce: %d\n", pceIndex-1, pce);

        for(pceNum = 0; pceNum < board.pceNum[pce]; ++pceNum) {
            sq = board.pceList[pce][pceNum];
            assert(sqOnBoard(sq));
            printf("Piece: %c on %s\n", pceChar[pce], Board::sqToStr(sq).c_str());

            for(index = 0; index < numDir[pce]; ++index) {
                dir = pceDir[pce][index];
                t_sq = sq + dir;

                if(sqOffBoard(t_sq))
                    continue;

                if(board.pieces[t_sq] != EMPTY) {
                    if(pieceCol[board.pieces[t_sq]] == !side) {
                        std::cout << "\t\tCapture on " << Board::sqToStr(t_sq) << std::endl;
                    }
                    continue;
                }
                std::cout << "\t\tNormal on " << Board::sqToStr(t_sq) << std::endl;
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
