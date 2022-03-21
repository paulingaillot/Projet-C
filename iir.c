#include "iir.h"
absorp  lastAbsorp = {0}, lastIIR = {0};

absorp iirTest(char* filename){
	absorp myAbsorp, tmp;
    int etat;

    FILE* readFIR = initFichier(filename);
    tmp = lireFichier(readFIR,&etat);

    while(etat != EOF) {
        myAbsorp = filtreIIR(tmp); //opération de filtrage
        tmp = lireFichier(readFIR,&etat); //mise à jour de la valeur lue dans le fichier
        //printf("%d %d %d %d\n", (int)myAbsorp.acr, myAbsorp.dcr, myAbsorp.acir, myAbsorp.dcir);
    }
    finFichier(readFIR);
    return myAbsorp;
}

absorp filtreIIR(absorp myAbsort) {
    absorp tmp; //valeur absorp retournée après filtrage
    tmp.acr = myAbsort.acr - lastAbsorp.acr + 0.992*lastIIR.acr; //filtrage du signal de la led rouge
    tmp.acir = myAbsort.acir - lastAbsorp.acir + 0.992*lastIIR.acir; //filtrage du signal de la led infrarouge
    tmp.dcr = myAbsort.dcr; //copie de la valeur moyenne
    tmp.dcir = myAbsort.dcir; //copie de la valeur moyenne
    lastIIR = tmp; //on enregistre la valeur IIR qui va être retournée après filtrage
    lastAbsorp = myAbsort; //on enregistre la dernière valeur absorp reçue sans le filtrage
    return tmp; //on retourne la valeur absorp filtrée qui devient l'IIR
}


