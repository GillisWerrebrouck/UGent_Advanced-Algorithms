#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>
#include "pakjes.h"

#include <chrono>
using namespace std::chrono;

TEST_CASE("Pakjes", "[pakjes]")
{
	std::vector<int> voertuigen_grootte = {1, 4, 17, 18, 75};
	std::vector<int> v = {75, 18, 17, 4, 1, 21, 86, 90, 38, 115, 1515, 1516, 1517};
	std::vector<int> opl = {1, 1, 1, 1, 1, 2, 5, 5, 3, 4, 24, 24, 21};
	for (int i = 0; i < v.size(); i++)
	{
		int min_aantal = min_aantal_voertuigen(v[i], voertuigen_grootte);
		std::cout << "aantal voertuigen nodig voor " << v[i] << " pakjes: " << min_aantal << std::endl;
		REQUIRE(min_aantal == opl[i]);
	}
}

//zonder bakfiets
TEST_CASE("Pakjes zonder bakfiets", "[pakjes]")
{
	std::vector<int> voertuigen_grootte = {1, 17, 18, 75};
	std::vector<int> v = {75, 18, 17, 4, 1, 21, 86, 90, 38, 115, 1515, 1516, 1517};
	std::vector<int> opl = {1, 1, 1, 4, 1, 4, 5, 5, 4, 7, 24, 25, 21};
	for (int i = 0; i < v.size(); i++)
	{
		int min_aantal = min_aantal_voertuigen(v[i], voertuigen_grootte);
		std::cout << "aantal voertuigen nodig voor " << v[i] << " pakjes: " << min_aantal << std::endl;
		REQUIRE(min_aantal == opl[i]);
	}
}

TEST_CASE("Geen voertuigen of geen pakjes", "[fout]")
{
	std::vector<int> voertuigen_grootte1;
	REQUIRE(min_aantal_voertuigen(10, voertuigen_grootte1) == -1);
	voertuigen_grootte1 = {1, 17, 18, 75};
	REQUIRE(min_aantal_voertuigen(0, voertuigen_grootte1) == 0);
}

// test met timings van execution om recursie er uit te halen
// het kan zijn dat de test foutief af gaat
TEST_CASE("Niet recursief pakjes", "[experimenteel]")
{
	std::vector<int> voertuigen_grootte = {1, 17, 18, 75};

	auto t0 = high_resolution_clock::now();

	BENCHMARK("Min aantal voertuigen 115")
	{
		int min_aantal = min_aantal_voertuigen(115, voertuigen_grootte);
		REQUIRE(min_aantal == 7);
	};

	auto t1 = high_resolution_clock::now();

	BENCHMARK("Min aantal voertuigen 86")
	{
		int min_aantal = min_aantal_voertuigen(86, voertuigen_grootte);
		REQUIRE(min_aantal == 5);
	};

	auto t2 = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(t1 - t0);
	std::cout << duration.count() << " µs" << std::endl;
	auto duration2 = duration_cast<microseconds>(t2 - t1);
	std::cout << duration2.count() << " µs" << std::endl;
	REQUIRE(duration.count() < 2 * duration2.count());
}
