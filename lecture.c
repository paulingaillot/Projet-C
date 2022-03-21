#include "lecture.h"
#include "ftd2xx.h"
#include "fir.h"
#include "iir.h"
#include "mesure.h"
#include "affichage.h"


absorp lecture(FILE* file_pf, int* file_state){
    absorp myAbsorp = {0};

    myAbsorp = lireFichier(file_pf, file_state); //lecture de l'absorption
    if(*file_state == 6) { //on v�rifie que les champs sont correctement remplis
        myAbsorp.acr = myAbsorp.acr-2048; //recentrage autour de la valeur captur�e autour de 0
        myAbsorp.dcr = myAbsorp.dcr-2048; //recentrage autour de la valeur captur�e autour de 0
        return myAbsorp; //retourne la nouvelle valeur de absorp apr�s recentrage
    } else if(*file_state == EOF) {
        return myAbsorp;
    } else {
        return lecture(file_pf, file_state); //si la valeur de lecture est erronn�e, on appelle par r�cursion une nouvelle lecture des donn�es
    }
}

absorp lectureUSB(char* RxBuffer, int* file_state) {
    absorp myAbsorp = {0};

    char x,y = 'a';
    int a=0,b=0,c=0,d=0;
    *file_state = sscanf(RxBuffer,"%d,%d,%d,%d%c%c",&a,&b,&c,&d,&x,&y); //on lit les donn�es recues au format string
    if(*file_state == 6 && a>1000) { //on v�rifie que les champs sont correctement remplis et que la premiere valeur lu poss�de bien 4 chiffres
        myAbsorp.acr=a-2048; //recentrage autour de 0
        myAbsorp.dcr=b;
        myAbsorp.acir=c-2048; //recentrage autour de 0
        myAbsorp.dcir=d;
        printf("%d,%d,%d,%d\n",a,b,c,d);
        oxy myoxy = mesure(filtreIIR(FIR(myAbsorp)));
        myoxy.pouls /= 2;
        affichage(myoxy); //permet d'�crire dans le fichier data.txt pour que ce fichier soit lu par le script java
        return myAbsorp;
    } else { //la premiere valeur ne poss�de pas 4 chiffres
        char car = 'a';
        int i =-1;
        while(car != 13) { //on cherche dans le buffer la fin de la ligne avec les donn�es erron�es qui se finit par '\n' soit 13 en ASCII
            i++;
            car = RxBuffer[i]; //on enregistre le nouveau caract�re pour ensuite verifier s'il s'agit d'un '\n'
        }
        i++; //permet de savoir � partir de quelle position dans le buffer les donn�es sont correctes

        char tab[256] = {'\000'}; //creation d'un nouveau tableau comme nouveau buffer
        int j=-1;
        while(car != '\000') {
            j++;
            tab[j] = RxBuffer[i+j]; //on copie l'ancien buffer dans un nouveau tableau, � partir de la position o� les donnes sont correctes
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
    char RxBuffer[256] = {'\000'}; // Chaine de caract�re resultat

    if(ftHandle == NULL) ftStatus = FT_Open(0, &ftHandle); // On s�lectionne notre appareil si celui ci n'est pas d�j� s�lectionn�

    FT_SetTimeouts(ftHandle,500,0); // On choisit une valeur de timeout (temps maximal d'execution)
    ftStatus = FT_Read(ftHandle,RxBuffer,RxBytes,&BytesReceived); // On lit les donn�es re�ues

        if (BytesReceived == RxBytes) {
            // Lecture r�ussie
            // FT_Read OK
            int file_state=0;

            absorp myAbsorp = lectureUSB(RxBuffer, &file_state); //on r�cup�re la valeur d'Absorp correspondante
            return myAbsorp;
        } else {
            //Lecture �chou�e (Carte non branch�)

            printf("ca va pas\n");
            FT_Close(ftHandle);
            return connexionUSB();
            // FT_Read Timeout
        }
}