//
// Created by Alex Gisi on 6/18/22.
//

#include <iostream>
#include <cassert>
#include "Board.h"
#include "util.cpp"
#include "evaluation.cpp"
#include "MoveGen.h"

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
    float fhf;  // Fail high first.
} SearchInfo;

/*
 * Check if search time up or interrupt from GUI.
 */
inline void checkInterrupt(SearchInfo& info) {
    if(info.time_set == true && get_time() > info.stop_time)
        info.stopped = true;
}

/*
 * Does everything but set the start time of the search.
 */
inline void prepSearch(Board& board, SearchInfo& info) {
    board.prep_search();

    info.stopped = false;
    info.nodes = 0;
    info.fh = 0;
    info.fhf = 0;
}

/*
 * Searching from the index move_idx, switch the move at move_idx with the best
 * move in ml.
 */
inline void pickNextMove(int move_idx, MoveListPtr& ml) {
    SearchMove temp;
    int best_score = 0;
    int best_idx = move_idx;

    for(int i=move_idx; i < ml->size(); i++) {
        if(ml->at(i).score > best_score) {
            best_score = ml->at(i).score;
            best_idx = i;
        }
    }
    temp = (*ml)[move_idx];
    (*ml)[move_idx] = (*ml)[best_idx];
    (*ml)[best_idx] = temp;
}

/*
 * Search just until a quiet position, avoiding the horizon effect.
 * TODO: consider other factors.
 */
inline int quiescence(int alpha, int beta, Board& board, SearchInfo &info) {
    assert(board.check_board());

    if(info.nodes % 2048 == 0 )
        checkInterrupt(info);

    info.nodes++;

    if(board.ply > MAX_DEPTH - 1)
        return eval(board);

    int score = eval(board);

    if(score >= beta)
        return beta;

    if(score > alpha)
        alpha = score;

    MoveGen mg(&board);
    MoveListPtr ml = mg.generateAllCaps();

    int legal = 0;
    int old_alpha = alpha;
    Move best_move;
    score = -INFINITE;
    std::optional<Move> pv_move = board.pvTable.probe(board.posKey);

    for(int i = 0; i < ml->size(); i++) {
        pickNextMove(i, ml);

        if(!board.make_move(ml->at(i).move))
            continue;

        legal++;
        score = -quiescence(-beta, -alpha, board, info);
        board.take_move();

        if(info.stopped)
            return 0;

        if(score > alpha) {
            if(score >= beta) {
                if(legal == 1)
                    info.fhf++;
                info.fh++;

                return beta;
            }
            alpha = score;
            best_move = ml->at(i).move;
        }
    }

    if(alpha != old_alpha)
        board.pvTable.store(board.posKey, best_move);

    return alpha;
}

inline int alphaBeta(int alpha, int beta, int depth, Board& board, SearchInfo& info, int doNull) {
    assert(board.check_board());

    if(depth == 0) {
        return quiescence(alpha, beta, board, info);
    }

    if(info.nodes % 2048 == 0 )
        checkInterrupt(info);

    info.nodes++;

    if(board.is_repetition() || board.fiftyMove >= 100)
        return 0;

    if(board.ply > MAX_DEPTH - 1)
        return eval(board);

    MoveGen mg(&board);
    MoveListPtr ml = mg.generateAllMoves();

    int legal = 0;
    int old_alpha = alpha;
    Move best_move;
    int score = -INFINITE;
    std::optional<Move> pv_move = board.pvTable.probe(board.posKey);

    // Ensure we search the PV line first.
    if(pv_move) {
        for(auto & i : *ml) {
            if(i.move == pv_move.value()) {
                i.score = 2000000;
                break;
            }
        }
    }

    for(int i = 0; i < ml->size(); i++) {
        pickNextMove(i, ml);

        if(!board.make_move(ml->at(i).move))
            continue;

        legal++;
        score = -alphaBeta(-beta, -alpha, depth-1, board, info, true);
        board.take_move();

        if(info.stopped)
            return 0;

        if(score > alpha) {
            if(score >= beta) {
                if(legal == 1)
                    info.fhf++;
                info.fh++;

                if(!ml->at(i).move.capture()) {
                    board.searchKillers[1][board.ply] = board.searchKillers[0][board.ply];
                    board.searchKillers[0][board.ply] = ml->at(i).move;
                }

                return beta;
            }
            alpha = score;
            best_move = ml->at(i).move;

            if(!ml->at(i).move.capture()) {
                board.searchHistory[board.pieces[best_move.from()]][best_move.to()] += depth;
            }
        }
    }

    if(legal == 0) {
        if(board.sq_attacked(board.kingSq[board.side], board.side ^ 1))
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

        if(info.stopped)
            break;

        pv_moves = board.update_pv_line(current_depth);
        best_move = board.pvArray[0];

        if (current_depth > 50) {
            std::cout << std::endl;
        }
        printf("info score cp %d depth %d nodes %llu time %llu ",
               best_score, current_depth, info.nodes, get_time()-info.start_time);
        printf("pv");
        for(pv_num = 0; pv_num < pv_moves; pv_num++) {
            printf(" %s", board.pvArray[pv_num].to_str().c_str());
        }
        std::cout << std::endl;
        // printf("Ordering:%.2f\n", info.fhf / info.fh);
    }
    // printf("bestmove %s\n", best_move.to_str().c_str());
    std::cout << "bestmove " << best_move.to_str() << std::endl;
    // assert(board.make_move(best_move));
}
