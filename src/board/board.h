//
// Created by Alex Gisi on 2/17/22.
//

#ifndef ATHENE_BOARD_H
#define ATHENE_BOARD_H

#include <string>
#include "bitboard.h"
#include "../defs.h"
#include "../hashing/poskey.h"
#include "../move/move.h"
#include "../hashing/pvtable.h"

class Board {
public:
    Board();  // Create an empty board.
    explicit Board(const std::string& fen);

    Board(const Board& rhs);
    Board& operator=(const Board& rhs);

    void initialize();
    void initialize(const std::string& fen);
    void reset();

    // Lookup tables.
    static int sq120ToSq64[BRD_SQ_NUM];
    static int sq64ToSq120[64];
    static int filesBrd[BRD_SQ_NUM];
    static int ranksBrd[BRD_SQ_NUM];
    #define sq64(sq) Board::sq120ToSq64[sq]
    #define sq120(sq) Board::sq64ToSq120[sq]

    static void initialize_lookup_tables();

    [[nodiscard]] bool check_board() const;
    [[nodiscard]] bool sq_attacked(int sq, int att_side) const;
    void print_squares_attacked(int side) const;

    static std::string sq_to_str(int sq);
    [[nodiscard]] std::string to_str() const;
    void print() const;

    // Assign a piece (could be EMPTY) to every square on the 120-piece board.
    int pieces[BRD_SQ_NUM];
    BitBoard pawns[3];  // WHITE, BLACK, BOTH.
    int kingSq[2];
    int side;
    int enPas;
    int fiftyMove; // Counts plies towards the fifty-move rule.

    int ply; // Records depth during searches; starts at 0 for a given search.
    int hisPly;  // History.

    PosKey posKey;

    int pceNum[13];  // Number of pieces of each type.
    int bigPce[2];  // Number of 'big pieces', i.e. not pawns.
    int majPce[2];  // Number of 'major pieces', i.e. rooks and queens.
    int minPce[2];  // Number of 'minor pieces', bishops and knights.
    int material[2];  // Sum of material values for white and black's pieces.

    int castlePerm;  // All castle permissions represented by four bits.

    typedef struct {
        // Move about to be played.
        Move move;

        // States prior to move.
        int castlePerm;
        int enPas;
        int fiftyMove;
        PosKey posKey;
    } S_UNDO;

    S_UNDO history[MAX_GAME_MOVES];

    // Track positions for each piece, so we don't have to loop through pieces[] to
    // find piece positions.
    // Ex:
    // pceList[wN][0] = E1;
    // pceList[wN][1] = D4;
    // ...
    //
    int pceList[13][10];

    PVTable pvTable;
    Move pvArray[MAX_DEPTH];
    friend class PosKey;

    // Arrays to heuristically improve move ordering.
    int searchHistory[13][BRD_SQ_NUM];
    Move searchKillers[2][MAX_DEPTH];

    void prep_search();
    void resize_pv_table(size_t size);

    // Making moves.
    void clear_piece(int sq);
    void add_piece(int sq, int pce);
    void move_piece(int from, int to);
    bool make_move(Move& move);
    void take_move();

    // Perft.
    u64 perft(int depth);
    void perft_suite(int depth, const std::string& resultsfile);
    bool perft_eval_pos(int depth, const std::string& fen, const u64* correct);

    // Principal variation.
    int update_pv_line(int depth);

    // Board status.
    [[nodiscard]] bool is_repetition() const;
    [[nodiscard]] static bool is_on_board(int sq);
    [[nodiscard]] static bool is_off_board(int sq);
};


#endif //ATHENE_BOARD_H
