/**
 * @file SimulateurCapteur.ino
 * @author Julie Brindejont (julie.brindejont)
 * @brief 
 * @version 0.1
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 * 
 * Fichier principal pour la simulation d'un capteur de CO²
 * 
 */

//Fichier pincipal pour l'arduino

//Inclusion du fichier d'entête pour la configuration de certaine constante et variables.
#include "config.h"

//Cette instruction évite d'écrire sous forme arduino::fonction et permet d'appeler fonction directement.
//using namespace arduino;
byte ABCreq[] = { 0xFE, 0X03, 0X00, 0X1F, 0X00, 0X01 };   // 1f in Hex -> 31 dezimal

void send_Request(byte* Request, int Re_len)
{
    while (!Serial.available())
    {
        Serial.write(Request, Re_len);   // Send request to S8-Sensor
        delay(50);
    }
}

void setup() {
    Serial.begin(9600);

    send_Request(ABCreq, 6);
}

void loop() {

}
/**
 * @brief Fonction nécessaire pour l'initialisation de l'arduino
 * Dans notre cas cette fonction initialisera paramettre le 'capteur'
 */
void setupSave() {
    Serial1.begin(9600);
    //arduino::enableSerial(9600);
    //arduino::debug << 
    arduino::enableSerial(arduino::debug, 9600);
}

    String InputString;
    int stringComplete = 0;
/**
 * @brief Fonction nécessaire qui est appelé en boucle par le programme après le passage dans la fonction setupe
 * Un peut comme le jeu du ping pong, le 'capteur' sera en écoute, et attend ces instruction.
 */
void loopSave() {
    //Si des donnée ont été reçus alors ces condition vaut true, dans le cas contraire nous restons toujours en attente d'instruction.
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
        //arduino::debug << "nombre de byt : " << Serial1.available() << arduino::endl;
    }
    
    if(stringComplete) {
        arduino::debug << "Une demande à été reçu : " << InputString << arduino::endl;
        stringComplete = 0;
        InputString = "";
        Serial1.write("Pong!");
        arduino::debug << "Réponse envoyé : Pong!" << arduino::endl;
    }
    //Test dialogue avec la carte m5stack Atom
}