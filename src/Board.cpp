//
// Created by Alex Gisi on 2/17/22.
//

#include <iostream>
#include <cctype>
#include <sstream>
#include "Board.h"

Board::Board() {
    setUpEmpty();
};

void Board::setUpEmpty() {
    for(int & piece : pieces)
        piece = OFFBOARD;

    for(int i : sq64ToSq120)
        pieces[i] = EMPTY;

    for(BitBoard & bb : pawns)
        bb = BitBoard();

    kingSq[0] = NO_SQ;
    kingSq[1] = NO_SQ;

    side = WHITE;
    enPas = 0;
    fiftyMove = 0;

    ply = 0;
    hisPlay = 0;

    for(int & num : pceNum)
        num = 0;
    for(int & num : bigPce)
        num = 0;
    for(int & num : majPce)
        num = 0;
    for(int & num : minPce)
        num = 0;
    for(int & num : material)
        num = 0;

    castlePerm = 0;

    for(auto & piece : pceList) {
        for(int & p : piece)
            p = NO_SQ;
    }

    posKey = PosKey(this);
}

// TODO
Board::Board(const Board &rhs) {
    setUpEmpty();

    for(int i=0; i < BRD_SQ_NUM; ++i)
        pieces[i] = rhs.pieces[i];

    for(int i=0; i < 3; ++i)
        pawns[i] = rhs.pawns[i];

    kingSq[0] = rhs.kingSq[0];
    kingSq[1] = rhs.kingSq[1];

    side = rhs.side;
    enPas = rhs.enPas;
    fiftyMove = rhs.fiftyMove;

    ply = rhs.ply;
    hisPlay = rhs.hisPlay;

    for(int i=0; i < 13; ++i)
        pceNum[i] = rhs.pceNum[i];

    for(int i=0; i < 2; ++i) {
        bigPce[i] = rhs.bigPce[i];
        majPce[i] = rhs.majPce[i];
        minPce[i] = rhs.minPce[i];
        material[i] = rhs.material[i];
    }

    castlePerm = rhs.castlePerm;

    for(int i=0; i < 13; ++i) {
        for(int j=0; j < 10; ++j)
            pceList[i][j] = rhs.pceList[i][j];
    }

    posKey = rhs.posKey;
};

Board &Board::operator=(const Board &rhs) {
    // This self-assignment check is considered bad design.
    if(&rhs == this)
        return *this;

    setUpEmpty();

    for(int i=0; i < BRD_SQ_NUM; ++i)
        pieces[i] = rhs.pieces[i];

    for(int i=0; i < 3; ++i)
        pawns[i] = rhs.pawns[i];

    kingSq[0] = rhs.kingSq[0];
    kingSq[1] = rhs.kingSq[1];

    side = rhs.side;
    enPas = rhs.enPas;
    fiftyMove = rhs.fiftyMove;

    ply = rhs.ply;
    hisPlay = rhs.hisPlay;

    for(int i=0; i < 13; ++i)
        pceNum[i] = rhs.pceNum[i];

    for(int i=0; i < 2; ++i) {
        bigPce[i] = rhs.bigPce[i];
        majPce[i] = rhs.majPce[i];
        minPce[i] = rhs.minPce[i];
        material[i] = rhs.material[i];
    }

    castlePerm = rhs.castlePerm;

    for(int i=0; i < 13; ++i) {
        for(int j=0; j < 10; ++j)
            pceList[i][j] = rhs.pceList[i][j];
    }

    posKey = rhs.posKey;

    return *this;
}

// Not robust to incorrect fen strings.
Board::Board(const std::string& fen) {
    setUpEmpty();

    int rank = RANK_8;
    int file = FILE_A;
    int piece = 0;
    int count = 0;
    int i = 0;
    char c = '\0';
    std::istringstream ss(fen);
    int sq64 = 0;
    int sq120 = 0;

    ss >> std::noskipws;  // By default, >> skips white space, so change that.

    // Piece placement.
    while ((ss >> c) && !std::isspace(c)) {
        count = 1;
        switch(c) {
            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'k': piece = bK; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'K': piece = wK; break;
            case 'Q': piece = wQ; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                piece = EMPTY;
                count = c - '0';
                break;

            case '/':
                rank--;
                file = FILE_A;
                continue;

            default:
                ; // TODO: throw error if another character appears.
        }

        for(i = 0; i < count; i++) {
            sq64 = rank*8 + file;
            sq120 = sq64ToSq120[sq64];
            if(piece != EMPTY)
                pieces[sq120] = piece;
            file++;
        }
    }

    ss >> c;
    assert(c == 'w' || c == 'b');
    side = (c == 'w') ? WHITE : BLACK;
    ss >> c; // Space.

    castlePerm = 0;
    while((ss >> c) && !std::isspace(c)) {
        switch (c) {
            case 'K': castlePerm |= WKCA; break;
            case 'Q': castlePerm |= WQCA; break;
            case 'k': castlePerm |= BKCA; break;
            case 'q': castlePerm |= BQCA; break;
            case '-': break;
            default:
                std::cout << "Error: read wrong character in castling string: " << c << std::endl; // TODO: throw.
        }
    }
    ss >> c; // Space.

    // En passant square.
    if(c != '-') {
        file = c - 'a';
        ss >> c;
        rank = c - '1';

        assert(file >= FILE_A && file <= FILE_H);
        assert(rank >= RANK_1 && rank <= RANK_8);

        enPas = FR2SQ(file, rank);
    } else {
        enPas = NO_SQ;
    }


    // Move/halfmove clocks.
    // TODO.

    // Other initialization.
    posKey = PosKey(this);
    updateListsMaterial();
}

void Board::initSq120To64() {
    int index = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq64 = 0;

    // Initialize array values.
    for(index = 0; index < 64; ++index) {
        sq120ToSq64[index] = 65;
    }
    for(index = 0; index < 64; ++index) {
        sq64ToSq120[index] = 120;
    }

    for(rank = RANK_1; rank <= RANK_8; ++rank) {
        for(file = FILE_A; file <= FILE_H; ++file) {
            int sq120 = FR2SQ(file, rank);
            sq64ToSq120[sq64] = sq120;
            sq120ToSq64[sq120] = sq64;
            sq64++;
        }
    }
}

void Board::reset() {
    *this = Board();
}

void Board::print() {
    int sq, file, rank, piece;
    for(rank = RANK_8; rank >= RANK_1; rank--) {
        std::cout << rank+1 << ' ';
        for(file = FILE_A; file <= FILE_H; file++) {
            sq = FR2SQ(file, rank);
            piece = pieces[sq];
            printf("%3c", pceChar[piece]);
        }
        std::cout << std::endl;
    }

    std::cout << "\n  ";
    for(file = FILE_A; file <= FILE_H; file++) {
        printf("%3c", 'a'+file);
    }
    std::cout << std::endl;

    printf("side:%c\n", sideChar[side]);
    printf("enPas:%d\n", enPas);
    printf("castle:%c%c%c%c\n",
           (castlePerm & WKCA) ? 'K' : '-',
           (castlePerm & WQCA) ? 'Q' : '-',
           (castlePerm & BKCA) ? 'k' : '-',
           (castlePerm & BQCA) ? 'q' : '-'
           );
    printf("PosKey:%llX\n", posKey.val);
}

void Board::updateListsMaterial() {
    int piece, sq, col, idx;
    for(idx = 0; idx < BRD_SQ_NUM; ++idx) {
        sq = idx;
        piece = pieces[idx];
        if(piece != OFFBOARD && piece != EMPTY) {
            col = pieceCol[piece];

            if(pieceBig[piece]) bigPce[col]++;
            if(pieceMin[piece]) minPce[col]++;
            if(pieceMaj[piece]) majPce[col]++;

            material[col] += pieceVal[piece];

            pceList[piece][pceNum[piece]] = sq;
            pceNum[piece]++;

            if(piece == wK) kingSq[WHITE] = sq;
            if(piece == bK) kingSq[BLACK] = sq;

            if(piece == wP) {
                pawns[WHITE].setBit(sq120ToSq64[sq]);
                pawns[BOTH].setBit(sq120ToSq64[sq]);
            } else if(piece == bP) {
                pawns[BLACK].setBit(sq120ToSq64[sq]);
                pawns[BOTH].setBit(sq120ToSq64[sq]);
            }
        }
    }
}

void Board::initFilesRanksBrd() {
    int idx = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;
    int sq64 = 0;

    for(idx = 0; idx < BRD_SQ_NUM; ++idx) {
        filesBrd[idx] = OFFBOARD;
        ranksBrd[idx] = OFFBOARD;
    }

    for(rank = RANK_1; rank <= RANK_8; ++rank) {
        for(file = FILE_A; file <= FILE_H; ++file) {
            sq = FR2SQ(file, rank);
            filesBrd[sq] = file;
            ranksBrd[sq] = rank;
        }
    }
}

// Asserts accuracy of board-tracking arrays. Useful after series of captures or undoes.
// Always returns true or throws an error.
bool Board::checkBoard() const {
    // Temporary arrays.
    int t_pceNum[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int t_bigPce[2] = { 0, 0 };
    int t_majPce[2] = { 0, 0 };
    int t_minPce[2] = { 0, 0 };
    int t_material[2] = { 0, 0 };

    int sq64, t_piece, t_pce_num, sq120, color, p_count;
    BitBoard t_pawns[3] = { BitBoard(), BitBoard(), BitBoard() };

    t_pawns[WHITE] = pawns[WHITE];
    t_pawns[BLACK] = pawns[BLACK];
    t_pawns[BOTH] = pawns[BOTH];

    // Check piece lists.
    for(t_piece = wP; t_piece <= bK; ++t_piece) {
        for(t_pce_num = 0; t_pce_num < pceNum[t_piece]; ++t_pce_num) {
            sq120 = pceList[t_piece][t_pce_num];
            assert(pieces[sq120] == t_piece);
        }
    }

    // Check piece count and other counters.
    for(sq64 = 0; sq64 < 64; ++sq64) {
        sq120 = sq64ToSq120[sq64];
        t_piece = pieces[sq120];
        t_pceNum[t_piece]++;
        color = pieceCol[t_piece];
        if(pieceBig[t_piece]) t_bigPce[color]++;
        if(pieceMin[t_piece]) t_minPce[color]++;
        if(pieceMaj[t_piece]) t_majPce[color]++;

        t_material[color] += pieceVal[t_piece];
    }

    for(t_piece = wP; t_piece <= bK; ++t_piece)
        assert(t_pceNum[t_piece] == pceNum[t_piece]);

    // Check bitboard counts.
    p_count = t_pawns[WHITE].countBits();
    assert(p_count == pceNum[wP]);
    p_count = t_pawns[BLACK].countBits();
    assert(p_count == pceNum[bP]);
    p_count = t_pawns[BOTH].countBits();
    assert(p_count == (pceNum[wP] + pceNum[bP]));

    // Check bitboard squares.
    while(t_pawns[WHITE]) {
        sq64 = t_pawns[WHITE].popBit();
        assert(pieces[sq64ToSq120[sq64]] == wP);
    }
    while(t_pawns[BLACK]) {
        sq64 = t_pawns[BLACK].popBit();
        assert(pieces[sq64ToSq120[sq64]] == bP);
    }
    while(t_pawns[BOTH]) {
        sq64 = t_pawns[BOTH].popBit();
        assert( (pieces[sq64ToSq120[sq64]] == wP) || (pieces[sq64ToSq120[sq64]] == bP) );
    }

    // Additional sanity checks.
    assert(t_material[WHITE]==material[WHITE] && t_material[BLACK]==material[BLACK]);
    assert(t_minPce[WHITE]==minPce[WHITE] && t_minPce[BLACK]==minPce[BLACK]);
    assert(t_majPce[WHITE]==majPce[WHITE] && t_majPce[BLACK]==majPce[BLACK]);
    assert(t_bigPce[WHITE]==bigPce[WHITE] && t_bigPce[BLACK]==bigPce[BLACK]);

    assert(side==WHITE || side==BLACK);
    assert(PosKey(this) == posKey);

    assert(enPas==NO_SQ || (ranksBrd[enPas]==RANK_6 && side==WHITE)
        || (ranksBrd[enPas]==RANK_3 && side==BLACK));

    assert(pieces[kingSq[WHITE]] == wK);
    assert(pieces[kingSq[BLACK]] == bK);

    return true;
}

// Determine if a square is attacked by a side.
bool Board::sqAttacked(const int sq, const int att_side) const {
    int pce, idx, t_sq, dir = 0;

    // Pawns.
    if(att_side == WHITE) {
        if(pieces[sq-11] == wP || pieces[sq-9] == wP)
            return true;
    } else {
        if(pieces[sq+11] == bP || pieces[sq+9] == bP)
            return true;
    }

    // Knights.
    for(idx = 0; idx < 8; ++idx) {
        pce = pieces[sq+knDir[idx]];
        if(pce != OFFBOARD && pce != EMPTY && isKn(pce) && pieceCol[pce]==att_side)
            return true;
    }

    // Rooks, queens.
    for(idx = 0; idx < 4; ++idx) {
        dir = rkDir[idx];
        t_sq = sq + dir;
        pce = pieces[t_sq];
        while(pce != OFFBOARD) {
            if(pce != EMPTY) {
                if(isRQ(pce) && pieceCol[pce]==att_side)
                    return true;
                break;
            }
            t_sq += dir;
            pce = pieces[t_sq];
        }
    }

    // Bishops, queens.
    for(idx = 0; idx < 4; ++idx) {
        dir = biDir[idx];
        t_sq = sq + dir;
        pce = pieces[t_sq];
        while(pce != OFFBOARD) {
            if(pce != EMPTY) {
                if(isBQ(pce) && pieceCol[pce]==att_side)
                    return true;
                break;
            }
            t_sq += dir;
            pce = pieces[t_sq];
        }
    }

    // Kings.
    for(idx = 0; idx < 8; ++idx) {
        pce = pieces[sq+kiDir[idx]];
        if(pce != OFFBOARD && pce != EMPTY && isKi(pce) && pieceCol[pce]==att_side)
            return true;
    }

    return false;
}

// Testing function for sqAttacked.
void Board::showSqAttBySide(int att_side) const {
    int rank = 0;
    int file = 0;
    int sq = 0;

    printf("\n\nSquares attacked by:%c\n", sideChar[att_side]);
    for(rank = RANK_8; rank >= RANK_1; --rank) {
        for(file = FILE_A; file <= FILE_H; ++file) {
            sq = FR2SQ(file,rank);
            if(sqAttacked(sq, att_side)) {
                printf("X");
            } else {
                printf("-");
            }

        }
        printf("\n");
    }
    printf("\n\n");
}


int Board::sq120ToSq64[BRD_SQ_NUM];
int Board::sq64ToSq120[64];
int Board::filesBrd[BRD_SQ_NUM];
int Board::ranksBrd[BRD_SQ_NUM];
