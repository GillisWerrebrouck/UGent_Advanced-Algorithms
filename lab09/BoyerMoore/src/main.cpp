#include <iostream>
#include "boyermoore.h"

int main() {
	BMVerkeerdeKarakter bm("abab");
	auto plaatsen = bm.zoek("abababababababab");

	while (!plaatsen.empty()) {
		cout << plaatsen.front() << " ";
		plaatsen.pop();
	}
	cout << endl;

	return 0;
}
