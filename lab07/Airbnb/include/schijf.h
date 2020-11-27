#ifndef __SCHIJF
#define __SCHIJF
#include <map>
#include <iostream>
using std::map;
typedef unsigned int BlokIndex;

//naamgeving gaat uit van de gebruiker: schrijf is een schrijfoperatie naar
//de schijf, enzovoorts
template <class Knoop>
class Schijf : private map<BlokIndex, Knoop>
{
public:
    //schrijf: voegt een Knoop toe aan de schijf. Geeft de index terug van de nieuwe knoop (zodat die gebruikt kan worden in de BTree).
    BlokIndex schrijf(const Knoop &b)
    {
        ++indexteller;
        this->operator[](indexteller) = b; //this-> is nodig (bij sommige compilers) omdat de parametrisatie niet afgeleid kan worden uit de argumenten.
        return indexteller;
    }

    // verwijder een blox index
    void verwijder(BlokIndex bi)
    {
        this->erase(bi);
    }

    // nieuw woord op de schijf plaatsen
    void herschrijf(const Knoop &b, const BlokIndex bi)
    {
        this->operator[](bi) = b;
    }

    // knoop van de schijf lezen en teruggeven
    Knoop lees(const BlokIndex bi)
    {
        return this->operator[](bi);
    }

    //constructor
    Schijf() : map<BlokIndex, Knoop>(), indexteller(0){};

private:
    //teller die het aantal toegevoegde elementen op de schijf zal tellen, merk op dat na verwijderen er ook "lege" indexen op de schijf zullen zijn.
    BlokIndex indexteller;
};

#endif
