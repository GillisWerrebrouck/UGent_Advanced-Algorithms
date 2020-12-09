#ifndef __BOYERMOORE_H
#define __BOYERMOORE_H

#include <queue>
#include <string>
#include <array>
#include <map>
#include <vector>
#include <iostream>
using std::string;

using namespace std;

typedef unsigned int uint;
typedef unsigned char uchar;

const uint ALFABET_GROOTTE = 256; //unsigned char is maximaal 1 byte groot 

/**
 * Deze klasse gebruikt enkel de heuristiek van het verkeerde karakter 
 */
class BMVerkeerdeKarakter
{
public:
    BMVerkeerdeKarakter(const string &naald);
    bool compare(int naaldPos, char hooibergChar);
    std::queue<int> zoek(const string &hooiberg);

    /**
     * Deze variabele wordt ingevuld met het aantal karaktervergelijkingen dat nodig was in de laatste zoek-opdracht
     */
    long int laatsteAantalKaraktervergelijkingen = 0;

    map<char, int> lookup;
    const string naald;
};

/**
 * Deze klasse gebruikt enkel de heuristiek van het verkeerde karakter (gebruik van vector<map<char, int>>)
 */
class BMVerkeerdeKarakterAlternative
{
public:
    BMVerkeerdeKarakterAlternative(const string &naald);
    bool compare(int naaldPos, char hooibergChar);
    std::queue<int> zoek(const string &hooiberg);

    /**
     * Deze variabele wordt ingevuld met het aantal karaktervergelijkingen dat nodig was in de laatste zoek-opdracht
     */
    long int laatsteAantalKaraktervergelijkingen = 0;

    vector<map<char, int>> lookup;
    const string naald;
};

/**
 * Deze klasse implementeert de variant van Horspool van het Boyer-Moore algoritme
 */
class BMHorspool
{
public:
    BMHorspool(const string &naald);
    bool compare(int naaldPos, char hooibergChar);
    std::queue<int> zoek(const string &hooiberg);

    /**
     * Deze variabele wordt ingevuld met het aantal karaktervergelijkingen dat nodig was in de laatste zoek-opdracht
     */
    long int laatsteAantalKaraktervergelijkingen = 0;

    vector<int> lookup;
    const string naald;
};

/**
 * Deze klasse implementeert de variant van Sunday van het Boyer-Moore algoritme
 */
class BMSunday
{
public:
    BMSunday(const string &naald);
    bool compare(int naaldPos, char hooibergChar);
    std::queue<int> zoek(const string &hooiberg);

    /**
     * Deze variabele wordt ingevuld met het aantal karaktervergelijkingen dat nodig was in de laatste zoek-opdracht
     */
    long int laatsteAantalKaraktervergelijkingen = 0;

    vector<int> lookup;
    const string naald;
};

#endif
