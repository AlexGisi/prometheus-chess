//
// Created by Alex Gisi on 2/17/22.
// For board representation explanation see:
// https://www.youtube.com/watch?v=x9sPmLt-EBM&list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg&index=3
//

#ifndef BLUE_DEFS_H
#define BLUE_DEFS_H

#define NAME "blue"
#define VERSION_MAJOR "0"
#define VERSION_MINOR "1"

typedef unsigned long long u64;

#define BRD_SQ_NUM 120
#define MAX_GAME_MOVES 2048
#define MAX_POSITION_MOVES 256

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// Useful for printing the board.
const char pceChar[] = ".PNBRQKpnbrqk";
const char sideChar[] = "wb-";
const char rankChar[] = "12345678";
const char fileChar[] = "abcdefgh";

// Basic piece and board enums.
enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };
enum { WHITE, BLACK, BOTH };
enum {
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD
};

enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

const int BitTable[64] = {
        63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
        51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
        26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
        58, 20, 37, 17, 36, 8
};

// Info about pieces.
const int pieceBig[13] = { false, false, true, true, true, true, true, false, true, true, true, true, true };
const int pieceMaj[13] = { false, false, false, false, true, true, true, false, false, false, true, true, true };
const int pieceMin[13] = { false, false, true, true, false, false, false, false, true, true, false, false, false };
const int pieceVal[13] = { 0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000 };
const int pieceCol[13] = { BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                           BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };

// How pieces can move.
const int knDir[8] = { -8, -19, -21, -12, 8, 19, 21, 12 };
const int rkDir[4] = { -1, -10, 1, 10 };
const int biDir[4] = { -9, -11, 11, 9 };
const int kiDir[8] = { -1, -10, 1, 10, -9, -11, 11, 9};

// Move flags.
#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGCA 0x1000000

#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000

// Is given piece a certain type.
const int pieceKnight[13] = { false, false, true, false, false, false, false, false, true, false, false, false, false };
const int pieceKing[13] = { false, false, false, false, false, false, true, false, false, false, false, false, true };
const int pieceRookQueen[13] = { false, false, false, false, true, true, false, false, false, false, true, true, false };
const int pieceBishopQueen[13] = { false, false, false, true, false, true, false, false, false, true, false, true, false };
const int pieceSlides[13] = { false, false, false, true, true, true, false, false, false, true, true, true, false };
const int piecePawn[13] = { false, true, false, false, false, false, false, true, false, false, false, false, false };

// Castle permission: used for bitwise or with board.castlePerm.
const int castlePermArr[120] = {
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

/* MACROS */
#define FR2SQ(f, r) ( (21 + (f)) + ((r) * 10) )  // For a given file and rank, return the 120-based square number.
#define isBQ(p) (pieceBishopQueen[(p)])
#define isRQ(p) (pieceRookQueen[(p)])
#define isKn(p) (pieceKnight[(p)])
#define isKi(p) (pieceKing[(p)])

#endif //BLUE_DEFS_H
