//
// Created by Alex Gisi on 6/19/22.
//

#include "Defs.h"
#include "Board.h"
#include "assertions.cpp"

// Piece-square tables, in centipawns.
const int pawnTable[64] = {
        0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
        10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
        5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
        0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
        5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
        10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
        20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
        0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int knightTable[64] = {
        0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
        0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
        0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
        0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
        5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
        5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int bishopTable[64] = {
        0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
        0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
        0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
        0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
        0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
        0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
        0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
        0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int rookTable[64] = {
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
        25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
        0	,	0	,	5	,	10	,	10	,	5	,	0	,	0
};

const int mirror64[64] = {
        56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
        48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
        40	,	41	,	42	,	43	,	44	,	45	,	46	,	47	,
        32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
        24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
        16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
        8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
        0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
};

/*
 * Static evaluation in hundredths of a pawn.
 */
inline int eval(const Board& board) {
    int pce;
    int pce_num;
    int sq;
    int score = board.material[WHITE] - board.material[BLACK];

    pce = wP;
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score += pawnTable[sq64(sq)];
    }

    pce = bP;
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score -= pawnTable[mirror64[sq64(sq)]];
    }

    pce = wN;	
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score += knightTable[sq64(sq)];
    }	

    pce = bN;	
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score -= knightTable[mirror64[sq64(sq)]];
    }			
	
    pce = wB;	
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score += bishopTable[sq64(sq)];
    }	

    pce = bB;	
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score -= bishopTable[mirror64[sq64(sq)]];
    }	

    pce = wR;	
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score += rookTable[sq64(sq)];
    }	

    pce = bR;	
    for(pce_num = 0; pce_num < board.pceNum[pce]; ++pce_num) {
        sq = board.pceList[pce][pce_num];
        assert(sqOnBoard(sq));
        score -= rookTable[mirror64[sq64(sq)]];
    }	

    if (board.side == WHITE)
        return score;
    else
        return -score;
}
