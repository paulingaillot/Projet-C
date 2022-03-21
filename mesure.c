#include "mesure.h"
absorp minAbsorp = {0};
absorp maxAbsorp = {0};
oxy myOxy = {0};

int time1 = 0, time2 = 0, tim2l = 0, tim1l = 0;
int i = 0, val = 1;
int etat;

oxy mesureTest(char* filename){
	oxy myOxy;
    absorp myAbsorp;

    FILE* readIIR = initFichier(filename);
    myAbsorp = lireFichier(readIIR,&etat);

    while(etat != EOF) {
        myOxy = mesure(myAbsorp);
        myAbsorp = lireFichier(readIIR,&etat);
    }

    finFichier(readIIR);

	return myOxy;
}


oxy mesure(absorp myAbsorp) { //on va utiliser le principe de la machine état pour détecter si on est sur un front montant ou descendant
    if (val == 1) { //dans le cas d'un front descendant
        if (myAbsorp.acr >= 0) { //la valeur de pouls est positive par rapport à zéro (car les valeurs ont été centrées autour de 0)
            i++; //incrément du nombre de valeurs lu (permettra de calculer la durée entre 2 changements de valeurs positif/négatif) ce qui correspond à la durée d'une pulsation
            if (myAbsorp.acr > maxAbsorp.acr) {
                maxAbsorp = myAbsorp; //on enregistre la nouvelle valeur maximum pour une pulsation
            }
        }
        if (myAbsorp.acr < 0) { //on passe d'une valeur positive à une valeur négative
            val = 4; //on est sur un front montant en fin de période
            tim1l = time1+1; //valeur pour le début de période
            time1 = i; //valeur pour la fin de période
        }
    } else if (val == 2) {
        if (myAbsorp.acr <= 0) { //dans le cas d'un front montant
            i++;
            if (myAbsorp.acr < minAbsorp.acr) {
                minAbsorp = myAbsorp; //on enregistre la nouvelle valeur minimum pour une pulsation
            }
        }
        if (myAbsorp.acr > 0) { //on passe d'une valeur négative à une valeur positive
            val = 3;  //on est sur un front montant en fin de période
            tim2l = time2+1; //valeur pour le début de période
            time2 = i; //valeur pour la fin de période
        }
    }

    if (val == 3 || val == 4) { //calcul du SPO2 et du pouls lorsqu'on est à la fin de la puslation
        float tmp = ((maxAbsorp.acr - minAbsorp.acr) / ((maxAbsorp.dcr + minAbsorp.dcr) / 2)) /
                    ((maxAbsorp.acir - minAbsorp.acir) / ((maxAbsorp.dcir + minAbsorp.dcir) / 2)); //calcul du ratio RSIR
        if (tmp <= 1) {
            myOxy.spo2 = tmp * (-25) + 110; //fonction de correspondance entre RSIR et SPO2 (de 0.4 à 1)
        } else {
            myOxy.spo2 = tmp * (-35) + 120; //fonction de correspondance entre RSIR et SPO2 (de 1 à 0)
        }

        if(val ==4) {
            minAbsorp.acr = 0, minAbsorp.acir=0; //réinitialiser les valeurs crête à crête du SPO2 pour déterminer le nouveau SPO2 sur une nouvelle pulsation
        }else {
            maxAbsorp.acr = 0, maxAbsorp.acir=0;
        }

        if(val == 3) {
            myOxy.pouls = (int) (60 / ((time2 - tim2l) * 0.002)); //calcul de la pulsation pour un front descendant en fin de période
        } else {
            myOxy.pouls = (int) (60 / ((time1 - tim1l) * 0.002)); //calcul de la pulsation pour un front montant en fin de période
        }
        if(val == 3) val = 1; //si le front est descendant (cad positif) alors il va passer de positif à négatif
        else val = 2; //si le front est montant (cad négatif) alors il va passer de négatif à positif

    }
    return myOxy; //retourne les valeurs de oxy c'est à dire le SPO2 et le pouls
}