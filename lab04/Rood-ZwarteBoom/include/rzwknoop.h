#ifndef __RZWknoopH
#define __RZWknoopH
#include "rzwboom.h"

template <class Sleutel>
class RZWknoop {
    // Friend Class: A friend class can access private and protected members of other class in which it is declared as friend.
    friend class RZWboom<Sleutel>;
    public:
        RZWknoop():ouder(0){}
        // Copy constructor (you copy the key, there is no unique_ptr involved)
        RZWknoop(const Sleutel& sl):sleutel{sl},ouder(0),kleur(rood){};
        // Use move because of unique_ptr (you don't want to make a copy)
        RZWknoop(Sleutel&& sl):sleutel{move(sl)},ouder(0),kleur(rood){};
        
        RZWboom<Sleutel>& geefKind(bool links);
        RZWkleur geefKleur();
        void zetKleur(RZWkleur kleur);

        // Structure of a node
        Sleutel sleutel;
        RZWknoop<Sleutel>* ouder;
        RZWboom<Sleutel> links,rechts;
        RZWkleur kleur;
};

template <class Sleutel>
RZWboom<Sleutel>& RZWknoop<Sleutel>:: geefKind(bool linkerkind){
    if (linkerkind)
        return links;
    else
        return rechts;
};

template <class Sleutel>
RZWkleur RZWknoop<Sleutel>::geefKleur() {
    return kleur;
}

template <class Sleutel>
void RZWknoop<Sleutel>::zetKleur(RZWkleur kleur) {
    this->kleur = kleur;
};


#endif