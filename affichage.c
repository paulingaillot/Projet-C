#include "affichage.h"

void affichage(oxy myOxy){
        if (access(".verrouData", F_OK) != -1) { // Fichier existe

        } else {
            FILE* fichier = fopen(".verrouData", "w"); //creation du fichier verrouData
            fclose(fichier); //fermeture du fichier

            remove("Data.txt"); //suppression du fichier data qui contient SPO2 et pulse
            FILE* fichier2 = fopen("Data.txt", "w"); //creation d'un nouveau fichier data en mode écriture
            fprintf(fichier2, "%d\n",myOxy.spo2); //écriture du SPO2 dans le fichier data
            fprintf(fichier2, "%d\n",myOxy.pouls); //écriture du pouls dans le fichier data

            fclose(fichier2); //fermeture du fichier data

            remove(".verrouData"); //suppression du verrou pour autoriser la lecture
        }
}