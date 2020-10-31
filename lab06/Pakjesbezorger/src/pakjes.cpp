#include "pakjes.h"
#include <iostream>

using namespace std;

// void min_aantal_voertuigen_recursief(int aantalPakjes, const vector<int>& voertuigen_grootte, vector<int>& currResult, vector<int>& result) {
// 	if (aantalPakjes == 0) {
// 		result = currResult;
// 		return;
// 	}

// 	auto it = voertuigen_grootte.end();
// 	while (it != voertuigen_grootte.begin() && (result.size() == 0 ^ result.size() > currResult.size())) {
// 		it--;

// 		if (currResult.size() != 0 && currResult[currResult.size()-1] < *it) continue;

// 		if (aantalPakjes >= *it) {
// 			// add to result
// 			currResult.push_back(*it);

// 			// recursive call
// 			min_aantal_voertuigen_recursief(aantalPakjes - *it, voertuigen_grootte, currResult, result);

// 			// remove result
// 			currResult.pop_back();
// 		}
// 	}
// }

// int min_aantal_voertuigen(int aantalPakjes, const std::vector<int>& voertuigen_grootte) {
// 	if (voertuigen_grootte.size() == 0) return -1;

// 	vector<int> currResult;
// 	vector<int> result;
// 	min_aantal_voertuigen_recursief(aantalPakjes, voertuigen_grootte, currResult, result);

// 	return result.size();
// }


int min_aantal_voertuigen(int aantalPakjes, const std::vector<int>& voertuigen_grootte) {
	if (voertuigen_grootte.size() == 0) return -1;

	vector<vector<int>> rooster(voertuigen_grootte.size(), vector<int>(aantalPakjes+1, 0));

	for (int p = 1; p <= aantalPakjes; p++) {
		int minVoertuigen = -1;
		int huidigVoertuigen = 0;
		vector<int> minVoertuigenV(voertuigen_grootte.size(), 0);
		vector<int> huidigMinVoertuigenV(voertuigen_grootte.size(), 0);

		for (int voertuigIndex = voertuigen_grootte.size()-1; 0 <= voertuigIndex; voertuigIndex--) {
			if (p < voertuigen_grootte[voertuigIndex]) continue;

			huidigMinVoertuigenV = vector<int>(voertuigen_grootte.size(), 0);

			huidigVoertuigen = 1;
			huidigMinVoertuigenV[voertuigIndex] = 1;
			int restP = p - voertuigen_grootte[voertuigIndex];

			if (restP != 0) {
				for (int v = 0; v < voertuigen_grootte.size(); v++) {
					huidigVoertuigen += rooster[v][restP];
					huidigMinVoertuigenV[v] += rooster[v][restP];
				}
			}

			if (minVoertuigen == -1 || minVoertuigen > huidigVoertuigen) {
				minVoertuigen = huidigVoertuigen;
				minVoertuigenV = huidigMinVoertuigenV;
			}
		}

		for (int v = 0; v < voertuigen_grootte.size(); v++) {
			rooster[v][p] = minVoertuigenV[v];
		}
	}

	int aantalVoertuigen = 0;
	for (int v = 0; v < voertuigen_grootte.size(); v++) {
		aantalVoertuigen += rooster[v][aantalPakjes];
	}

	return aantalVoertuigen;
}
