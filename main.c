#include "iir.h"
#include "mesure.h"
#include "affichage.h"
#include "integration.h"
#include "lecture.h"
#include "fir.h"

int main() {
    remove(".verrouData");

    //! POUR LE TEST MANUEL AVEC SIMULATEUR
    /*
    int etat;
    FILE* fichier = initFichier("record1_bin.dat");
    absorp donnees = lecture(fichier, &etat);
    affichage(mesure(filtreIIR(FIR(donnees))))
    */

    //! POUR LE TEST MANUEL AVEC CARTE ELECTRONIQUE
    
    while(1) {
        connexionUSB();

    }
    return 0;
}
