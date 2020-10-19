#include "rzwknoop.h"


int main() 
{
  // boom 2 (eenvoudige boom): 
  std::cout << "Eenvoudige boom : " << std::endl;
  RZWboom<int> boom;
  int punten[10] = {8,4,2,1,3,6,5,7,12, 10}; 
  for (int i:punten) {
    boom.voegtoe(i);
  }

  boom.schrijf(std::cout);
  boom.roteer(true);
  boom.tekenAlsBinaireBoom("boom2.dot");


  // Deel woordenboom
  RZWboom<string> woordenboom;
  string bestandsnaam = "Ovidius", woord;
  std::ifstream file;
  file.open (bestandsnaam);
  int teller = 0, toegevoegd = 0;
  if (!file.is_open()){ std::cout << "Fout bij openen bestand";}
  else{
    while (file >> woord){
        if (!woordenboom.zoekSleutel(woord)){
          woordenboom.voegtoe(woord);
          toegevoegd++;
        }
        teller++;
    }
  }
  std::cout << "Je voegde " << toegevoegd << " unieke woorden toe aan de boom!\n"; 
  std::cout << "De boom van de tekst heeft nu diepte: " << woordenboom.geefDiepte() << std::endl;


  return 0;
}