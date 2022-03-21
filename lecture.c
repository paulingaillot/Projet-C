#include "lecture.h"
#include "ftd2xx.h"
#include "fir.h"
#include "iir.h"
#include "mesure.h"
#include "affichage.h"


absorp lecture(FILE* file_pf, int* file_state){
    absorp myAbsorp = {0};

    myAbsorp = lireFichier(file_pf, file_state); //lecture de l'absorption
    if(*file_state == 6) { //on vérifie que les champs sont correctement remplis
        myAbsorp.acr = myAbsorp.acr-2048; //recentrage autour de la valeur capturée autour de 0
        myAbsorp.dcr = myAbsorp.dcr-2048; //recentrage autour de la valeur capturée autour de 0
        return myAbsorp; //retourne la nouvelle valeur de absorp après recentrage
    } else if(*file_state == EOF) {
        return myAbsorp;
    } else {
        return lecture(file_pf, file_state); //si la valeur de lecture est erronnée, on appelle par récursion une nouvelle lecture des données
    }
}

absorp lectureUSB(char* RxBuffer, int* file_state) {
    absorp myAbsorp = {0};

    char x,y = 'a';
    int a=0,b=0,c=0,d=0;
    *file_state = sscanf(RxBuffer,"%d,%d,%d,%d%c%c",&a,&b,&c,&d,&x,&y); //on lit les données recues au format string
    if(*file_state == 6 && a>1000) { //on vérifie que les champs sont correctement remplis et que la premiere valeur lu possède bien 4 chiffres
        myAbsorp.acr=a-2048; //recentrage autour de 0
        myAbsorp.dcr=b;
        myAbsorp.acir=c-2048; //recentrage autour de 0
        myAbsorp.dcir=d;
        printf("%d,%d,%d,%d\n",a,b,c,d);
        oxy myoxy = mesure(filtreIIR(FIR(myAbsorp)));
        myoxy.pouls /= 2;
        affichage(myoxy); //permet d'écrire dans le fichier data.txt pour que ce fichier soit lu par le script java
        return myAbsorp;
    } else { //la premiere valeur ne possède pas 4 chiffres
        char car = 'a';
        int i =-1;
        while(car != 13) { //on cherche dans le buffer la fin de la ligne avec les données erronées qui se finit par '\n' soit 13 en ASCII
            i++;
            car = RxBuffer[i]; //on enregistre le nouveau caractère pour ensuite verifier s'il s'agit d'un '\n'
        }
        i++; //permet de savoir à partir de quelle position dans le buffer les données sont correctes

        char tab[256] = {'\000'}; //creation d'un nouveau tableau comme nouveau buffer
        int j=-1;
        while(car != '\000') {
            j++;
            tab[j] = RxBuffer[i+j]; //on copie l'ancien buffer dans un nouveau tableau, à partir de la position où les donnes sont correctes
            car = tab[j];
        }
        return lectureUSB(tab, file_state);
    }
}

FT_HANDLE ftHandle= NULL;
FT_STATUS ftStatus = FT_OK;

absorp connexionUSB() {
    DWORD RxBytes = 42; // Nombre de bits a lire
    DWORD BytesReceived; // Nombre de bits lus
    char RxBuffer[256] = {'\000'}; // Chaine de caractère resultat

    if(ftHandle == NULL) ftStatus = FT_Open(0, &ftHandle); // On sélectionne notre appareil si celui ci n'est pas déjà sélectionné

    FT_SetTimeouts(ftHandle,500,0); // On choisit une valeur de timeout (temps maximal d'execution)
    ftStatus = FT_Read(ftHandle,RxBuffer,RxBytes,&BytesReceived); // On lit les données reçues

        if (BytesReceived == RxBytes) {
            // Lecture réussie
            // FT_Read OK
            int file_state=0;

            absorp myAbsorp = lectureUSB(RxBuffer, &file_state); //on récupère la valeur d'Absorp correspondante
            return myAbsorp;
        } else {
            //Lecture échouée (Carte non branché)

            printf("ca va pas\n");
            FT_Close(ftHandle);
            return connexionUSB();
            // FT_Read Timeout
        }
}