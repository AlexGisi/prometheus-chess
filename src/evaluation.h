//
// Created by Alex Gisi on 1/6/23.
//

#include <array>
#include <optional>
#include "util.h"

#ifndef ATHENE_EVALUATION_H
#define ATHENE_EVALUATION_H

class PieceSquareTables {
private:
    /*
     * Default piece-square tables, in centipawns.
     */
    constexpr static const std::array<int, 64> defPawnTable = {
            0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
            10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
            5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
            0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
            5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
            10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
            20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
            0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
    };

    constexpr static const std::array<int, 64> defKnightTable = {
            0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
            0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
            0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
            0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
            5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
            5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
            0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
            0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
    };

    constexpr static const std::array<int, 64> defBishopTable = {
            0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
            0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
            0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
            0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
            0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
            0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
            0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
            0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
    };

    constexpr static const std::array<int, 64> defRookTable = {
            0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
            0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
            0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
            0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
            0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
            0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
            25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
            0	,	0	,	5	,	10	,	10	,	5	,	0	,	0
    };

public:
    static std::array<int, 64> pawnTable;
    static std::array<int, 64> knightTable;
    static std::array<int, 64> bishopTable;
    static std::array<int, 64> rookTable;

    static void initPieceSquareTables(std::optional<std::string> replace) {
        if (replace.has_value()) {
            OptionalPieceSquareTables in_psts = get_psts(replace.value());
            if (in_psts.pawns.has_value())
                pawnTable = in_psts.pawns.value();
            if (in_psts.knights.has_value())
                pawnTable = in_psts.knights.value();
            if (in_psts.bishops.has_value())
                pawnTable = in_psts.bishops.value();
            if (in_psts.rooks.has_value())
                pawnTable = in_psts.rooks.value();
        } else {
            pawnTable = defPawnTable;
            knightTable = defKnightTable;
            bishopTable = defBishopTable;
            rookTable = defRookTable;
        }
    }

    /*
     * When considering piece-square scores for black pieces, flip the index
     * over the center of the board.
     */
    constexpr static const int mirror64[64] = {
            56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
            48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
            40	,	41	,	42	,	43	,	44	,	45	,	46	,	47	,
            32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
            24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
            16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
            8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
            0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
    };
};

int eval(const Board& board);

#endif //ATHENE_EVALUATION_H
