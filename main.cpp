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

    eq.start(ranges, board, dead);
    eq.wait();
    auto results = eq.getResults();

    json res_json;
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

    // double stdErrMargin = 2e-5; // stop when standard error below 0.002%
    // auto callback = [&eq](const omp::EquityCalculator::Results& results) {
    //     std::cout << results.equity[0] << " " << 100 * results.progress
    //             << " " << 1e-6 * results.intervalSpeed << std::endl;
    //     if (results.time > 5) // Stop after 5s
    //         eq.stop();
    // };

    // double updateInterval = 0.25; // Callback called every 0.25s.
    // unsigned threads = 0; // max hardware parallelism (default)
    // eq.start(ranges, board, dead, false, stdErrMargin,
    // 		 callback, updateInterval, threads);
    // eq.wait();
    // auto r = eq.getResults();


    // std::cout << std::endl << r.equity[0] << " " << r.equity[1]
    // 	      << " " << r.equity[2] << std::endl;

    // std::cout << r.wins[0] << " " << r.wins[1] << " "
    // 		  << r.wins[2] << std::endl;

    // std::cout << r.hands << " " << r.time << " " << 1e-6 * r.speed
    // 		  << " " << r.stdev << std::endl;


	return 0;
}
