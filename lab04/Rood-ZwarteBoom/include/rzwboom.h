#ifndef __RZWboom_H
#define __RZWboom_H
#include <iostream>

#include <memory>
#include <functional>
#include <fstream>
#include <sstream>
#include <cassert>
#include <math.h>

using std::cerr;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::pair;
using std::string;
using std::unique_ptr;
using std::make_unique;
using std::move;
using std::max;

enum RZWkleur
{
    rood,
    zwart
};

template <class Sleutel>
class RZWknoop;

/**
 * Binaire RZWboom waarin geen duplicaatsleutels zijn toegestaan.
 */
template <class Sleutel>
class RZWboom : public unique_ptr<RZWknoop<Sleutel>>
{
public:
    using unique_ptr<RZWknoop<Sleutel>>::unique_ptr;

	// Move constructor because of unique_ptr
    RZWboom(unique_ptr<RZWknoop<Sleutel>> &&a);
    // Empty constructor, creates empty unique_ptr
    RZWboom() : unique_ptr<RZWknoop<Sleutel>>(){};

    //Gegeven
    void inorder(std::function<void(const RZWknoop<Sleutel> &)> bezoek) const;
    void schrijf(ostream &os) const;
    void tekenAls234Boom(const char *bestandsnaam) const;
    string tekenrec234(ostream &uit, int &nullteller, int &knoop34teller) const;
    void tekenAlsBinaireBoom(const char *bestandsnaam) const;
    string tekenrecBinair(ostream &uit, int &nullteller) const;
    // Return subtree of tree
    RZWboom<Sleutel> *geefBoomVanKnoop(RZWknoop<Sleutel> &knoop);

    //te implementeren
    bool zoekSleutel(const Sleutel &sleutel);
    void roteer(bool naarlinks);
    int geefDiepte() const;
    RZWkleur geefKleur() const;
    void zetKleur(RZWkleur kl);
    void voegtoe(const Sleutel &sleutel); //noot: functie mag alleen opgeroepen worden bij hoofdboom (bij wortel), niet bij deelboom!

protected:
    struct ZoekResultaat
    {
        RZWboom<Sleutel> *plaats;
        RZWknoop<Sleutel> *ouder;
    };
    /**
     * Deze functie zoekt sleutel en geeft de plaats in de boom terug waar de sleutel zich
     * bevindt, of indien afwezig: waar de sleutel zich zou moeten bevinden. 
     * 
     * Om deze functie ook in `voegtoe` te kunnen gebruiken geven we ook de ouder terug van 
     * de gevonden plaats. Deze referentie kan dan gebruikt worden om `ouder` in te vullen
     * in de nieuwe - aan te maken - knoop.
     * 
     * Tip: maak gebruik van structured binding om het resultaat van deze functie in twee
     *      variabelen op te slaan. Zo kan je voorkomen dat je de struct `ZoekResultaat`  
     *      dient te manipuleren.
     * 
     *      auto [plaats,ouder] = zoek(sleutel);
     * 
     * Noot: alhoewel de functie niets verandert aan de boom is ze geen const functie, 
     *       omdat ze een niet-const pointer naar het inwendige van de boom teruggeeft.
     */
	ZoekResultaat zoek(const Sleutel &sleutel);

private:
    void rebalanceAndRecolor(const Sleutel &sleutel);

};

template <class Sleutel>
RZWboom<Sleutel>::RZWboom(unique_ptr<RZWknoop<Sleutel>> &&a) : unique_ptr<RZWknoop<Sleutel>>(move(a)) {}

template <class Sleutel>
void RZWboom<Sleutel>::inorder(std::function<void(const RZWknoop<Sleutel> &)> bezoek) const
{
    // Check if there is a root-node for this RZWboom
	if (*this)
    {
        (*this)->links.inorder(bezoek);
        bezoek(**this);
        (*this)->rechts.inorder(bezoek);
    };
}

template <class Sleutel>
void RZWboom<Sleutel>::schrijf(ostream &os) const
{
    inorder([&os](const RZWknoop<Sleutel> &knoop) {
        os << "(" << knoop.sleutel << " ";
        if (knoop.kleur == rood)
            os << "rood";
        else
        {
            os << "zwart";
        }
        os << ")";
        os << "\n  Linkerkind: ";
        if (knoop.links)
            os << knoop.links->sleutel;
        else
            os << "-----";
        os << "\n  Rechterkind: ";
        if (knoop.rechts)
            os << knoop.rechts->sleutel;
        else
            os << "-----";
        os << "\n";
    });
}

template <class Sleutel>
void RZWboom<Sleutel>::tekenAls234Boom(const char *bestandsnaam) const
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int nullteller = 0;    //nullknopen moeten een eigen nummer krijgen.
    int knoop34teller = 0; //3-knopen en 4-knopen worden ook genummerd
    uit << "digraph {\n";
    this->tekenrec234(uit, nullteller, knoop34teller);
    uit << "}";
}

template <class Sleutel>
string RZWboom<Sleutel>::tekenrec234(ostream &uit, int &nullteller, int &knoop34teller) const
{
    ostringstream wortelstring;
    if (!*this)
    {
        wortelstring << "null" << ++nullteller;
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        wortelstring << '"' << (*this)->sleutel << '"';
        uit << wortelstring.str() << "[label=\"" << (*this)->sleutel << "\"]";
        if (this->geefKleur() == rood)
            uit << "[color=red]";
        uit << ";\n";
        string linkskind = (*this)->links.tekenrec234(uit, nullteller, knoop34teller);
        ;
        string rechtskind = (*this)->rechts.tekenrec234(uit, nullteller, knoop34teller);
        ;
        if ((*this)->links.geefKleur() == rood ||
            (*this)->rechts.geefKleur() == rood)
        {
            uit << "subgraph cluster_" << ++knoop34teller << " {\n   { rank=\"same\"; ";
            if ((*this)->links.geefKleur() == rood)
                uit << linkskind << " , ";
            if ((*this)->rechts.geefKleur() == rood)
                uit << rechtskind << " , ";
            uit << wortelstring.str() << "}\n";
            if ((*this)->links.geefKleur() == rood)
                uit << "   " << linkskind << " ->" << wortelstring.str() << "[dir=back];\n";
            if ((*this)->rechts.geefKleur() == rood)
                uit << "   " << wortelstring.str() << " -> " << rechtskind << ";\n";
            uit << "color=white\n}\n";
        };
        if ((*this)->links.geefKleur() == zwart)
            uit << wortelstring.str() << " -> " << linkskind << ";\n";
        if ((*this)->rechts.geefKleur() == zwart)
            uit << wortelstring.str() << " -> " << rechtskind << ";\n";
    };
    return wortelstring.str();
}

template <class Sleutel>
void RZWboom<Sleutel>::tekenAlsBinaireBoom(const char *bestandsnaam) const
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int nullteller = 0; //nullknopen moeten een eigen nummer krijgen.
    uit << "digraph {\n";
    this->tekenrecBinair(uit, nullteller);
    uit << "}";
}

template <class Sleutel>
string RZWboom<Sleutel>::tekenrecBinair(ostream &uit, int &nullteller) const
{
    ostringstream wortelstring;
    if (!*this)
    {
        wortelstring << "null" << ++nullteller;
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        wortelstring << '"' << (*this)->sleutel << '"';
        uit << wortelstring.str() << "[label=\"" << (*this)->sleutel << "\"]";
        if (this->geefKleur() == rood)
            uit << "[color=red]";
        uit << ";\n";
        string linkskind = (*this)->links.tekenrecBinair(uit, nullteller);
        string rechtskind = (*this)->rechts.tekenrecBinair(uit, nullteller);
        uit << wortelstring.str() << " -> " << linkskind << ";\n";
        uit << wortelstring.str() << " -> " << rechtskind << ";\n";
    };
    return wortelstring.str();
}

template <class Sleutel>
RZWboom<Sleutel> *RZWboom<Sleutel>::geefBoomVanKnoop(RZWknoop<Sleutel> &knoop)
{
    assert((*this)->ouder == nullptr); //deze functie mag enkel in de wortel van de boom worden opgeroepen

    if (knoop.ouder == nullptr)
        return this; //wortel
    else if (knoop.ouder->links.get() == &knoop)
        return &(knoop.ouder->links);
    else
        return &(knoop.ouder->rechts);
};

// Te implementeren:
template <class Sleutel>
int RZWboom<Sleutel>::geefDiepte() const
{
    if (!*this) {
        return 0;
    } else {
        int lDepth = this->get()->geefKind(true).geefDiepte();
        int rDepth = this->get()->geefKind(false).geefDiepte();

        return max(lDepth, rDepth)+1;
    }
}

template <class Sleutel>
typename RZWboom<Sleutel>::ZoekResultaat RZWboom<Sleutel>::zoek(const Sleutel &sleutel)
{
	RZWboom<Sleutel> *plaats = this;
    RZWknoop<Sleutel> *ouder = 0;

    while (*plaats && (*plaats)->sleutel != sleutel) {
        ouder = plaats->get();
        if (sleutel < plaats->get()->sleutel)
            plaats = &(*plaats)->links;
        else
            plaats = &(*plaats)->rechts;
    };

    return ZoekResultaat{plaats, ouder};
};

//gebruik makend van zoek()
template <class Sleutel>
bool RZWboom<Sleutel>::zoekSleutel(const Sleutel &sleutel)
{
	auto [plaats, ouder] = zoek(sleutel);
    return *plaats != nullptr;
}

// alternative rotate method
// template <class Sleutel>
// void RZWboom<Sleutel>::roteer(bool naarLinks)
// {   
//     // *this is the root, rotate starts from root
//     // Check child of the root -> l or p -> depends on rotateRight or rotateLeft
// 	RZWboom<Sleutel> lOrP = move((*this)->geefKind(!naarLinks));

//     // move beta
//     (*this)->geefKind(!naarLinks) = move(lOrP->geefKind(naarLinks));

//     // move l or p as a child of the new root
//     lOrP->geefKind(naarLinks) = move(*this);

//     // set the correct root
//     *this = move(lOrP);

//     // restore parent pointers
//     (*this)->ouder = (*this)->geefKind(naarLinks)->ouder;
//     (*this)->geefKind(naarLinks)->ouder = this->get();

//     // restore alpha if necessary
//     if ((*this)->geefKind(naarLinks)->geefKind(!naarLinks)) {
//         (*this)->geefKind(naarLinks)->geefKind(!naarLinks)->ouder = (*this)->geefKind(naarLinks).get();
//     }
// }

//preconditie: wortel en nodige kind bestaan
template <class Sleutel>
void RZWboom<Sleutel>::roteer(bool naarLinks)
{
    if (naarLinks) {
        auto right_child = move((*this)->rechts);
        (*this)->rechts = move(right_child->links);
        right_child->ouder = (*this)->ouder;

        if ((*this)->rechts)
            (*this)->rechts->ouder = (*this).get();

        (*this)->ouder = right_child.get();
        right_child->links = move(*this);
        *this = move(right_child);
    } else {
        auto left_child = move((*this)->links);
        (*this)->links = move(left_child->rechts);
        left_child->ouder = (*this)->ouder;

        if ((*this)->links)
            (*this)->links->ouder = (*this).get();

        (*this)->ouder = left_child.get();
        left_child->rechts = move(*this);
        *this = move(left_child);
    }
};

template <class Sleutel>
void RZWboom<Sleutel>::zetKleur(RZWkleur kleur)
{
	this->get()->kleur = kleur;
};

template <class Sleutel>
RZWkleur RZWboom<Sleutel>::geefKleur() const
{
	return this->get()->kleur;
}

template <class Sleutel>
void RZWboom<Sleutel>::rebalanceAndRecolor(const Sleutel &sleutel){
    auto [plaats, ouder] = zoek(sleutel);
    RZWboom<Sleutel> *c = plaats;
    RZWknoop<Sleutel> *p = ouder;

    // double red issue
    if(p != nullptr && p->kleur == rood && (*c).geefKleur() == rood) {
        // get grandparent of c
        RZWknoop<Sleutel> *g = p->ouder;

        // p is a left child of g
        if(p->sleutel < g->sleutel) {
            // switch colors if uncle is red
            if(g->rechts != nullptr && g->rechts->geefKleur() == rood) { // uncle is red
                RZWboom<Sleutel> *uncle = &(g->rechts);
                p->kleur = zwart;
                uncle->zetKleur(zwart);

                // make grandparent red if grandparent is not the root, else make it black by default
                if(g->ouder != nullptr) {
                    g->kleur = rood;
                } else {
                    g->kleur = zwart;
                }
                
                // recursive call method on grandparent
                if (g != nullptr) {
                    this->rebalanceAndRecolor(g->sleutel);
                }
            } else { // uncle is black
                // c is a left child of p (c/p/g are alligned)
                if(sleutel < p->sleutel) {
                    // rotate right & swicth the colors of p en g
                    geefBoomVanKnoop(*(g))->roteer(false);
                    p->kleur = zwart;
                    g->zetKleur(rood);
                } else { // c is a right child of p (c/p/g aren't alligned)
                    // rotate left & previous case (recursive)
                    geefBoomVanKnoop(*(p))->roteer(true);
                    this->rebalanceAndRecolor(p->sleutel);
                }
            }
        } else if (p->sleutel > g->sleutel) { // p is a right child of g
            // switch colors if uncle is red
            if (g->links != nullptr &&  g->links->geefKleur() == rood) {
                RZWboom<Sleutel> *uncle = &(g->links);
                // push black color from g to uncle and p
                p->kleur = zwart;
                uncle->zetKleur(zwart);

                // make grandparent red if grandparent is not the root, else make it black by default
                if (g->ouder != nullptr) {
                    g->kleur = rood;
                } else {
                    g->kleur = zwart;
                }

                if (g != nullptr) {
                    this->rebalanceAndRecolor(g->sleutel);
                }
            } else { // if uncle is black
                // c is left child of p (c/p/g aren't alligned)
                if (sleutel < p->sleutel) {
                    // rotate right from the parent perspective & previous case (recursive)
                    geefBoomVanKnoop(*p)->roteer(false);
                    this->rebalanceAndRecolor(p->sleutel);
                } else { // c is right child of p (c/p/g are alligned)
                    // rotate left from the parent perspective & switch the color of p and g
                    geefBoomVanKnoop(*(g))->roteer(true);
                    p->kleur = zwart;
                    // g is now the uncle node (after rotating)
                    g->zetKleur(RZWkleur::rood);
                }
            }
        }
    }
}

template <class Sleutel>
void RZWboom<Sleutel>::voegtoe(const Sleutel &sleutel)
{
    RZWboom<Sleutel> newSubTree = make_unique<RZWknoop<Sleutel>>(sleutel);
    
    if (geefDiepte() == 0) {
        newSubTree.zetKleur(zwart);
        *this = move(newSubTree);
    } else {
        auto [plaats, ouder] = zoek(sleutel);

        if (!*plaats) {
            newSubTree->ouder = ouder;
            *plaats = move(newSubTree);
            this->rebalanceAndRecolor(sleutel);
        }
    }
}

#endif
