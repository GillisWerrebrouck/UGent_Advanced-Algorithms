#include <vector>
#include <iostream>
#include "pakjes.h"


int main(int argc, char** argv) {
	
	std::vector<int> voertuigen_grootte = {1,17,18,75};
	std::vector<int> v = {75,18,17,4,1,21,86,90,38,115,1515,1516,1517};
	for(int aant: v){ 
		std::cout << "aantal voertuigen nodig voor " << aant << " pakjes: " << min_aantal_voertuigen(aant, voertuigen_grootte) << std::endl; 
	} 
}