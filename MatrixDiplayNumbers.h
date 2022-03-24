/**
 * @file MatrixDiplayNumbers.h
 * @author Pierro, Julie Brindejont (julie.brindejont@gmail.com)
 * @brief ProjetCO²
 * @version 1.0.0
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 * Affichage de chiffres aux dimensions 3x5 leds
 * @see 
 */
#ifndef MATRIX_DIPLAY_NUMBERS_H
#define MATRIX_DIPLAY_NUMBERS_H

#include <M5Atom.h>

#define TEMPO_AFFICHAGE 400      //Le temps d'affichage d'un chiffre en millisecondes
#define ENTRE_CHIFFRES 75        //Le temps de la pause entre chaque chiffre en millisecondes

int chiffres[10][25] = {
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


void drawArray(int color, int colors[] )
{
  for(int i = 0; i < 25; i++)
  {
      M5.dis.drawpix(i, colors[chiffres[color][i]]);
  }
}

void displayCO2(String co2,int colorList[]) {
    int co2Length = co2.length();

    for(int i = 0; i < co2Length; i++) {
        char currentChar = co2.charAt(i);

        //Important: l'instruction ci dessous permet de traduire le caractère ci-dessus en entier.
        //nécessaire pour récupérer l'id du tableau voir plus haut.
        int currentCharInt = currentChar - '0';
        M5.dis.clear();
        delay(ENTRE_CHIFFRES);
        drawArray(currentCharInt, colorList);
       
        delay(TEMPO_AFFICHAGE);
    }
}
#endif
