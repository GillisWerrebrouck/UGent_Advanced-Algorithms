#include "regexp.h"
#include <vector>
#include <set>
#include <map>

using namespace std;

class Verbinding {

    friend class NA;
    friend class DA;

    public:
        Verbinding(int van, int naar, char symbool);

    private:
        int start;
        int eind;
        char symbool;
};

class NA {

    friend class DA;

    public:
        NA(const Regexp &);

        void voegVerbindingToe(int van, int naar, char symbool);
        void voegSamen(const NA &na, int verschuiving);
        void schrijf();

    private:
        int start;
        int eind;
        int aantalKnopen;
        vector<Verbinding> verbindingen;
        set<char> invoersymbolen;
};

class DA {
    public:
        DA(const Regexp &);

        bool zitInTaal(const string&);
        set<int> sluiting(const set<int> &T);
        set<int> overgang(const set<int> &T, char symbool);
        void schrijf();

    private:
        NA na;
        vector<map<char, int>> matrix;
        set<int> eindstaten;
};
