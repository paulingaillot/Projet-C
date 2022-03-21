#include "fir.h"
#include "fichiers.h"

absorp x[51];

absorp firTest(char* filename){    // Lecture Fichier
    // On ouvre le fichier souhaité
    FILE* fp = initFichier(filename);
    absorp absorpX;
    absorp absorpY;

    // absorp absorpX; // Valeur d'absorption de base
    int etat = 1; // etat de la lecture
    while (etat != EOF) {
        absorpX = lireFichier(fp, &etat); // On lit la ligne et on recupere la valeur absorp correspondante
        if(etat != EOF) {
            absorpY = FIR(absorpX); //filtrage des composantes de absorp
        }
    }
    return absorpY;
}

float FIR_TAPS[51] = { //filtre impulsionnelle du filtre fir
        1.4774946e-019,
        1.6465231e-004,
        3.8503956e-004,
        7.0848037e-004,
        1.1840522e-003,
        1.8598621e-003,
        2.7802151e-003,
        3.9828263e-003,
        5.4962252e-003,
        7.3374938e-003,
        9.5104679e-003,
        1.2004510e-002,
        1.4793934e-002,
        1.7838135e-002,
        2.1082435e-002,
        2.4459630e-002,
        2.7892178e-002,
        3.1294938e-002,
        3.4578348e-002,
        3.7651889e-002,
        4.0427695e-002,
        4.2824111e-002,
        4.4769071e-002,
        4.6203098e-002,
        4.7081811e-002,
        4.7377805e-002,
        4.7081811e-002,
        4.6203098e-002,
        4.4769071e-002,
        4.2824111e-002,
        4.0427695e-002,
        3.7651889e-002,
        3.4578348e-002,
        3.1294938e-002,
        2.7892178e-002,
        2.4459630e-002,
        2.1082435e-002,
        1.7838135e-002,
        1.4793934e-002,
        1.2004510e-002,
        9.5104679e-003,
        7.3374938e-003,
        5.4962252e-003,
        3.9828263e-003,
        2.7802151e-003,
        1.8598621e-003,
        1.1840522e-003,
        7.0848037e-004,
        3.8503956e-004,
        1.6465231e-004,
        1.4774946e-019
};

absorp FIR(absorp absorpX) {
    int i;
    absorp myAbsorp;

    for(i=0;i<50; i++) {
        x[i] = x[i+1]; //décallage de toutes les valeurs d'un cran vers la gauche
    }
    x[50] = absorpX; //on enregistre la dernière valeur de absorp reçue dans le tableau


    int j=51; // taille du tableau
    myAbsorp.acr=0; // on defini a zero notre valeur de base d'ACR
    myAbsorp.acir=0; // on defini a zero notre valeur de base d'ACIR

    // On parcours nous 51 dernières valeurs afin d'y appliquer le filtre
    for(j; j>0; j--) {
        // On applique la fonction de filtrage sur l'ACR
        myAbsorp.acr+=FIR_TAPS[j]*x[50-j].acr;
        // On applique la fonction de filtrage sur l'ACIR
        myAbsorp.acir+=FIR_TAPS[j]*x[50-j].acir;
    }

    //On enregistre les nouvelles valeurs d'ACR et d'ACIR
    myAbsorp.acr = (int)myAbsorp.acr; // On arondi notre valeur d'ACR calculé
    myAbsorp.acir = (int)myAbsorp.acir; // On arondi notre valeur d'ACIR calculé
    myAbsorp.dcr = absorpX.dcr; // Les valeur de DCR et de DCIR restent inchangés
    myAbsorp.dcir = absorpX.dcir;
    return myAbsorp;
}