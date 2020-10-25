#include "splayboom.h"
#include "leeslanden.h"
#include <vector>
#include <string>

int main()
{
	{
		auto wedstrijden = readCSV("../voetbal_resultaten.csv");

		Splayboom<std::string, int> doelp = doelpunten(wedstrijden);
		Splayboom<std::string, int> overw = overwinningen(wedstrijden);

		int aantalLanden = overw.aantalKnopen();
		std::cout << "aantal landen is gelijk aan : " << aantalLanden << std::endl;

		std::string scenario1[10] = {"gelijkspel", "France", "Faroe Islands", "France", "Gambia", "Greece", "Finland", "gelijkspel", "gelijkspel", "gelijkspel"};
		std::string scenario2[10] = {"gelijkspel", "Belgium", "France", "United States", "Canada", "Russia", "China", "gelijkspel", "Belgium", "England"};
		std::string scenario3[10] = {"gelijkspel", "Faroe Islands", "Finland", "France", "France", "Gambia", "gelijkspel", "gelijkspel", "gelijkspel", "Greece"};

		// for (auto i : scenario1)
		// {
		// 	overw.zoekSleutel(i);
		// 	doelp.zoekSleutel(i);
		// 	std::cout << i << " heeft " << doelp.get()->sleutel << "doelpunten" << std::endl;
		// 	std::cout << i << " heeft " << overw.get()->sleutel << "overwiningen" << std::endl;
		// }
	}
}
