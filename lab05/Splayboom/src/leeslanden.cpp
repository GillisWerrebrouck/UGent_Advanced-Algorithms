#include "leeslanden.h"
#include <string>
#include <vector>
#include "splayboom.h"
#include <iostream>

std::vector<landen::wedstrijd> readCSV(const std::string &bestandsnaam)
{
	std::vector<landen::wedstrijd> vec_wedstrijden;
	Splayboom<string, int> landen;
	std::ifstream csvFile;
	csvFile.open(bestandsnaam.c_str());

	if (!csvFile.is_open())
	{
		std::cout << "Fout pad naar bestand!!!!" << std::endl;
		exit(EXIT_FAILURE);
	}
	string line;
	getline(csvFile, line); // skip the 1st line
	while (getline(csvFile, line))
	{
		if (line.empty()) // skip empty lines:
		{
			continue;
		}

		std::istringstream iss(line);
		std::string lineStream;

		std::vector<std::string> row;

		while (getline(iss, lineStream, ','))
		{
			row.push_back(lineStream);
		}
		int score_home = std::stoi(row[3]);
		int score_away = std::stoi(row[4]);
		std::string winner = row[1];
		if (score_away > score_home)
		{
			winner = row[2];
		}
		else if (score_away == score_home)
		{
			winner = "gelijkspel";
		}
		landen::wedstrijd wedstr = {row[1], row[2], score_home, score_away, winner};
		vec_wedstrijden.push_back(wedstr);
	}
	return vec_wedstrijden;
}

Splayboom<string, int> doelpunten(std::vector<landen::wedstrijd> wedstrijden)
{
	Splayboom<string, int> boom;
	
	auto it = wedstrijden.begin();
	while (it != wedstrijden.end()) {
		bool found = boom.zoekSleutel(it->thuisploeg);

		if (found) {
			boom->wijzigData(boom->geefData() + it->thuisscore);
		} else {
			boom.voegtoe(it->thuisploeg, it->thuisscore);
		}

		found = boom.zoekSleutel(it->bezoekers);

		if (found) {
			boom->wijzigData(boom->geefData() + it->bezoekersscore);
		} else {
			boom.voegtoe(it->bezoekers, it->bezoekersscore);
		}

		it++;
	}

	return boom;
}

Splayboom<string, int> overwinningen(std::vector<landen::wedstrijd> wedstrijden)
{
	Splayboom<string, int> boom;

	auto it = wedstrijden.begin();
	while (it != wedstrijden.end()) {
		bool found = boom.zoekSleutel(it->thuisploeg);
		int i = it->winnaar == it->thuisploeg;

		if (found) {
			boom->wijzigData(boom->geefData() + i);
		} else {
			boom.voegtoe(it->thuisploeg, i);
		}

		found = boom.zoekSleutel(it->bezoekers);
		i = it->winnaar == it->bezoekers;
	
		if (found) {
			boom->wijzigData(boom->geefData() + i);
		} else {
			boom.voegtoe(it->bezoekers, i);
		}

		it++;
	}

	return boom;
}