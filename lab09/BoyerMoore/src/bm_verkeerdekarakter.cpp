#include "boyermoore.h"

BMVerkeerdeKarakter::BMVerkeerdeKarakter(const string &naald) : naald(naald)
{
  for (int i = 0; i < naald.size(); i++) {
    auto it = lookup.find(naald[i]);
    if (it != lookup.end()) {
      it->second = i;
    } else {
      lookup.insert(pair<char, int>(naald[i], i));
    }
  }
}

bool BMVerkeerdeKarakter::compare(int naaldPos, char hooibergChar) {
  laatsteAantalKaraktervergelijkingen++;
  return naald[naaldPos] == hooibergChar;
}

std::queue<int> BMVerkeerdeKarakter::zoek(const string &hooiberg)
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
      naaldPos += 1;
      continue;
    }
    
    auto it = lookup.find(hooiberg[naaldPos + i]);
    if (it != lookup.end() && 0 < i - it->second) {
      naaldPos += i - it->second;
    } else if (it != lookup.end()) {
      naaldPos += 1;
    } else {
      naaldPos += i + 1;
    }
  }

  return gevondenPosities;
}

BMVerkeerdeKarakterAlternative::BMVerkeerdeKarakterAlternative(const string &naald) : naald(naald), lookup(naald.size())
{
  for (int i = 0; i < naald.size(); i++) {
    if (i != 0) {
      lookup[i] = lookup[i - 1];
    }

    auto it = lookup[i].find(naald[i]);
    if (it != lookup[i].end()) {
      it->second = i;
    } else {
      lookup[i].insert(pair<char, int>(naald[i], i));
    }
  }
}

bool BMVerkeerdeKarakterAlternative::compare(int naaldPos, char hooibergChar) {
  laatsteAantalKaraktervergelijkingen++;
  return naald[naaldPos] == hooibergChar;
}

std::queue<int> BMVerkeerdeKarakterAlternative::zoek(const string &hooiberg)
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
      naaldPos += 1;
      continue;
    }

    auto it = lookup[i].find(hooiberg[naaldPos + i]);
    if (it == lookup[i].end()) {
      naaldPos += i + 1;
    } else {
      naaldPos += i - it->second;
    }
  }

  return gevondenPosities;
}
