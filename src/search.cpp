//
// Created by Alex Gisi on 6/18/22.
//

#include <cassert>
#include "Board.h"
#include "util.cpp"
#include "evaluation.cpp"
#include "MoveGen.h"
#include "MoveList.h"

typedef struct {
    u64 start_time;
    u64 stop_time;
    int depth;
    int depth_set;
    int time_set;
    int moves_to_go;
    bool infinite;

    u64 nodes;

    bool quit;
    bool stopped;

    float fh;   // Fail high.
    float fhf;  // Faily high first.
} SearchInfo;

/*
 * Check if search time up or interrupt from GUI.
 */
inline void checkInterrupt() {

}

/*
 * Detects existance of a single repetition. A repetition could only have happened since the
 * last time the fifty move rule counter was reset.
 */
inline bool hasRepetition(Board& board) {
    for(int i = 0; i < board.hisPly-board.fiftyMove; i++)
        if(board.posKey == board.history[i].posKey)
            return true;

    return false;
}

inline void prepSearch(Board& board, SearchInfo& info) {
    for(auto & i : board.searchHistory) {
        for(int & j : i)
            j = 0;
    }

    for(auto & i : board.searchKillers) {
        for(auto & j : i)
            j = 0;
    }

    board.pvTable.clear();
    board.ply = 0;

    info.start_time = get_time();
    info.stopped = false;
    info.nodes = 0;
    info.fh = 0;
    info.fhf = 0;
}

inline int quiescence(int alpha, int beta, Board& board, SearchInfo* info) {
    return 0;
}

inline int alphaBeta(int alpha, int beta, int depth, Board& board, SearchInfo& info, int doNull) {
    assert(board.checkBoard());

    info.nodes++;
    if(depth == 0) {
        return eval(board);
    }

    if(hasRepetition(board) || board.fiftyMove >= 100)
        return 0;

    if(board.ply > MAX_DEPTH - 1)
        return eval(board);

    MoveList ml;
    MoveGen mg(board);
    mg.generateAllMoves(&ml);

    int legal = 0;
    int old_alpha = alpha;
    Move best_move;
    int score = -INFINITE;

    for(int i = 0; i < ml.count; i++) {
        if(!board.makeMove(ml.moves[i].move))
            continue;

        legal++;
        score = -alphaBeta(-beta, -alpha, depth-1, board, info, true);
        board.takeMove();

        if(score > alpha) {
            if(score >= beta) {
                if(legal == 1)
                    info.fhf++;
                info.fh++;
                return beta;
            }
            alpha = score;
            best_move = ml.moves[i].move;
        }
    }

    if(legal == 0) {
        if(board.sqAttacked(board.kingSq[board.side], board.side^1))
            return -MATE + board.ply;
        else
            return 0;
    }

    if(alpha != old_alpha)
        board.pvTable.store(board.posKey, best_move);

    return alpha;
}

/*
 * Iterative deepening with alpha-beta minimax.
 */
inline void search(Board& board, SearchInfo& info) {
    Move best_move;
    int best_score = -INFINITE;
    int current_depth = 0;
    int pv_moves = 0;
    int pv_num = 0;

    prepSearch(board, info);

    for(current_depth = 1; current_depth <= info.depth; ++current_depth) {
        best_score = alphaBeta(-INFINITE, INFINITE, current_depth, board, info, true);

        // Out of time?

        pv_moves = board.getPVLine(current_depth);
        best_move = board.pvArray[0];

        // DEBUG
        printf("Depth:%d score:%d move:%s nodes:%llu ",
               current_depth, best_score, best_move.to_str().c_str(), info.nodes);
        printf("PV:");
        for(pv_num = 0; pv_num < pv_moves; pv_num++) {
            Move mv = board.pvArray[pv_num];
            printf(" %s", mv.to_str().c_str());
        }
        printf("\n");
        printf("Ordering:%.2f\n", info.fhf / info.fh);
    }
}
