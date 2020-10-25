#ifndef __zoekBoom_H
#define __zoekBoom_H
#include <cstdlib>
#include <iostream>
#include <queue>
#include <memory>
#include <functional>
#include <fstream>
#include <sstream>
#include <cassert>
#include <random>
#include <cmath>

using std::cerr;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::pair;
using std::string;
using std::unique_ptr;
using std::make_unique;
using std::move;
/**********************************************************************

   Class: ZoekBoom

   beschrijving: Binaire ZoekBoom waarin duplicaatsleutels wel of niet zijn toegestaan.

***************************************************************************/

template <class Sleutel, class Data>
class ZoekKnoop;

template <class Sleutel, class Data>
class ZoekBoom : public unique_ptr<ZoekKnoop<Sleutel, Data>>
{
	using unique_ptr<ZoekKnoop<Sleutel, Data>>::unique_ptr;

public:
	// constructors and assignment operators
	ZoekBoom();
	ZoekBoom(const Sleutel &sleutel, const Data &data);
	ZoekBoom(ZoekBoom<Sleutel, Data> &andere);
	ZoekBoom(ZoekBoom<Sleutel, Data> &&andere);
	ZoekBoom<Sleutel, Data> &operator=(const ZoekBoom<Sleutel, Data> &andere);
	ZoekBoom<Sleutel, Data> &operator=(ZoekBoom<Sleutel, Data> &&andere);


	// Output functies schrijf en teken
	void inorder(std::function<void(const ZoekKnoop<Sleutel, Data> &)> bezoek) const;
	void schrijf(ostream &os) const;
	void teken(const char *bestandsnaam);
	string tekenrec(ostream &uit, int &knoopteller);

	//zoekboom operaties 
	void roteer(bool naarlinks);
  virtual bool zoekSleutel(const Sleutel& sleutel);
	virtual void voegtoe(const Sleutel &sleutel, const Data &data);
	virtual ZoekKnoop<Sleutel, Data> *verwijder(const Sleutel &sleutel); //geeft pointer naar ouder terug
	void maakOnevenwichtig();
	void maakEvenwichtig();

	//zoekboom hulpfuncties
	int aantalKnopen();
	int geefDiepte();
	ZoekBoom<Sleutel, Data> *geefBoomVanKnoop(ZoekKnoop<Sleutel, Data> &knoopptr);
	

protected:
	struct ZoekResultaat
	{
		ZoekBoom<Sleutel, Data> *plaats;
		ZoekKnoop<Sleutel, Data> *ouder;
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
     * omdat ze een niet-const pointer naar het inwendige van de boom teruggeeft.
     */
	ZoekResultaat zoek(const Sleutel &sleutel);
};

/*****************************************************************************

	Implementatie

*****************************************************************************/

/*
Constructors en assignment operators
*/
template <class Sleutel, class Data>
ZoekBoom<Sleutel, Data>::ZoekBoom() {}

template <class Sleutel, class Data>
ZoekBoom<Sleutel, Data>::ZoekBoom(const Sleutel& sleutel, const Data& data) {
	auto nieuw = make_unique<ZoekKnoop<Sleutel, Data>>(sleutel, data);
	this->swap(nieuw);
}

/**
 * Copy contructor
 */
template <class Sleutel, class Data>
ZoekBoom<Sleutel, Data>::ZoekBoom(ZoekBoom<Sleutel, Data> &andere) : std::unique_ptr<ZoekKnoop<Sleutel, Data>>{nullptr}
{
	if (andere)
	{
		(*this) = std::make_unique<ZoekKnoop<Sleutel, Data>>(*andere);
	}
}

/**
 * Move contructor
 */
template <class Sleutel, class Data>
ZoekBoom<Sleutel, Data>::ZoekBoom(ZoekBoom<Sleutel, Data> &&andere)
	: std::unique_ptr<ZoekKnoop<Sleutel, Data>>{std::move(andere)}
{
}

/**
 * Move assignment operator
 */
template <class Sleutel, class Data>
ZoekBoom<Sleutel, Data> &ZoekBoom<Sleutel, Data>::operator=(ZoekBoom<Sleutel, Data> &&andere)
{
	this->swap(andere);
	return *this;
}

/**
 * Copy assignment operator
 */
template <class Sleutel, class Data>
ZoekBoom<Sleutel, Data> &ZoekBoom<Sleutel, Data>::operator=(const ZoekBoom<Sleutel, Data> &andere)
{
	ZoekBoom<Sleutel, Data> temp(andere);
	temp.swap(*this);
	return *this;
}


/*
Output functies schrijf en teken
*/
template <class Sleutel, class Data>
void ZoekBoom<Sleutel, Data>::inorder(std::function<void(const ZoekKnoop<Sleutel, Data> &)> bezoek) const
{
	if (*this)
	{
		(*this)->links.inorder(bezoek);
		bezoek(**this);
		(*this)->rechts.inorder(bezoek);
	};
}

template <class Sleutel, class Data>
void ZoekBoom<Sleutel, Data>::schrijf(ostream &os) const
{
	inorder([&os](const ZoekKnoop<Sleutel, Data> &knoop) {
		if (knoop.ouder)
		{
			os << "(" << knoop.sleutel << " -> " << knoop.data << "/ ouder: " << knoop.ouder->sleutel << ")";
		}
		else
		{
			os << "(" << knoop.sleutel << " -> " << knoop.data << "/ wortel)";
		}
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

template <class Sleutel, class Data>
void ZoekBoom<Sleutel, Data>::teken(const char *bestandsnaam)
{
	ofstream uit(bestandsnaam);
	assert(uit);
	int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
	uit << "digraph {\n";
	this->tekenrec(uit, knoopteller);
	uit << "}";
}

template <class Sleutel, class Data>
string ZoekBoom<Sleutel, Data>::tekenrec(ostream &uit, int &knoopteller)
{
	ostringstream wortelstring;
	wortelstring << '"' << ++knoopteller << '"';
	if (!*this)
	{
		uit << wortelstring.str() << " [shape=point];\n";
	}
	else
	{
		uit << wortelstring.str() << "[label=\"" << (*this)->sleutel << ":" << (*this)->data << "\"]";
		uit << ";\n";
		string linkskind = (*this)->links.tekenrec(uit, knoopteller);
		string rechtskind = (*this)->rechts.tekenrec(uit, knoopteller);
		uit << wortelstring.str() << " -> " << linkskind << ";\n";
		uit << wortelstring.str() << " -> " << rechtskind << ";\n";
	};
	return wortelstring.str();
}


/*
	Zoekboom operaties
*/

template <class Sleutel, class Data>
void ZoekBoom<Sleutel, Data>::roteer(bool naarlinks)
{
	if (naarlinks) {
        auto right_child = move((*this)->rechts);
				if (right_child->links != nullptr)
        	(*this)->rechts = move(right_child->links);
        right_child->ouder = (*this)->ouder;

        if ((*this)->rechts)
            (*this)->rechts->ouder = (*this).get();

        (*this)->ouder = right_child.get();
        right_child->links = move(*this);
        *this = move(right_child);
    } else {
        auto left_child = move((*this)->links);
				if (left_child->rechts != nullptr)
        	(*this)->links = move(left_child->rechts);
        left_child->ouder = (*this)->ouder;

        if ((*this)->links)
            (*this)->links->ouder = (*this).get();

        (*this)->ouder = left_child.get();
        left_child->rechts = move(*this);
        *this = move(left_child);
    }
}

template <class Sleutel, class Data>
bool ZoekBoom<Sleutel, Data>::zoekSleutel(const Sleutel &sleutel){
    auto [plaats,ouder] = zoek(sleutel);
    return (*plaats != nullptr);
}

template <class Sleutel, class Data>
typename ZoekBoom<Sleutel, Data>::ZoekResultaat ZoekBoom<Sleutel, Data>::zoek(const Sleutel &gezochteSleutel)
{
	ZoekBoom<Sleutel, Data> *plaats = this;
	ZoekKnoop<Sleutel, Data> *ouder = this && this->get() ? this->get()->ouder : nullptr;

	while (*plaats && (*plaats)->sleutel != gezochteSleutel) {
			ouder = plaats->get();
			if (gezochteSleutel < plaats->get()->sleutel)
					plaats = &(*plaats)->links;
			else
					plaats = &(*plaats)->rechts;
	};

	return ZoekResultaat{plaats, ouder};
};

template <class Sleutel, class Data>
void ZoekBoom<Sleutel, Data>::voegtoe(const Sleutel &sleutel, const Data &data)
{
	auto [plaats,ouder] = zoek(sleutel);
	ZoekBoom<Sleutel, Data> newSubTree(sleutel, data);
  
	if (*plaats == nullptr) {
		newSubTree->ouder = ouder;
		*plaats = move(newSubTree);
	}
}

template <class Sleutel, class Data>
void ZoekBoom<Sleutel, Data>::maakOnevenwichtig()
{
	while (this->get()->links != nullptr) {
		this->roteer(false);
	}
	if (this->get()->rechts != nullptr) {
		this->get()->geefKind(false).maakOnevenwichtig();
	}
}

template <class Sleutel, class Data>
void ZoekBoom<Sleutel, Data>::maakEvenwichtig()
{
	this->maakOnevenwichtig();
	while (std::abs(this->get()->links.geefDiepte() - this->get()->rechts.geefDiepte()) > 1) {
		this->roteer(true);
	}
	
	if (this->get()->links != nullptr) {
		this->get()->links.maakEvenwichtig();
	}
	if (this->get()->rechts != nullptr) {
		this->get()->rechts.maakEvenwichtig();
	}
}

template <class Sleutel, class Data>
ZoekKnoop<Sleutel, Data> *ZoekBoom<Sleutel, Data>::verwijder(const Sleutel &sleutel)
{
	auto [plaats,ouder] = zoek(sleutel);
  
	if (*plaats != nullptr) {
		if (plaats->geefDiepte() == 1) { // plaats has no children
			plaats->get()->ouder->geefKind(plaats->get()->ouder->links == *plaats) = nullptr;
		} else if (plaats->get()->links == nullptr ^ plaats->get()->rechts == nullptr) { // plaats has one child
			*plaats = move((plaats->get()->links == nullptr) ? plaats->get()->rechts : plaats->get()->links);
			plaats->get()->ouder = ouder;
		} else { // plaats has two children
			ZoekKnoop<Sleutel, Data> *rightChild = plaats->get()->rechts.get();

			while (rightChild->links != nullptr) {
				rightChild = rightChild->links.get();
			}

			plaats->get()->sleutel = rightChild->sleutel;
			plaats->get()->data = rightChild->data;
			plaats->get()->rechts.verwijder(plaats->get()->sleutel);
		}
	}

	return ouder;
}

/*
hulpfuncties
*/
template <class Sleutel, class Data>
ZoekBoom<Sleutel, Data> *ZoekBoom<Sleutel, Data>::geefBoomVanKnoop(ZoekKnoop<Sleutel, Data> &knoop)
{
	assert((*this)->ouder == nullptr); //deze functie mag enkel in de wortel van de boom worden opgeroepen
	if (knoop.ouder == nullptr)
		return this;
	else if (knoop.ouder->links.get() == &knoop)
		return &(knoop.ouder->links);
	else
		return &(knoop.ouder->rechts);
}

template <class Sleutel, class Data>
int ZoekBoom<Sleutel, Data>::geefDiepte()
{
	if (*this)
	{
		return 1 + std::max((*this)->links.geefDiepte(), (*this)->rechts.geefDiepte());
	}
	else
		return 0;
}

template <class Sleutel, class Data>
int ZoekBoom<Sleutel, Data>::aantalKnopen()
{
	if (*this)
	{
		return 1 + (*this)->links.aantalKnopen() + (*this)->rechts.aantalKnopen();
	}
	return 0;
}

#endif