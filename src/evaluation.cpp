//
// Created by Alex Gisi on 6/19/22.
//

#include <cassert>
#include "Defs.h"
#include "Board.h"
#include "assertions.cpp"
#include "evaluation.h"

/*
 * Static evaluation in hundredths of a pawn.
 */
int eval(const Board& board) {
    int pce;
    int pce_num;
    int sq;
    int score = board.material[WHITE] - board.material[BLACK];

    pce = wP;
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score += PieceSquareTables::pawnTable[sq64(sq)];
    }

    pce = bP;
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score -= PieceSquareTables::pawnTable[PieceSquareTables::mirror64[sq64(sq)]];
    }

    pce = wN;	
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score += PieceSquareTables::knightTable[sq64(sq)];
    }	

    pce = bN;	
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score -= PieceSquareTables::knightTable[PieceSquareTables::mirror64[sq64(sq)]];
    }			
	
    pce = wB;	
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score += PieceSquareTables::bishopTable[sq64(sq)];
    }	

    pce = bB;	
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score -= PieceSquareTables::bishopTable[PieceSquareTables::mirror64[sq64(sq)]];
    }	

    pce = wR;	
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score += PieceSquareTables::rookTable[sq64(sq)];
    }	

    pce = bR;	
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score -= PieceSquareTables::rookTable[PieceSquareTables::mirror64[sq64(sq)]];
    }	

    if (board.side == WHITE)
        return score;
    else
        return -score;
}

std::array<int, 64> PieceSquareTables::pawnTable;
std::array<int, 64> PieceSquareTables::bishopTable;
std::array<int, 64> PieceSquareTables::knightTable;
std::array<int, 64> PieceSquareTables::rookTable;
