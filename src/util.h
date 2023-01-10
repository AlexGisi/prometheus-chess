//
// Created by Alex Gisi on 1/9/23.
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
#include "Defs.h"

#ifndef ATHENE_UTIL_H
#define ATHENE_UTIL_H

struct OptionalPieceSquareTables{
    std::optional<std::array<int, 64>> pawns;
    std::optional<std::array<int, 64>> knights;
    std::optional<std::array<int, 64>> bishops;
    std::optional<std::array<int, 64>> rooks;
};

std::vector<std::string> parse_epd_line(const std::string& line);
u64 get_time();
void readTable(std::fstream & infile, std::optional<std::array<int, 64>> & table);
OptionalPieceSquareTables get_psts(std::string file_name);

#endif //ATHENE_UTIL_H
