#ifndef __bknoop
#define __bknoop
#include "btree.h"
#include "schijf.h"

template <class Sleutel, class Data, unsigned int orde>
class BKnoop
{
    friend class BTree<Sleutel, Data, orde>;

public:
    BKnoop<Sleutel, Data, orde>()
    {
        isblad = true;
        aantalSleutels = 0;
    };

    const BKnoop<Sleutel, Data, orde> &operator=(const BKnoop<Sleutel, Data, orde> &b)
    {
        aantalSleutels = b.aantalSleutels;
        isblad = b.isblad;
        for (unsigned int i = 0; i < aantalSleutels; i++)
        {
            sleutels[i] = b.sleutels[i];
            data[i] = b.data[i];
        }
        if (!isblad)
        {
            for (unsigned int i = 0; i <= aantalSleutels; i++)
            {
                index[i] = b.index[i];
            }
        }
        return *this;
    }

    // array om in de knoop de sleutels bij te houden en de Data die bij een sleutel hoort
    // en array van BlokIndex met wijzers naar de indexen ("wijzers") op de schijf voor de knopen die horen bij de sleutel
    // de arrays worden gesorteerd bijgehouden!
    Sleutel sleutels[orde];
    Data data[orde];
    BlokIndex index[orde + 1];

    // aantalSleutels die momenteel in de knoop zitten en of het al dan niet een blad is
    unsigned int aantalSleutels;
    bool isblad;

    // functie die een nieuwe sleutel aan een knoop zal toevoegen
    int voegsleuteltoe(Sleutel key, Data val, BlokIndex bi);
    void schrijf(std::ostream &os) const;

private:
    int binarySearch(Sleutel key, int l, int r);
};

template <class Sleutel, class Data, unsigned int orde>
int BKnoop<Sleutel, Data, orde>::binarySearch(Sleutel key, int l, int r) {
    if (l < r) {
        int m = l + (r-l)/2;

        if (this->sleutels[m] < key) {
            return this->binarySearch(key, m+1, r);
        } else {
            return this->binarySearch(key, l, m);
        }
    } else {
        return r;
    }
}

template <class Sleutel, class Data, unsigned int orde>
int BKnoop<Sleutel, Data, orde>::voegsleuteltoe(Sleutel key, Data val, BlokIndex bi)
{
    int i = 0;
    if (0 < this->aantalSleutels)
        i = this->binarySearch(key, 0, this->aantalSleutels);
    int keyIndex = i;

    this->aantalSleutels++;
    while (i != this->aantalSleutels) {
        swap(this->sleutels[i], key);
        swap(this->data[i], val);
        swap(this->index[i+1], bi); // swappen vanaf index i+1 (index op positie i zal tussen vorige sleutel en huidige zitten
        i++;
    }
    
    return keyIndex;
}

template <class Sleutel, class Data, unsigned int orde>
void BKnoop<Sleutel, Data, orde>::schrijf(std::ostream &os) const
{

    os << " --- " << aantalSleutels << " sleutels\n";
    for (int i = 0; i < aantalSleutels; i++)
    {
        os << sleutels[i] << " (" << data[i] << ") ";
    }
    os << "\n";
    if (isblad)
    {
        os << "knoop is blad";
    }
    else
    {
        os << "wijzers naar:\n";
        for (int j = 0; j <= aantalSleutels; j++)
        {
            os << index[j] << " - ";
        }
    }
    os << "\n";
}

#endif