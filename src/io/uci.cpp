//
// Created by Alex Gisi on 7/6/22.
// Ref: Stockfish uci.h
//

#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include "../defs.h"
#include "../board/board.h"
#include "../search/search.cpp"
#include "uci.h"
#include "../move/move.h"

/*
 * The main I/O loop for gameplay with the UCI protocol.
 * See https://github.com/official-stockfish/Stockfish/blob/master/src/uci.cpp
 * for reference.
 */
void uci_loop() {
    std::string cmd, token;
    bool quit = false;

    Board board;
    SearchInfo info;


    do {
        std::getline(std::cin, cmd);
        std::istringstream is(cmd);

        token.clear();
        is >> std::skipws >> token;

        if (token == "uci") {
            std::cout << "id name " << NAME << ' ' << VERSION_MAJOR << '.' << VERSION_MINOR << std::endl;
            std::cout << "id author " << AUTHOR << std::endl;

            std::cout << "option name Hash type spin default 64 min 1 max 4096" << std::endl;

            std::cout << "uciok" << std::endl;
        }
        else if (token == "isready")
            std::cout << "readyok" << std::endl;
        else if (token == "position")
            uci_parse_pos(board, std::move(is));
        else if (token == "setoption")
            uci_parse_setoption(board, std::move(is));
        else if (token == "ucinewgame") {
            std::string POSITION_STARTPOS = "startpos";
            std::istringstream isng(POSITION_STARTPOS);
            uci_parse_pos(board, std::move(isng));
        }
        else if (token == "go")
            uci_parse_go(board, std::move(is), info);
        else if (token == "stop")
            info.stopped = true;
        else if (token == "quit")
            quit = true;
        } while (!quit);
}

void uci_parse_go(Board &board, std::istringstream is, SearchInfo &info) {
    std::string token;
    int depth = -1, alpha = 30, movetime = -1;
    int time = -1, inc = 0;
    info.time_set = false;

    while (is >> token) {
        if (token == "binc" && board.side == BLACK) {
            is >> token;
            inc = std::stoi(token);
        }
        else if (token == "winc" && board.side == WHITE) {
            is >> token;
            inc = std::stoi(token);
        }
        else if (token == "wtime" && board.side == WHITE) {
            is >> token;
            time = std::stoi(token);
        }
        else if (token == "btime" && board.side == BLACK) {
            is >> token;
            time = std::stoi(token);
        }
        else if (token == "movestogo") {
            is >> token;
            alpha = std::stoi(token); // moves until next time control
        }
        else if (token == "movetime") {
            is >> token;
            movetime = std::stoi(token); // search exactly movetime seconds
        }
        else if (token == "depth") {
            is >> token;
            depth = std::stoi(token); // search only up to depth plies
        }
    }

    if (movetime != -1) {
        time = movetime;
        alpha = 1;
    }

    info.start_time = get_time();

    if (time != -1) {
        info.time_set = true;
        time /= alpha;
        time -= 25;  // Cushion to be safe.
        info.stop_time = info.start_time + static_cast<unsigned long long int>(time) +
                         static_cast<unsigned long long int>(inc);
    }

    info.depth = (depth == -1) ? MAX_DEPTH : depth;

    printf("info string time:%d start:%llu stop:%llu depth:%d timeset:%d",
           time,info.start_time,info.stop_time,info.depth,info.time_set);
    std::cout << std::endl;

    std::thread (search, std::ref(board), std::ref(info)).detach();
}

void uci_parse_pos(Board &board, std::istringstream is) {
    Move m;
    std::string token, fen;

    is >> token;
    if (token == "startpos") {
        fen = START_FEN;
        is >> token;
    } else if (token == "fen") {
        while (is >> token && token != "moves")
            fen += token + " ";
    } else
        return;

    board.initialize(fen);

    while (is >> token) {
        m = Move::from_str(token, board);
        board.make_move(m);
        board.ply = 0;
    }
}

void uci_parse_setoption(Board &board, std::istringstream is) {
    std::string token;
    is >> token;  // Skip "name".
    is >> token;
    if (token == "Hash") {
        is >> token;  // Hash size in MB.
        board.resize_pv_table(std::stoul(token) * 0x100000);
    }
}


