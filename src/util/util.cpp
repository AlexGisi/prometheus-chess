//
// Created by Alex Gisi on 6/7/22.
//

#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include "../defs.h"

/*
 * Return functional components of a line in the perft results (.epd)
 * file in a vector: fen, depth one result, ..., depth 6 result.
 *
 * Ref: https://stackoverflow.com/questions/10058606/splitting-a-string-by-a-character
 */
inline std::vector<std::string> parse_epd_line(const std::string& line) {
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
inline u64 get_time() {
    using namespace std::chrono;
    return static_cast<u64>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
}
