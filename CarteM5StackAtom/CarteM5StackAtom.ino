/**
 * @file CarteM5StackAtom.ino
 * @author Julie Brindejont (julie.brindejont)
 * @brief 
 * @version 0.1
 * @date 2021-12-16
 * 
 * @copyright Copyright (c) 2021
 * 
 * 
 * Toutes les 2 secondes environ le status du capteur doit être vérifié.
 * afin de parrer à d'éventuels défaut détecter.
 * 
 * Identification des paquet 
 * tout paquet démarre 
 */

#include "config.h"
#include "api_senseair.h"
//Fichier pincipal pour la Carte M5

void setup() {
    // bouton de calibration
    //Si j'ai bien compris il n'y aura pas de bouton de calibration
    //en conséquence je désactive cette instruction
    // pinMode(BOUTON_CAL, INPUT);

    // ports série de debug et de communication capteur
    Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8N1, PIN_SERIAL_RX, PIN_SERIAL_TX);

    
    
}

String InputString;
int stringComplete = 0;
byte ABCreq[] = { 0xFE, 0X03, 0X00, 0X1F, 0X00, 0X01, 0XA1, 0XC3 };   // 1f in Hex -> 31 dezimal

void loop() {
    
    //J'envoie la requete ping à la carte arduinp
    Serial1.write(ABCreq, 8);
    Serial.println("Requet envoyé : Ping!");

    //J'écoute ce que la carte me répond.
    if(Serial1.available()) {
        while(Serial1.available()) {
            //Récupération du byte
            byte inByte = Serial1.read();
            //Ajoutons-le à InputString
            InputString += (char)inByte;

            if((char)inByte == '!') {
                stringComplete = 1;
            }
        }
    }

    //Enfin, je traite les infos reçus.
    if(stringComplete) {
        Serial.print("Réponse reçus : ");
        Serial.println(InputString);
        stringComplete = 0;
        InputString = "";
    }
    delay(10000);
}