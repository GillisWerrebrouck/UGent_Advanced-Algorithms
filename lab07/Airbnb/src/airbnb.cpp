#include "airbnb.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

BTree<int, int, 100> leesCSV_prijs(const std::string &bestandsnaam, Schijf<BKnoop<int, int, 100>>* schijf){
  BTree<int, int, 100> boom(*schijf);
  std::ifstream csvFile;
	csvFile.open(bestandsnaam.c_str());
  int teller = 0;
	if (!csvFile.is_open())
	{
		std::cout << "Fout pad naar bestand!!!!" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string line;
	getline(csvFile, line); // skip the 1st line
	while (getline(csvFile, line))
	{
    if (line.empty()) // skip empty lines:
		{
			continue;
		}

		vector<string> elements;
    bool instring = false;
		int lastIndex = 0;

		for (int i = 0; i < line.size(); i++) {
			if (line[i] == '"') {
				instring = !instring;
			} else if (i == line.size()-1 && instring) {
				string tempLine;
				while (tempLine.size() == 0) {
					getline(csvFile, tempLine);
				}
				line += tempLine;
			} else if (line[i] == ',' && !instring) {
				elements.push_back(line.substr(lastIndex, i-lastIndex));
				lastIndex = i+1;
			}
    }
		elements.push_back(line.substr(lastIndex));
		boom.voegtoe(stoi(elements[0]), stoi(elements[9]));
		teller++;
  }
  std::cout << teller << " prijzen toegevoegd" << std::endl;
  return boom;
}

BTree<int, hosts::property, 10> leesCSV(const std::string &bestandsnaam, Schijf<BKnoop<int, hosts::property, 10>>* schijf){
  BTree<int, hosts::property, 10> boom(*schijf);

  std::ifstream csvFile;
	csvFile.open(bestandsnaam.c_str());
  int teller = 0;
	if (!csvFile.is_open())
	{
		std::cout << "Fout pad naar bestand!!!!" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string line;
	getline(csvFile, line); // skip the 1st line
	while (getline(csvFile, line))
	{
    if (line.empty()) // skip empty lines:
		{
			continue;
		}

		vector<string> elements;
    bool instring = false;
		int lastIndex = 0;

		for (int i = 0; i < line.size(); i++) {
			if (line[i] == '"') {
				instring = !instring;
			} else if (i == line.size()-1 && instring) {
				string tempLine;
				while (tempLine.size() == 0) {
					getline(csvFile, tempLine);
				}
				line += tempLine;
			} else if (line[i] == ',' && !instring) {
				elements.push_back(line.substr(lastIndex, i-lastIndex));
				lastIndex = i+1;
			}
    }

		hosts::property p;
		p.name = elements[1];
		p.buurt = elements[5];
		p.prijs = stoi(elements[9]);
		p.aantalreviews = stoi(elements[11]);
		p.beschikbaarheid = stoi(elements[14]);

		elements.push_back(line.substr(lastIndex));
		boom.voegtoe(stoi(elements[0]), p);
		teller++;
	}
	std::cout << teller << " huizen toegevoegd" << std::endl;
	return boom;
}
