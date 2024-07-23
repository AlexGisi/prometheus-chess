//
// Created by Alex Gisi on 6/18/22.
//

#include <iostream>
#include <cassert>
#include "../board/board.h"
#include "../util/util.cpp"
#include "evaluation.cpp"
#include "../move/movegen.h"

typedef struct {
    uint64_t start_time;
    uint64_t stop_time;
    uint8_t depth;
    int depth_set;
    int time_set;
    int moves_to_go;
    bool infinite;

    uint64_t nodes;

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
inline void pickNextMove(size_t move_idx, MoveListPtr& ml) {
    SearchMove temp;
    int best_score = 0;
    size_t best_idx = move_idx;

    for(size_t i=move_idx; i < ml->size(); i++) {
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
 * TODO: consider other factors, i.e. checks.
 */
inline int quiescence(int alpha, int beta, Board& board, SearchInfo &info) {
    assert(board.check_board());

    if(info.nodes % 2048 == 0 )
        checkInterrupt(info);

    info.nodes++;

    if(board.is_repetition() || board.fiftyMove >= 100)
        return 0;

    if(board.ply > MAX_DEPTH - 1)
        return eval(board);

    int score = eval(board);

    if(score >= beta)
        return beta;

    if(score > alpha)
        alpha = score;

    MoveGen mg(&board);
    MoveListPtr ml = mg.generate_all_caps();

    int legal = 0;
    int old_alpha = alpha;
    Move best_move;
    score = -INFINITE;
    std::optional<PVTable::PVEntry> pv_move = board.pvTable.probe(board.posKey);

    // Ensure we search the PV line first.
    if(pv_move) {
        for(auto & i : *ml) {
            if(i.move == pv_move.value().move.move) {
                i.score = 2000000;
                break;
            }
        }
    }

    for(size_t i = 0; i < ml->size(); i++) {
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

    if (alpha != old_alpha) {
        board.pvTable.store(board.posKey, {board.posKey, {best_move, score}, 0, EntryFlag::HFNONE});
    }
    return alpha;
}

inline int alphaBeta(int alpha, int beta, uint8_t depth, Board& board, SearchInfo& info, int doNull) {
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

    std::optional<SearchMove> hash_move = board.pvTable.probe_move(board.posKey, alpha, beta, depth);
    if (hash_move) {
        return hash_move.value().score;
    }

    MoveGen mg(&board);
    MoveListPtr ml = mg.generate_all_moves();

    int legal = 0;
    int old_alpha = alpha;
    SearchMove best_move = SearchMove(Move(), -INFINITE);
    SearchMove current_move;
    int score = -INFINITE;
    std::optional<PVTable::PVEntry> pv_entry = board.pvTable.probe(board.posKey);

    // Ensure we search the PV line first.
    if(pv_entry) {
        for(auto & i : *ml) {
            if(i.move == pv_entry.value().move.move) {
                i.score = 2000000;
                break;
            }
        }
    }

    for(size_t i = 0; i < ml->size(); i++) {
        pickNextMove(i, ml);

        if(!board.make_move(ml->at(i).move))
            continue;

        legal++;
        score = -alphaBeta(-beta, -alpha, depth-1, board, info, true);
        board.take_move();
        current_move = SearchMove(ml->at(i).move, score);

        if(info.stopped)
            return 0;

        if (score > best_move.score) {
            best_move = current_move;

            if(score > alpha) {
                if(score >= beta) {
                    if(legal == 1)
                        info.fhf++;
                    info.fh++;

                    if(!current_move.move.capture()) {
                        board.searchKillers[1][board.ply] = board.searchKillers[0][board.ply];
                        board.searchKillers[0][board.ply] = ml->at(i).move;
                    }

                    board.pvTable.store(board.posKey,
                                        {board.posKey,
                                         {current_move.move, beta},
                                         depth,
                                         EntryFlag::HFBETA});

                    return beta;
                }
                alpha = score;

                if(!current_move.move.capture()) {
                    board.searchHistory[board.pieces[best_move.move.from()]][best_move.move.to()] += depth;
                }
            }
        }
    }

    if(legal == 0) {
        if(board.sq_attacked(board.kingSq[board.side], board.side ^ 1))
            return -SHORTEST_MATE + board.ply;
        else
            return 0;
    }

    if(alpha != old_alpha) {
        board.pvTable.store(board.posKey, {board.posKey, best_move, depth, EntryFlag::HFEXACT});
    } else{
        board.pvTable.store(board.posKey, {board.posKey, {best_move.move, alpha}, depth, EntryFlag::HFALPHA});
    }

    return alpha;
}

/*
 * Iterative deepening with alpha-beta minimax.
 */
inline void search(Board& board, SearchInfo& info) {
    Move best_move;
    int best_score = -INFINITE;
    uint8_t current_depth = 0;
    int pv_moves = 0;
    int pv_num = 0;

    prepSearch(board, info);

    for(current_depth = 1; current_depth <= info.depth; ++current_depth) {
        best_score = alphaBeta(-INFINITE, INFINITE, current_depth, board, info, true);

        // Need to be sure some move has been inserted into table before this.
        pv_moves = board.update_pv_line(current_depth);
        assert(pv_moves != 0);
        best_move = board.pvArray[0];

        // Be sure best_move has been populated before this hits.
        if(info.stopped)
            break;

        printf("info score cp %d depth %d nodes %llu time %llu ",
               best_score, current_depth, info.nodes, get_time()-info.start_time);
        printf("pv");
        for(pv_num = 0; pv_num < pv_moves; pv_num++) {
            printf(" %s", board.pvArray[pv_num].to_str().c_str());
        }
        std::cout << std::endl;

        if (best_score > MATE) {
            break;
        }

        // Measure of move ordering effectiveness; >0.9 is good
        // printf("Ordering:%.2f\n", info.fhf / info.fh);
    }
    std::cout << "bestmove " << best_move.to_str() << std::endl;
}
