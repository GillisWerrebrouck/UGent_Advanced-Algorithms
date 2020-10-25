
#ifndef Splayboom_H
#define Splayboom_H

#include "zoekknoop.h"

template <class Sleutel, class Data>
class Splayboom : public ZoekBoom<Sleutel, Data>
{
public: 
    bool zoekSleutel(const Sleutel& sleutel) override;
    void voegtoe(const Sleutel &sleutel, const Data &data) override;
    void splay(ZoekKnoop<Sleutel, Data> *c);
    ZoekKnoop<Sleutel, Data> *verwijder(const Sleutel &sleutel) override;
};

/******************************************************************************/

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::splay(ZoekKnoop<Sleutel, Data> *c)
{
    if (c->ouder == nullptr) return;

    ZoekKnoop<Sleutel, Data> *p = c->ouder;

    if (p->ouder == nullptr) { // c --> p
        this->roteer(!c->isLinkerKind());
    } else if (p->ouder != nullptr) { // c --> p --> g
        ZoekKnoop<Sleutel, Data> *g = p->ouder;

        if (c->isLinkerKind()) { // c is left child of p
            if (c->ouder->isLinkerKind()) { // p is left child of g
                ZoekBoom<Sleutel, Data>::geefBoomVanKnoop(*g)->roteer(false);
                ZoekBoom<Sleutel, Data>::geefBoomVanKnoop(*p)->roteer(false);
            } else { // p is right child of g
                ZoekBoom<Sleutel, Data>::geefBoomVanKnoop(*p)->roteer(false);
                ZoekBoom<Sleutel, Data>::geefBoomVanKnoop(*g)->roteer(true);
            }
        } else { // c is right child of p
            if (c->ouder->isLinkerKind()) { // p is left child of g
                ZoekBoom<Sleutel, Data>::geefBoomVanKnoop(*p)->roteer(true);
                ZoekBoom<Sleutel, Data>::geefBoomVanKnoop(*g)->roteer(false);
            } else { // p is right child of g
                ZoekBoom<Sleutel, Data>::geefBoomVanKnoop(*g)->roteer(true);
                ZoekBoom<Sleutel, Data>::geefBoomVanKnoop(*p)->roteer(true);
            }
        }

        this->splay(c);
    }
}

template <class Sleutel, class Data>
bool Splayboom<Sleutel, Data>::zoekSleutel(const Sleutel &sleutel)
{
    auto [plaats,ouder] = ZoekBoom<Sleutel, Data>::zoek(sleutel);

    if (*plaats != nullptr) {
        this->splay(plaats->get());
        return true;
    } else {
        if (ouder) {
            this->splay(ouder);
        }
        return false; 
    }
}

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::voegtoe(const Sleutel &sleutel, const Data &data)
{
    ZoekBoom<Sleutel, Data>::voegtoe(sleutel, data);
    auto [plaats,ouder] = ZoekBoom<Sleutel, Data>::zoek(sleutel);
    this->splay(plaats->get());
}

template <class Sleutel, class Data>
ZoekKnoop<Sleutel, Data> *Splayboom<Sleutel, Data>::verwijder(const Sleutel &sleutel)
{
    ZoekKnoop<Sleutel, Data> *ouder = ZoekBoom<Sleutel, Data>::verwijder(sleutel);
    if (ouder != nullptr) {
        this->splay(ouder);
    }
    return ouder;
}

#endif