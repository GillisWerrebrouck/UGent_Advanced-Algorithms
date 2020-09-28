#include "keten.hpp"
#include <vector>

using namespace std;


int main(int argc, char *argv[]){
    vector<string> lijst;
    Keten K(lijst);
    lijst.push_back("aalbessengelei");
    lijst.push_back("geleidehond");
    lijst.push_back("honderdmaal");
    cout<<K<<std::endl;
    return 0;
}
