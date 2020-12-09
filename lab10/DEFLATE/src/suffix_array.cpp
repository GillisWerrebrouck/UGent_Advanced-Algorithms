#include "suffix_array.h"
#include <algorithm>
#include <iostream>

using namespace std;

SuffixArray::SuffixArray(const unsigned char *T, int len) : vector<int>(len), LCP(len - 1, 0), lookup(len), lengte_woord(len)
{
    saisxx(T, begin(), len);

    int current_index;
    int prev_index;
    int lcp_index = 0;

    auto it_current = this->begin();
    auto it_prev = this->begin();
    it_current++;

    while (it_current != this->end()) {
        current_index = *it_current;
        prev_index = *it_prev;

        while(current_index < len && prev_index < len && T[current_index] == T[prev_index]) {
            current_index++;
            prev_index++;
        }

        this->LCP[lcp_index] = current_index - *it_current;

        lookup[this->operator[](lcp_index)] = lcp_index;
        
        it_current++;
        it_prev++;
        lcp_index++;
    }
};

SuffixArray::SuffixArray(const std::string &s) : SuffixArray((unsigned char *)s.c_str(), s.size()) {}

std::pair<int, int> SuffixArray::zoekHerhaling(size_t search_buffer_begin, size_t search_buffer_lengte, size_t lookahead_buffer_lengte) {
    // start van het groen window
    int groen_start = search_buffer_begin + search_buffer_lengte;
    
    int positie = lookup[groen_start];

    int vorige = positie;
    int vorige_prefix = positie == 0 ? 0 : this->LCP[vorige - 1];
    // zoek eerste vorige in LCP die voldoet om te matchen
    while (0 < vorige && this->operator[](positie) <= this->operator[](vorige)) {
        --vorige;
        vorige_prefix = min(vorige_prefix, this->LCP[vorige]);
    }

    int volgende = positie + 1;
    int volgende_prefix = this->LCP[positie];
    // zoek eerste volgende in LCP die voldoet om te matchen
    while (volgende < this->lengte_woord && this->operator[](positie) <= this->operator[](volgende)) {
        volgende_prefix = min(volgende_prefix, this->LCP[volgende]);
        volgende++;
    }

    int afstand = 0;
    int lengte = 0;
    if ((vorige_prefix < 3 && volgende_prefix < 3) || (this->operator[](positie) <= this->operator[](volgende) && this->operator[](positie) <= this->operator[](vorige))) {
        afstand = lengte = 0;
    } else if (vorige_prefix < volgende_prefix || this->operator[](positie) <= this->operator[](vorige)) {
        afstand = this->operator[](positie) - this->operator[](volgende);
        lengte = volgende_prefix;
    } else {
        afstand = this->operator[](positie) - this->operator[](vorige);
        lengte = vorige_prefix;
    }
    
    // de gevonden herhaling mag niet buiten het groen window vallen
    lengte = min(lengte, (int)lookahead_buffer_lengte);

    return pair<int, int>(lengte, afstand);
}

void SuffixArray::schrijf() {
    auto it = this->begin();
    int i = 0;
    while (it != this->end()) {
        cout << i << " | " << (i == 0 ? "." : to_string(this->LCP[i - 1])) << " | " << *it << endl;
        i++;
        it++;
    }
}
