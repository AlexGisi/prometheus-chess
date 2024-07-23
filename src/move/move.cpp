//
// Created by Alex Gisi on 3/15/22.
//

#include <cassert>
#include <sstream>
#include "move.h"
#include "../board/board.h"
#include "movegen.h"

Move::Move() {
    move = 0;
}

Move::Move(uint32_t m) {
    move = m;
}

Move::Move(int from, int to, int cap, int pro, int f1) {
    move = static_cast<uint32_t>(from | (to << 7) | (cap << 14) | (pro << 20 | f1));
}

Move Move::from_str(const std::string &move_str, Board &board) {
    if (move_str[1] > '8' || move_str[1] < '1' || move_str[3] > '8' || move_str[3] < '1' || move_str[0] > 'h'
    || move_str[0] < 'a' || move_str[2] > 'h' || move_str[2] < 'a' || move_str.length() < 4 || move_str.length() > 6)
        throw std::invalid_argument("Bad string input");

    int from = FR2SQ(move_str[0] - 'a', move_str[1] - '1');
    int to = FR2SQ(move_str[2] - 'a', move_str[3] - '1');

    assert(Board::is_on_board(from) && Board::is_on_board(to));

    MoveGen mg(&board);
    MoveListPtr list = mg.generate_all_moves();
    for (auto & i : *list) {
        Move m = i.move;
        if(m.from() == from && m.to() == to) {
            int pro = m.promoted();
            if(pro != EMPTY) {
                if(isRQ(pro) && !isBQ(pro) && move_str[4] == 'r') {
                    return m;
                }
                else if(!isRQ(pro) && isBQ(pro) && move_str[4] == 'b') {
                    return m;
                }
                else if(isRQ(pro) && isBQ(pro) && move_str[4] == 'q') {
                    return m;
                }
                else if(isKn(pro) && move_str[4] == 'n') {
                    return m;
                }
                continue;
            }
            return m;
        }
    }
    throw std::invalid_argument("Bad string input");
}

Move::Move(const Move &rhs) {
    move = rhs.move;
}

Move& Move::operator=(const Move& rhs) = default;

std::string Move::to_str() const {
    std::stringstream ss;
    std::string s;

    char ff = char(Board::filesBrd[from()]+'a');
    char rf = char(Board::ranksBrd[from()]+'1');
    char ft = char(Board::filesBrd[to()]+'a');
    char rt = char(Board::ranksBrd[to()]+'1');

    ss << ff << rf << ft << rt;

    int promoted = this->promoted();
    if(promoted) {
        char pchar = 'q';
        if(isKn(promoted))
            pchar = 'n';
        else if(isRQ(promoted) && !isBQ(promoted))
            pchar = 'r';
        else if(!isRQ(promoted) && isBQ(promoted))
            pchar = 'b';
        ss << pchar;
    }

    ss >> s;

    return s;
}

bool Move::operator==(const Move &rhs) const {
    return move == rhs.move;
}
