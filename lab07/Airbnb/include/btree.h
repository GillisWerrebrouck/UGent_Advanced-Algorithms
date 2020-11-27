#ifndef __btree
#define __btree
#include "schijf.h"
#include <iostream>
#include <stack>
#include <memory>
using std::ostream;

using namespace std;

template <class Sleutel, class Data, unsigned int orde>
class BTree;
template <class Sleutel, class Data, unsigned int orde>
class BKnoop;

template <class Sleutel, class Data, unsigned int orde>
class BTree
{

    typedef BKnoop<Sleutel, Data, orde> Knoop;

public:
    //constructor
    BTree(Schijf<Knoop> &s) : schijf(s)
    {
        Knoop wortel;
        wortel.aantalSleutels = 0;
        wortel.isblad = true;
        wortelindex = schijf.schrijf(wortel);
    }

    // toevoegen, zoeken, data opvragen en data wijzigen:
    void voegtoe(const Sleutel& sleutel, const Data& waarde);     // waarde bij de sleutel wegschrijven op de schijf
    unique_ptr<BKnoop<Sleutel, Data, orde>> voegtoeHelper(const BlokIndex currentBi, const Sleutel & sleutel, const Data& waarde, const BlokIndex rightNodeBi);
    bool zoekSleutel(const Sleutel& sleutel);              // Kijken of sleutel aanwezig is op de schijf
    unique_ptr<Data> geefData(const Sleutel& sleutel);                // waarde bij de sleutel opvragen
    void updatedata(const Sleutel& sleutel, const  Data& waarde);  // waarde bij de sleutel aanpassen naar nieuwe waarde 

    // hulpfuncties en output functies
    Sleutel geefsleutelwortel(int index); // nodig voor testen // geeft de sleutel van de wortel terug op index.
    unsigned int aantalsleutelswortel();  // nodig voor testen // geeft het aantal sleutels in de wortelknoop terug

    //uitschrijven van boom
    void schrijfknoop(BlokIndex index, std::ostream &os);
    void schrijf(std::ostream &os);

private:

    // schijf waar de boom op staat en index van de wortel als attributen
    Schijf<Knoop> &schijf;
    BlokIndex wortelindex;

    // Zoekt een waarde in de sleutels van knoop die bij sleutel past.
    // Returned pointer van de waarde + het gevolgde pad zal in de stack zitten (nodig bij toevoegen).
    unique_ptr<Data> zoek(const Sleutel& sleutel, stack<BlokIndex> &pad);
};

template <class Sleutel, class Data, unsigned int orde>
void BTree<Sleutel, Data, orde>::voegtoe(const Sleutel & sleutel, const Data& waarde)
{
    stack<BlokIndex> path;
    this->zoek(sleutel, path);

    Knoop x;
    x.sleutels[0] = sleutel;
    x.data[0] = waarde;
    x.index[1] = -1;
    auto xPtr = make_unique<BKnoop<Sleutel, Data, orde>>(x);

    while (xPtr != nullptr && !path.empty()) {
        BlokIndex currentBi = path.top();
        path.pop();
        xPtr = voegtoeHelper(currentBi, xPtr.get()->sleutels[0], xPtr.get()->data[0], xPtr.get()->index[1]);
    }

    if(path.empty() && xPtr != nullptr){
        // std::cout << "nieuwe wortel toevoegen" << std::endl;
        Knoop* nieuwewortel = xPtr.get();
        wortelindex = schijf.schrijf(*nieuwewortel);
        // schrijf(std::cout);
    }
}

template <class Sleutel, class Data, unsigned int orde>
unique_ptr<BKnoop<Sleutel, Data, orde>> BTree<Sleutel, Data, orde>::voegtoeHelper(const BlokIndex currentBi, const Sleutel & sleutel, const Data& waarde, const BlokIndex rightNodeBi)
{
    Knoop k = schijf.lees(currentBi);
    
    if (k.aantalSleutels < orde) {
        k.voegsleuteltoe(sleutel, waarde, rightNodeBi);
        schijf.herschrijf(k, currentBi);
        return nullptr;
    }

    Knoop z; // right node
    int m = orde / 2;

    for (int i = m+1; i < orde; i++) {
        k.aantalSleutels--;
        z.voegsleuteltoe(k.sleutels[i], k.data[i], k.index[i]);
    }
    z.index[orde] = k.index[orde];

    if (k.sleutels[m] < sleutel) {
        z.voegsleuteltoe(sleutel, waarde, rightNodeBi);
        
    } else {
        z.voegsleuteltoe(k.sleutels[m], k.data[m], k.index[m]);
        k.aantalSleutels--; // BVH: nodig om sleutel toe te voegen aan k en het aantal sleutels te verminderen
        k.voegsleuteltoe(sleutel, waarde, rightNodeBi);
    }

    BlokIndex zBi = schijf.schrijf(z);
    Knoop x;
    x.sleutels[0] = k.sleutels[m];
    x.data[0] = k.data[m];
    x.index[0] = currentBi;
    x.index[1] = zBi;
    x.aantalSleutels++; // BVH: niet vergeten aantal sleutels te verhogen
    x.isblad = false;
    k.aantalSleutels--; 
    schijf.herschrijf(k, currentBi);
    return make_unique<BKnoop<Sleutel, Data, orde>>(x);
}

// public functie om sleutel in de BTree te gaan opzoeken
template <class Sleutel, class Data, unsigned int orde>
bool BTree<Sleutel, Data, orde>::zoekSleutel(const Sleutel & sleutel)
{
    stack<BlokIndex> path;
    return this->zoek(sleutel, path) != nullptr;
}

// public functie om sleutel in de BTree te gaan
template <class Sleutel, class Data, unsigned int orde>
unique_ptr<Data> BTree<Sleutel, Data, orde>::geefData(const Sleutel & sleutel)
{
    stack<BlokIndex> path;
    return this->zoek(sleutel, path);
}

// // private zoek functie die ook het pad in de boom zal meegeven
template <class Sleutel, class Data, unsigned int orde>
unique_ptr<Data> BTree<Sleutel, Data, orde>::zoek(const Sleutel& sleutel, stack<BlokIndex> &path)
{
    BlokIndex currentBi = this->wortelindex;

    while (0 < currentBi) {
        Knoop k = schijf.lees(currentBi);
        path.push(currentBi);

        int i = k.binarySearch(sleutel, 0, k.aantalSleutels);

        if (i < k.aantalSleutels && k.sleutels[i] == sleutel) {
            return make_unique<Data>(k.data[i]);
        } else if (k.isblad) {
            return nullptr;
        }

        if (k.aantalSleutels == i) {
            i--;
        }

        if (sleutel < k.sleutels[i])
            currentBi = k.index[i];
        else
            currentBi = k.index[i+1];
    }

    return nullptr;
}

/*
Hulpfuncties om gegevens van de wortel op te vragen bij de test cases
*/

template <class Sleutel, class Data, unsigned int orde>
Sleutel BTree<Sleutel, Data, orde>::geefsleutelwortel(int index)
{
    Knoop wortel = schijf.lees(wortelindex);
    return wortel.sleutels[index];
}

template <class Sleutel, class Data, unsigned int orde>
unsigned int BTree<Sleutel, Data, orde>::aantalsleutelswortel()
{
    Knoop wortel = schijf.lees(wortelindex);
    return wortel.aantalSleutels;
}

/*
Schrijf functies
*/
template <class Sleutel, class Data, unsigned int orde>
void BTree<Sleutel, Data, orde>::schrijfknoop(BlokIndex index, std::ostream &os)
{
    // knoop inlezen op de schijf
    Knoop knoop = schijf.lees(index);
    os << "index op schijf van knoop: " << index << "\n";
    // knoop uitschrijven
    knoop.schrijf(os);
}

template <class Sleutel, class Data, unsigned int orde>
void BTree<Sleutel, Data, orde>::schrijf(std::ostream &os)
{
    // stack voor alle eventuele kinderknopen bij te houden en wortel er als eerste op plaatsen
    stack<BlokIndex> tebezoeken;
    tebezoeken.push(wortelindex);

    while (!tebezoeken.empty())
    {
        // knoop van de stack halen en uitschrijven
        BlokIndex huidige = tebezoeken.top();
        tebezoeken.pop();
        schrijfknoop(huidige, os);
        // om kinderen van deze knoop te kunnen vinden moet knoop ingelezen worden van de schijf,
        // is het geen blad worden de kinderen toegevoegd aan de stack
        Knoop knoop = schijf.lees(huidige);
        if (!knoop.isblad)
        {
            for (int j = 0; j <= knoop.aantalSleutels; j++)
            {
                tebezoeken.push(knoop.index[j]);
            }
        }
    }
}

#endif