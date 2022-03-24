/**
 * @file MatrixDiplayNumbers.h
 * @author Pierro
 * @brief ProjetCO²
 * @version 1.0.0
 * @date 2022-03-15
 * @contributors: Julie Brindejont (julie.brindejont@gmail.com)
 *  * 
 * Affichage de chiffres aux dimensions 3x5 leds
 * @see 
 * @CC-BY-SA - 2022
 */
#ifndef MATRIX_DIPLAY_NUMBERS_H
#define MATRIX_DIPLAY_NUMBERS_H

#include <M5Atom.h>

#define TEMPO_AFFICHAGE 400      //Le temps d'affichage d'un chiffre en millisecondes
#define ENTRE_CHIFFRES 75        //Le temps de la pause entre chaque chiffre en millisecondes

//nombre de chiffre, nombre colonne, nombre pixel par colonne
const int chiffres[10][5][5] = {
    {   //0
        {0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0},
    }, //0
    {   //1
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0},
    }, //1
    {   //2
        {0, 0, 0, 0, 0},
        {1, 0, 1, 1, 1},
        {1, 0, 1, 0, 1},
        {1, 1, 1, 0, 1},
        {0, 0, 0, 0, 0},
    }, //2
    {   //3
        {0, 0, 0, 0, 0},
        {1, 0, 0, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0},
    }, //3
    {   //4
        {0, 0, 0, 0, 0},
        {1, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0},
    }, //4
    {   //5
        {0, 0, 0, 0, 0},
        {1, 1, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 1, 1},
        {0, 0, 0, 0, 0},
    }, //5
    {   //6
        {0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 1, 1},
        {0, 0, 0, 0, 0},
    }, //6
    {   //7
        {0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0},
    }, //7
    {   //8
        {0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1},
        {1, 0, 1, 0, 1},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0},
    }, //8
    {   //9
        {0, 0, 0, 0, 0},
        {1, 1, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0},
    }, //9
};
const int chiffresSave[10][25] = {
    {0,1,1,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,1,1,0},    //0
    {0,0,0,1,0,0,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0},    //1
    {0,1,1,1,0,0,0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,1,1,1,0},    //2
    {0,1,1,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,1,1,1,0},    //3
    {0,1,0,1,0,0,1,0,1,0,0,1,1,1,0,0,0,0,1,0,0,0,0,1,0},    //4
    {0,1,1,1,0,0,1,0,0,0,0,1,1,1,0,0,0,0,1,0,0,1,1,1,0},    //5
    {0,1,1,1,0,0,1,0,0,0,0,1,1,1,0,0,1,0,1,0,0,1,1,1,0},    //6
    {0,1,1,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0},    //7
    {0,1,1,1,0,0,1,0,1,0,0,1,1,1,0,0,1,0,1,0,0,1,1,1,0},    //8
    {0,1,1,1,0,0,1,0,1,0,0,1,1,1,0,0,0,0,1,0,0,1,1,1,0}     //9
};

void drawArray(int nb, int colors[] )
{
    int tableTmp[25];
    int i = 0;
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            M5.dis.drawpix(i, colors[chiffres[nb][y][x]]);
            i++;
        }
    }
}

void displayCO2(String co2,int colorList[]) {
    int co2Length = co2.length();
    bool first = true;

    for(int i = 0; i < co2Length; i++) {
        char currentChar = co2.charAt(i);

        //Important: l'instruction ci dessous permet de traduire le caractère ci-dessus en entier.
        //nécessaire pour récupérer l'id du tableau voir plus haut.
        int currentCharInt = currentChar - '0';
        M5.dis.clear();
        delay(ENTRE_CHIFFRES);
        /*if (first) {
            drawArray(currentCharInt, colorList);
            first = false;
        } else {
            //tentative pour créer un défilement.
            //Idée, récupérer le chiffre précédent
            //Crée une copie du tableau et décaler
        }*/
            drawArray(currentCharInt, colorList);
        
       
        delay(TEMPO_AFFICHAGE);
    }
}
#endif
