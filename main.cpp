#include <iostream>
#include <string>
#include <vector>
#include "dependencies/nlohmann/json.hpp"
#include "dependencies/OMPEval/omp/EquityCalculator.h"

using json = nlohmann::json;

int main() {
    json sim_info;
    std::cin >> sim_info;

    omp::EquityCalculator eq;

    std::vector<std::string> range_input = sim_info["ranges"];
    std::vector<omp::CardRange> ranges(range_input.begin(), range_input.end());

    std::string board_input = sim_info["board"];
    uint64_t board = omp::CardRange::getCardMask(board_input);

    std::string dead_input = sim_info["dead"];
    uint64_t dead = omp::CardRange::getCardMask(dead_input);

    double stdErrMargin = 0.001; // stop when standard error below 0.1%
    double updateInterval = 0.1; // runs the callback every 0.1s
    unsigned threads = 4; // max hardware parallelism
    auto callback = [&eq](const omp::EquityCalculator::Results& results) {
        if (results.time > 3) // Stop after 3s
            eq.stop();
    };
    // omp::AnalysisType analysis_type = omp::AnalysisType::monte_carlo;
    eq.start(ranges, board, dead, false, false, stdErrMargin,
             callback, updateInterval, threads);
    eq.wait();
    auto results = eq.getResults();

    // std::vector<std::vector<std::array<uint8_t,2>>> ranges_combos;
    // auto get_combos = [](const omp::CardRange &rng) { return rng.combinations(); };
    // std::transform(ranges.begin(), ranges.end(), ranges_combos.begin(), get_combos);

    json res_json;
    // res_json["ranges"] = ranges_combos;
    res_json["board"] = board;
    res_json["dead"] = dead;
    res_json["equity"] = results.equity;
    res_json["players"] = results.players;
    res_json["wins"] = results.wins;
    res_json["ties"] = results.ties;
    res_json["hands"] = results.hands;
    res_json["time"] = results.time;
    res_json["stdev"] = results.stdev;
    res_json["evaluations"] = results.evaluations;
    res_json["finished"] = results.finished;

    json output;
    output["input"] = sim_info;
    output["results"] = res_json;

    std::cout << output.dump(2) << std::endl;

    return 0;
}
