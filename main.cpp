#include <iostream>
#include <string>
#include "dependencies/nlohmann/json.hpp"
// #include "dependencies/OMPEval/omp/EquityCalculator.h"

using json = nlohmann::json;

int main() {
	json card_ranges;
	std::cin >> card_ranges;
	std::cout << card_ranges.dump(2);
	return 0;
}
