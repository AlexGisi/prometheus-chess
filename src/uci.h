//
// Created by Alex Gisi on 7/6/22.
//

#ifndef BLUE_UCI_H
#define BLUE_UCI_H

void uci_intro();
void uci_loop();
void uci_parse_go(Board &board, std::istringstream &is, SearchInfo &info);
void uci_parse_pos(Board& board, std::istringstream& is);

#endif //BLUE_UCI_H
