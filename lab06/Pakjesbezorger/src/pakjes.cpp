#include "pakjes.h"
#include <iostream>

using namespace std;

void min_aantal_voertuigen_recursief(int aantalPakjes, const vector<int>& voertuigen_grootte, vector<int>& currResult, vector<vector<int>>& results, int& currMin) {
	if (aantalPakjes == 0) {
		currMin = currResult.size();
		results.push_back(currResult);
		return;
	}

	auto it = voertuigen_grootte.end();
	while (it != voertuigen_grootte.begin() && currMin > currResult.size()) {
		it--;

		if (currResult.size() != 0 && currResult[currResult.size()-1] < *it) continue;

		if (aantalPakjes >= *it) {
			// add to result
			currResult.push_back(*it);

			// recursive call
			min_aantal_voertuigen_recursief(aantalPakjes - *it, voertuigen_grootte, currResult, results, currMin);

			// remove result
			currResult.pop_back();
		}
	}
}

int min_aantal_voertuigen(int aantalPakjes, const std::vector<int>& voertuigen_grootte) {
	if (voertuigen_grootte.size() == 0) return -1;

	vector<int> currResult;
	vector<vector<int>> results;
	int currMin = INT16_MAX;
	min_aantal_voertuigen_recursief(aantalPakjes, voertuigen_grootte, currResult, results, currMin);

	auto it = results.begin();
	int optimalCount = INT16_MAX;
	while (it != results.end()) {
		if (it->size() < optimalCount)
			optimalCount = it->size();		
		it++;
	}

	return optimalCount;
}
