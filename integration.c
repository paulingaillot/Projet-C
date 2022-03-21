#include "fir.h"
#include "iir.h"
#include "mesure.h"
#include "affichage.h"
#include "lecture.h"

void integrationTest(char* filename) {
    absorp tmp;
    int etat;

    FILE* read = initFichier(filename);
    tmp = lireFichier(read,&etat);

    while(etat != EOF) { //on enregistre dans le fichier data les valeurs d'un signal qui a été filtre pour éliminer certaines composantes
        affichage(mesure(filtreIIR(FIR(tmp)))); //écriture du SPO2 et du pouls dans le fichier data à partir du signal absorp qui a été recadré autour de 0
        tmp = lireFichier(read,&etat); //mise à jour de la valeur lu dans le fichier
        //printf("%d %d %d %d\n", (int)myAbsorp.acr, myAbsorp.dcr, myAbsorp.acir, myAbsorp.dcir);
    }
    finFichier(read); //fermeture du fichier
}