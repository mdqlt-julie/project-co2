/**
 * @file CarteM5StackAtom.ino
 * @author Julie Brindejont (julie.brindejont@gmail.com)
 * @brief Projet CO²
 * @version 0.1
 * @date 2021-12-18
 * 
 * Projet sympatique menant sur la détection de CO²
 * 
 * Pour cela, nous avons une carte M5Stack atom conncecté en Serial avec le capteur de CO² (Senseair S8).
 * Le capteur de CO² communique avec le protocol Modbus.
 * 
 * Après avoir reçus les infos du capteur, les led s'allumeront d'une certainne couleur.
 * Blanc su CO² == 0 (en attente de la 1ère lecture.)
 * Vert si CO² < 800ppm
 * Orange si entre 800ppm et 1000
 * Rouge si supérieur à 1000
 * Rouge clignotant si Erreur détecter.
 * 
 * @copyright Copyright (c) 2021
 */

#ifndef ARDUINO_M5Stack_ATOM
    #error "Ce code est supporter uniquement sur les cartes m5stack atom."
#endif

#include "SenseairS8.h"
#include <M5Atom.h>

// ==============================================================================================
// ==                                                                                          ==
// ==                                      CONSTANTES                                          ==
// ==                                                                                          ==
// ==============================================================================================

//Définition des broches nécessaire au bon fonctionnement du code
#define PIN_SERIAL_RX  25 //Allant sur le port UART TxD du capteur
#define PIN_SERIAL_TX  21 //Allant sur le port UART RxD du capteur

//Définition des seuil qualité de l'air
#define AIR_MOYEN 800
#define AIR_MEDIOCRE 1000
#define AIR_VICIE 1500

//Création d'une énumération pour définire la couleur des led's.
typedef enum {
    White = 0xffffff, 
    Green = 0x00ff00, 
    Red = 0xff0000, 
    Orange = 0xff7f00
} ColorLeds;

// ==============================================================================================
// ==                                                                                          ==
// ==                               PROTOTYPE DES FONCTIONS                                    ==
// ==                                                                                          ==
// ==============================================================================================

//------------------------------- Prototype concernant le capteur -------------------------------
/**
 * @brief Fonction émettant une requete basé sur le protocol Modbus,
 * puis ce met en attente d'une réponse provenant du capteur CO²
 * Cette fonction est un condancé des fonction 'send_Request', 'read_Response', et 'get_value'
 * Elle renvoie le taux de CO²
 * 
 * @return  taux de CO² ou 99 si erreur.
 */
// unsigned long getCO2();


/**
 * @brief Controle le niveau de CO²
 * 
 */
void checkCO2(unsigned long co2);

//Fonction d'une boucle infinie faisant clignoter la led en cas d'erreur de branchement.
void displayError();

//------------------------------- Prototype des fonctions appellée par le programme. -------------------------------
/**
 * @brief Fonction nécessaire pour l'initialisation de l'arduino
 * Dans notre cas cette fonction initialisera le capteur CO²
 */
void setup();

/**
 * @brief Fonction nécessaire qui est appelé en boucle par le programme après le passage dans la fonction setupe
 * Un peut comme le jeu du ping pong, le 'capteur' sera en écoute, et attend ces instruction.
 */
void loop();

// ==============================================================================================
// ==                                                                                          ==
// ==                                      VARIABLES                                           ==
// ==                                                                                          ==
// ==============================================================================================

//byte ReqCo2[] = { 0xFE, 0X04, 0X00, 0X03, 0X00, 0X01, 0XD5, 0XC5 }; //Requete pour consulter le niveau de CO².


// ==============================================================================================
// ==                                                                                          ==
// ==                                       FONCTIONS                                          ==
// ==                                                                                          ==
// ==============================================================================================

/*unsigned long  getCO2() {
    //Envoie de la requete.
    _send_Request(ReqCo2, 8);
    //Réception de la réponse.
    _read_Response(7);
   return _get_Value(7);
}*/

void checkCO2(unsigned long co2) {

    Serial.print("Qualiter de l'air : Air ");
    if (co2 == 0) {
        M5.dis.fillpix(ColorLeds::White);
        Serial.println("En attente.");
    } else if (co2 == 99) {
        //Mettre les led dans un état de clignotement à l'infinie si une erreur est survenue.
        displayError();
    } else if (co2 < AIR_MOYEN) {
        //Permet d'afficher toute les led du m5stack atom en vert
        M5.dis.fillpix(ColorLeds::Green);
        Serial.println("Excellent.");
    } else if (co2 >= AIR_MOYEN && co2 < AIR_MEDIOCRE) {
        //Permet d'afficher toute les led du m5stack atom en orange
        M5.dis.fillpix(ColorLeds::Orange);
        Serial.println("Moyen.");
    }else if (co2 >= AIR_MEDIOCRE && co2 < AIR_VICIE) {
        //Permet d'afficher toute les led du m5stack atom en rouge
        M5.dis.fillpix(ColorLeds::Red);
        Serial.println("Mediocre.");
    } else {
        M5.dis.fillpix(ColorLeds::Red);
        Serial.println("Vicie.");
    }
}
void displayError() {
    while(1) {
        M5.dis.fillpix(ColorLeds::Red);
        delay(500);
        M5.dis.clear();
        delay(500);
    }
}

void setup() {
    // ports série et de communication capteur
    M5.begin(true,false,true);
    Serial1.begin(9600, SERIAL_8N1, PIN_SERIAL_RX, PIN_SERIAL_TX);

}


void loop() {
    //Récupération du taux de CO²
    unsigned long co2 = senseairS8.getCO2();
    Serial.println("CO² = " + String(co2));

    //Controle du co2
    checkCO2(co2);

    //Attendre 10 secondes avant la prochaine mesure.
    delay(10000);
}