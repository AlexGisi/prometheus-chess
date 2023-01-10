//
// Created by Alex Gisi on 6/7/22.
//

#include <ios>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <array>
#include <optional>
#include <stdexcept>
#include <fstream>
#include "util.h"
#include "Defs.h"

/*
 * Return functional components of a line in the perft results (.epd)
 * file in a vector: fen, depth one result, ..., depth 6 result.
 *
 * Ref: https://stackoverflow.com/questions/10058606/splitting-a-string-by-a-character
 */
std::vector<std::string> parse_epd_line(const std::string& line) {
    std::stringstream ss(line);
    std::string segment;
    std::vector<std::string> res;

    while(std::getline(ss, segment, ';')) {
        if (segment[0] == 'D') {
            res.push_back(segment.substr(3));
        } else {
            res.push_back(segment);
        }
    }

    return res;
}

/*
 * Ref: https://stackoverflow.com/a/56107709/5127535
 */
u64 get_time() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void readTable(std::fstream & infile, std::optional<std::array<int, 64>> & table) {
    table = ZERO_PST;
    std::string tok;
    for (int i=0; i < 64; i++) {
        infile >> tok;
        table->at(i) = std::stoi(tok);
    }
}

OptionalPieceSquareTables get_psts(std::string file_name) {
    std::fstream infile;
    std::string token;
    OptionalPieceSquareTables ret;

    infile.open(file_name, std::ios::in);
    if(infile.is_open()) {
        while(infile >> token) {
            if (token == "pawns")
                readTable(infile, ret.pawns);
            else if (token == "knights")
                readTable(infile, ret.knights);
            else if (token == "bishops")
                readTable(infile, ret.bishops);
            else if (token == "rooks")
                readTable(infile, ret.rooks);
        }
    } else {
        throw std::runtime_error("Could not open PieceSquareTable file.");
    }

    return ret;
}
