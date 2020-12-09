#include "boyermoore.h"

BMHorspool::BMHorspool(const string &naald) : naald(naald), lookup(256, naald.size())
{
  for (int i = 0; i < naald.size() - 1; i++) {
    lookup[naald[i]] = naald.size() - i - 1;
  }
}

bool BMHorspool::compare(int naaldPos, char hooibergChar) {
  laatsteAantalKaraktervergelijkingen++;
  return naald[naaldPos] == hooibergChar;
}

std::queue<int> BMHorspool::zoek(const string &hooiberg)
{
  queue<int> gevondenPosities;
  int naaldPos = 0;

  while (naaldPos + naald.size() <= hooiberg.size()) {
    int i = naald.size() - 1;
    while (0 <= i && compare(i, hooiberg[naaldPos + i])) {
      i--;
    }

    if (i < 0) {
      gevondenPosities.push(naaldPos);
    }
    
    naaldPos += lookup[hooiberg[naaldPos + naald.size() - 1]];
  }

  return gevondenPosities;
}
