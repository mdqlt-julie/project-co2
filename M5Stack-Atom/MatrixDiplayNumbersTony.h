/**
 * @file MatrixDisplayNumbers.h
 * @author Pierro
 * @brief Projet CO² Labfab
 * @contributor : Tony Vanpoucke (animations).
 * @version 1.1.0
 * @date 2022-02-24
 *
 * Affichage de chiffres aux dimensions 3x5 leds
 * 
 * @CC-BY-SA - 2022
 */

#include "M5Atom.h"

#define TEMPO_AFFICHAGE 400      //Le temps d'affichage d'un chiffre en millisecondes
#define ENTRE_CHIFFRES 75        //Le temps de la pause entre chaque chiffre en millisecondes

void displayMatrix(String co2,int colorVal) {
  int co2Length = co2.length();

  for(int i = 0; i < co2Length; i++)
  {
    char currentChar = co2.charAt(i);

    if(currentChar == '0') { 
      M5.dis.clear();
      delay(ENTRE_CHIFFRES);
      M5.dis.drawpix( 0, CHSV( 0, 0, 0)); 
      M5.dis.drawpix( 1, CHSV( colorVal, 255, 255)); 
      M5.dis.drawpix( 2, CHSV( colorVal, 255, 255)); 
      M5.dis.drawpix( 3, CHSV( colorVal, 255, 255)); 
      M5.dis.drawpix( 4, CHSV( 0, 0, 0));
      M5.dis.drawpix( 5, CHSV( 0, 0, 0)); 
      M5.dis.drawpix( 6, CHSV( colorVal, 255, 255)); 
      M5.dis.drawpix( 7, CHSV( 0, 0, 0)); 
      M5.dis.drawpix( 8, CHSV( colorVal, 255, 255)); 
      M5.dis.drawpix( 9, CHSV( 0, 0, 0));
      M5.dis.drawpix( 10, CHSV( 0, 0, 0)); 
      M5.dis.drawpix( 11, CHSV( colorVal, 255, 255)); 
      M5.dis.drawpix( 12, CHSV( 0, 0, 0)); 
      M5.dis.drawpix( 13, CHSV( colorVal, 255, 255)); 
      M5.dis.drawpix( 14, CHSV( 0, 0, 0));
      M5.dis.drawpix( 15, CHSV( 0, 0, 0)); 
      M5.dis.drawpix( 16, CHSV( colorVal, 255, 255)); 
      M5.dis.drawpix( 17, CHSV( 0, 0, 0)); 
      M5.dis.drawpix( 18, CHSV( colorVal, 255, 255)); 
      M5.dis.drawpix( 19, CHSV( 0, 0, 0));
      M5.dis.drawpix( 20, CHSV( 0, 0, 0)); 
      M5.dis.drawpix( 21, CHSV( colorVal, 255, 255)); 
      M5.dis.drawpix( 22, CHSV( colorVal, 255, 255)); 
      M5.dis.drawpix( 23, CHSV( colorVal, 255, 255)); 
      M5.dis.drawpix( 24, CHSV( 0, 0, 0));
      delay(TEMPO_AFFICHAGE);
    }
    else if(currentChar == '1')
    { 
      M5.dis.clear();
      delay(ENTRE_CHIFFRES);
      M5.dis.drawpix( 0, CHSV( 0, 0, 0)); M5.dis.drawpix( 1, CHSV( 0, 0, 0)); M5.dis.drawpix( 2, CHSV( 0, 0, 0)); M5.dis.drawpix( 3, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 4, CHSV( 0, 0, 0));
      M5.dis.drawpix( 5, CHSV( 0, 0, 0)); M5.dis.drawpix( 6, CHSV( 0, 0, 0)); M5.dis.drawpix( 7, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 8, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 9, CHSV( 0, 0, 0));
      M5.dis.drawpix( 10, CHSV( 0, 0, 0)); M5.dis.drawpix( 11, CHSV( 0, 0, 0)); M5.dis.drawpix( 12, CHSV( 0, 0, 0)); M5.dis.drawpix( 13, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 14, CHSV( 0, 0, 0));
      M5.dis.drawpix( 15, CHSV( 0, 0, 0)); M5.dis.drawpix( 16, CHSV( 0, 0, 0)); M5.dis.drawpix( 17, CHSV( 0, 0, 0)); M5.dis.drawpix( 18, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 19, CHSV( 0, 0, 0));
      M5.dis.drawpix( 20, CHSV( 0, 0, 0)); M5.dis.drawpix( 21, CHSV( 0, 0, 0)); M5.dis.drawpix( 22, CHSV( 0, 0, 0)); M5.dis.drawpix( 23, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 24, CHSV( 0, 0, 0));
      delay(TEMPO_AFFICHAGE);
    }
    else if(currentChar == '2')
    { 
      M5.dis.clear();
      delay(ENTRE_CHIFFRES);
      M5.dis.drawpix( 0, CHSV( 0, 0, 0)); M5.dis.drawpix( 1, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 2, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 3, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 4, CHSV( 0, 0, 0));
      M5.dis.drawpix( 5, CHSV( 0, 0, 0)); M5.dis.drawpix( 6, CHSV( 0, 0, 0)); M5.dis.drawpix( 7, CHSV( 0, 0, 0)); M5.dis.drawpix( 8, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 9, CHSV( 0, 0, 0));
      M5.dis.drawpix( 10, CHSV( 0, 0, 0)); M5.dis.drawpix( 11, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 12, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 13, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 14, CHSV( 0, 0, 0));
      M5.dis.drawpix( 15, CHSV( 0, 0, 0)); M5.dis.drawpix( 16, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 17, CHSV( 0, 0, 0)); M5.dis.drawpix( 18, CHSV( 0, 0, 0)); M5.dis.drawpix( 19, CHSV( 0, 0, 0));
      M5.dis.drawpix( 20, CHSV( 0, 0, 0)); M5.dis.drawpix( 21, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 22, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 23, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 24, CHSV( 0, 0, 0));
      delay(TEMPO_AFFICHAGE);
    }
    else if(currentChar == '3')
    { 
M5.dis.clear();
      delay(ENTRE_CHIFFRES);
      M5.dis.drawpix( 0, CHSV( 0, 0, 0)); M5.dis.drawpix( 1, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 2, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 3, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 4, CHSV( 0, 0, 0));
      M5.dis.drawpix( 5, CHSV( 0, 0, 0)); M5.dis.drawpix( 6, CHSV( 0, 0, 0)); M5.dis.drawpix( 7, CHSV( 0, 0, 0)); M5.dis.drawpix( 8, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 9, CHSV( 0, 0, 0));
      M5.dis.drawpix( 10, CHSV( 0, 0, 0)); M5.dis.drawpix( 11, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 12, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 13, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 14, CHSV( 0, 0, 0));
      M5.dis.drawpix( 15, CHSV( 0, 0, 0)); M5.dis.drawpix( 16, CHSV( 0, 0, 0)); M5.dis.drawpix( 17, CHSV( 0, 0, 0)); M5.dis.drawpix( 18, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 19, CHSV( 0, 0, 0));
      M5.dis.drawpix( 20, CHSV( 0, 0, 0)); M5.dis.drawpix( 21, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 22, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 23, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 24, CHSV( 0, 0, 0));
      delay(TEMPO_AFFICHAGE);
    }
    else if(currentChar == '4')
    {
M5.dis.clear();
      delay(ENTRE_CHIFFRES);
      M5.dis.drawpix( 0, CHSV( 0, 0, 0)); M5.dis.drawpix( 1, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 2, CHSV( 0, 0, 0)); M5.dis.drawpix( 3, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 4, CHSV( 0, 0, 0));
      M5.dis.drawpix( 5, CHSV( 0, 0, 0)); M5.dis.drawpix( 6, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 7, CHSV( 0, 0, 0)); M5.dis.drawpix( 8, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 9, CHSV( 0, 0, 0));
      M5.dis.drawpix( 10, CHSV( 0, 0, 0)); M5.dis.drawpix( 11, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 12, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 13, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 14, CHSV( 0, 0, 0));
      M5.dis.drawpix( 15, CHSV( 0, 0, 0)); M5.dis.drawpix( 16, CHSV( 0, 0, 0)); M5.dis.drawpix( 17, CHSV( 0, 0, 0)); M5.dis.drawpix( 18, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 19, CHSV( 0, 0, 0));
      M5.dis.drawpix( 20, CHSV( 0, 0, 0)); M5.dis.drawpix( 21, CHSV( 0, 0, 0)); M5.dis.drawpix( 22, CHSV( 0, 0, 0)); M5.dis.drawpix( 23, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 24, CHSV( 0, 0, 0));
      delay(TEMPO_AFFICHAGE);
    }
    else if(currentChar == '5')
    {
M5.dis.clear();
      delay(ENTRE_CHIFFRES);
      M5.dis.drawpix( 0, CHSV( 0, 0, 0)); M5.dis.drawpix( 1, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 2, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 3, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 4, CHSV( 0, 0, 0));
      M5.dis.drawpix( 5, CHSV( 0, 0, 0)); M5.dis.drawpix( 6, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 7, CHSV( 0, 0, 0)); M5.dis.drawpix( 8, CHSV( 0, 0, 0)); M5.dis.drawpix( 9, CHSV( 0, 0, 0));
      M5.dis.drawpix( 10, CHSV( 0, 0, 0)); M5.dis.drawpix( 11, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 12, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 13, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 14, CHSV( 0, 0, 0));
      M5.dis.drawpix( 15, CHSV( 0, 0, 0)); M5.dis.drawpix( 16, CHSV( 0, 0, 0)); M5.dis.drawpix( 17, CHSV( 0, 0, 0)); M5.dis.drawpix( 18, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 19, CHSV( 0, 0, 0));
      M5.dis.drawpix( 20, CHSV( 0, 0, 0)); M5.dis.drawpix( 21, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 22, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 23, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 24, CHSV( 0, 0, 0));
      delay(TEMPO_AFFICHAGE);
    }
    else if(currentChar == '6')
    {
M5.dis.clear();
      delay(ENTRE_CHIFFRES);
      M5.dis.drawpix( 0, CHSV( 0, 0, 0)); M5.dis.drawpix( 1, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 2, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 3, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 4, CHSV( 0, 0, 0));
      M5.dis.drawpix( 5, CHSV( 0, 0, 0)); M5.dis.drawpix( 6, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 7, CHSV( 0, 0, 0)); M5.dis.drawpix( 8, CHSV( 0, 0, 0)); M5.dis.drawpix( 9, CHSV( 0, 0, 0));
      M5.dis.drawpix( 10, CHSV( 0, 0, 0)); M5.dis.drawpix( 11, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 12, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 13, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 14, CHSV( 0, 0, 0));
      M5.dis.drawpix( 15, CHSV( 0, 0, 0)); M5.dis.drawpix( 16, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 17, CHSV( 0, 0, 0)); M5.dis.drawpix( 18, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 19, CHSV( 0, 0, 0));
      M5.dis.drawpix( 20, CHSV( 0, 0, 0)); M5.dis.drawpix( 21, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 22, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 23, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 24, CHSV( 0, 0, 0));
      delay(TEMPO_AFFICHAGE);
    }
    else if(currentChar == '7')
    {
M5.dis.clear();
      delay(ENTRE_CHIFFRES);
      M5.dis.drawpix( 0, CHSV( 0, 0, 0)); M5.dis.drawpix( 1, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 2, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 3, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 4, CHSV( 0, 0, 0));
      M5.dis.drawpix( 5, CHSV( 0, 0, 0)); M5.dis.drawpix( 6, CHSV( 0, 0, 0)); M5.dis.drawpix( 7, CHSV( 0, 0, 0)); M5.dis.drawpix( 8, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 9, CHSV( 0, 0, 0));
      M5.dis.drawpix( 10, CHSV( 0, 0, 0)); M5.dis.drawpix( 11, CHSV( 0, 0, 0)); M5.dis.drawpix( 12, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 13, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 14, CHSV( 0, 0, 0));
      M5.dis.drawpix( 15, CHSV( 0, 0, 0)); M5.dis.drawpix( 16, CHSV( 0, 0, 0)); M5.dis.drawpix( 17, CHSV( 0, 0, 0)); M5.dis.drawpix( 18, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 19, CHSV( 0, 0, 0));
      M5.dis.drawpix( 20, CHSV( 0, 0, 0)); M5.dis.drawpix( 21, CHSV( 0, 0, 0)); M5.dis.drawpix( 22, CHSV( 0, 0, 0)); M5.dis.drawpix( 23, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 24, CHSV( 0, 0, 0));
      delay(TEMPO_AFFICHAGE);
    }
    else if(currentChar == '8')
    {
M5.dis.clear();
      delay(ENTRE_CHIFFRES);
      M5.dis.drawpix( 0, CHSV( 0, 0, 0)); M5.dis.drawpix( 1, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 2, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 3, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 4, CHSV( 0, 0, 0));
      M5.dis.drawpix( 5, CHSV( 0, 0, 0)); M5.dis.drawpix( 6, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 7, CHSV( 0, 0, 0)); M5.dis.drawpix( 8, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 9, CHSV( 0, 0, 0));
      M5.dis.drawpix( 10, CHSV( 0, 0, 0)); M5.dis.drawpix( 11, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 12, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 13, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 14, CHSV( 0, 0, 0));
      M5.dis.drawpix( 15, CHSV( 0, 0, 0)); M5.dis.drawpix( 16, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 17, CHSV( 0, 0, 0)); M5.dis.drawpix( 18, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 19, CHSV( 0, 0, 0));
      M5.dis.drawpix( 20, CHSV( 0, 0, 0)); M5.dis.drawpix( 21, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 22, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 23, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 24, CHSV( 0, 0, 0));
      delay(TEMPO_AFFICHAGE);
    }
    else if(currentChar == '9')
    {
M5.dis.clear();
      delay(ENTRE_CHIFFRES);
      M5.dis.drawpix( 0, CHSV( 0, 0, 0)); M5.dis.drawpix( 1, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 2, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 3, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 4, CHSV( 0, 0, 0));
      M5.dis.drawpix( 5, CHSV( 0, 0, 0)); M5.dis.drawpix( 6, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 7, CHSV( 0, 0, 0)); M5.dis.drawpix( 8, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 9, CHSV( 0, 0, 0));
      M5.dis.drawpix( 10, CHSV( 0, 0, 0)); M5.dis.drawpix( 11, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 12, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 13, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 14, CHSV( 0, 0, 0));
      M5.dis.drawpix( 15, CHSV( 0, 0, 0)); M5.dis.drawpix( 16, CHSV( 0, 0, 0)); M5.dis.drawpix( 17, CHSV( 0, 0, 0)); M5.dis.drawpix( 18, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 19, CHSV( 0, 0, 0));
      M5.dis.drawpix( 20, CHSV( 0, 0, 0)); M5.dis.drawpix( 21, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 22, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 23, CHSV( colorVal, 255, 255)); M5.dis.drawpix( 24, CHSV( 0, 0, 0));
      delay(TEMPO_AFFICHAGE);
    }
  }
}