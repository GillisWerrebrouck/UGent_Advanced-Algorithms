#include "automaten.h"
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <set>
#include <map>
#include <stack>
#include <unordered_map>
#include <algorithm>
#include <vector>

using namespace std;

const char epsilon = 0;

NA::NA(const Regexp &regexp) {
    switch (regexp.geefOpcode()) {
        case Regexp::letter: {
            voegVerbindingToe(0, 1, regexp.geefLetter());
            aantalKnopen = 2;
            break;
        }
        case Regexp::ster: {
            NA eersteOperand(*regexp.geefEersteOperand());
            this->aantalKnopen = eersteOperand.aantalKnopen + 2;
            voegSamen(eersteOperand, 1);
            voegVerbindingToe(0, 1, epsilon);
            voegVerbindingToe(0, eersteOperand.aantalKnopen, epsilon);
            voegVerbindingToe(this->aantalKnopen - 2, this->aantalKnopen - 1, epsilon);
            voegVerbindingToe(this->aantalKnopen - 2, 1, epsilon);
            break;
        }
        case Regexp::of: {
            NA eersteOperand(*regexp.geefEersteOperand());
            NA tweedeOperand(*regexp.geefTweedeOperand());
            this->aantalKnopen = eersteOperand.aantalKnopen + tweedeOperand.aantalKnopen + 2;
            voegSamen(eersteOperand, 1);
            voegSamen(tweedeOperand, eersteOperand.aantalKnopen + 1);
            voegVerbindingToe(0, 1, epsilon);
            voegVerbindingToe(0, eersteOperand.aantalKnopen + 1, epsilon);
            voegVerbindingToe(eersteOperand.aantalKnopen, this->aantalKnopen - 1, epsilon);
            voegVerbindingToe(this->aantalKnopen - 2, this->aantalKnopen - 1, epsilon);
            break;
        }
        case Regexp::concatenatie: {
            NA eersteOperand(*regexp.geefEersteOperand());
            NA tweedeOperand(*regexp.geefTweedeOperand());
            this->aantalKnopen = eersteOperand.aantalKnopen + tweedeOperand.aantalKnopen - 1;
            voegSamen(eersteOperand, 0);
            voegSamen(tweedeOperand, eersteOperand.aantalKnopen - 1);
            break;
        }
    }
}

void NA::voegSamen(const NA &na, int verschuiving) {
    for (int i = 0; i < na.verbindingen.size(); i++) {
        const Verbinding &temp = na.verbindingen[i];
        verbindingen.push_back(Verbinding(temp.start + verschuiving, temp.eind + verschuiving, temp.symbool));
    }

    auto it = na.invoersymbolen.begin();
    while (it != na.invoersymbolen.end()) {
        this->invoersymbolen.insert(*it);
        it++;
    }
}

void NA::voegVerbindingToe(int van, int naar, char symbool) {
    Verbinding verb(van, naar, symbool);
    this->verbindingen.push_back(verb);
    if (symbool != epsilon) {
        this->invoersymbolen.insert(symbool);
    }
}

void NA::schrijf() {
    for (int i = 0; i < verbindingen.size(); i++) {
        cout << verbindingen[i].start << "--";
        
        if (verbindingen[i].symbool == epsilon)
            cout << "ε";
        else
            cout << verbindingen[i].symbool;
        
        cout << "-->" << verbindingen[i].eind << endl;
    }
}

set<int> DA::sluiting(const set<int> &T) {
    set<int> e_sluiting;
    stack<int> DEZstack;

    auto it = T.begin();
    while (it != T.end()) {
        e_sluiting.insert(*it);
        DEZstack.push(*it);
        it++;
    }

    while (!DEZstack.empty()) {
        int bron = DEZstack.top();
        DEZstack.pop();

        for (int v = 0; v < na.verbindingen.size(); v++) {
            if (na.verbindingen[v].start == bron && na.verbindingen[v].symbool == epsilon && e_sluiting.find(na.verbindingen[v].eind) == e_sluiting.end()) {
                e_sluiting.insert(na.verbindingen[v].eind);
                DEZstack.push(na.verbindingen[v].eind);
            }
        }
    }

    return e_sluiting;
}

DA::DA(const Regexp &regexp) : na(regexp) {
    stack<set<int>> DEZ_stack_sets;
    map<set<int>, int> volgnummer;

    set<int> s0;
    s0.insert(0);
    set<int> T = sluiting(s0);

    int nieuwe_staat_nr = 0;
    volgnummer.insert(pair<set<int>, int>(T, nieuwe_staat_nr++));
    DEZ_stack_sets.push(T);

    while(!DEZ_stack_sets.empty()) {
        T = DEZ_stack_sets.top();
        DEZ_stack_sets.pop();
        
        auto it = na.invoersymbolen.begin();
        while (it != na.invoersymbolen.end()) {
            set<int> U = sluiting(overgang(T, *it));
            if (volgnummer.find(U) == volgnummer.end()) {
                volgnummer.insert(pair<set<int>, int>(U, nieuwe_staat_nr++));
                DEZ_stack_sets.push(U);
            }

            this->matrix.resize(max((int)this->matrix.size(), volgnummer.find(T)->second + 1));
            this->matrix[volgnummer.find(T)->second].insert(pair<char, int>(*it, volgnummer.find(U)->second));

            it++;
        }
    }
    
    auto it = volgnummer.begin();
    while(it != volgnummer.end()) {
        if (it->first.find(na.aantalKnopen - 1) != it->first.end()) {
            this->eindstaten.insert(it->second);
        }

        it++;
    }
}

void DA::schrijf() {
    auto start_staat = this->matrix.begin();
    for (int start_staat = 0; start_staat < this->matrix.size(); start_staat++) {
        auto eind_staat = this->matrix[start_staat].begin();
        while (eind_staat != this->matrix[start_staat].end()) {
            cout << start_staat << " --" << eind_staat->first << "--> " << eind_staat->second  << endl;
            eind_staat++;
        }
    }
}

set<int> DA::overgang(const set<int> &T, char symbool) {
    auto it = T.begin();

    set<int> result;
    
    while (it != T.end()) {
        for (int v = 0; v < na.verbindingen.size(); v++) {
            if (na.verbindingen[v].start == *it && na.verbindingen[v].symbool == symbool) {
                result.insert(na.verbindingen[v].eind);
            }
        }
        it++;
    }

    return result;
}

bool DA::zitInTaal(const string& s) {
    int staat = 0;

    for (int i = 0; i < s.size(); i++) {
        if (this->matrix[staat].find(s[i]) == this->matrix[staat].end()) {
            return false;
        }
        staat = this->matrix[staat].find(s[i])->second;
    }

    return eindstaten.find(staat) != eindstaten.end();
}

Verbinding::Verbinding(int start, int eind, char symbool) {
    this->start = start;
    this->eind = eind;
    this->symbool = symbool;
}
