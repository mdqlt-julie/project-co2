/**
 * @file CarteM5StackAtom.ino
 * @author Julie Brindejont (julie.brindejont@gmail.com)
 * @brief Projet CO²
 * @version 1.0.0
 * @date 2022-01-14
 * 
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
 * Problématique n°1, je ne peut pas créer une interruption directement en gérant un appui de 3sec.
 * @copyright Copyright (c) 2021
 */

#ifndef ARDUINO_M5Stack_ATOM
    #error "Ce code est supporter uniquement sur les cartes m5stack atom."
#endif

#include "SenseairS8.h"
#include "MatrixDiplayNumbers.h"
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

//converti directement les secondes en milisecondes.
#define ATTENTE_CHECK_CO2 10 * 1000 //nb secondes * (milisecondes dans une secondes)

//Temps (milisecondes) nécessaire à l'utilisateur pour demander une calibration du capteur
#define TEMPS_BTN_APPUYER 3 * 1000

//Temps (milisecondes) pour clignoter si nécessaire les leds.
#define TEMPS_CLIGNOTEMENT_LED 1 * 1000

//Création d'une énumération pour définire la couleur des led's.
typedef enum {
    White = 0xffffff, 
    Green = 0x00ff00, 
    Red = 0xff0000, 
    Orange = 0xff7f00,
    Black = 0x000000
    
} ColorLeds;
int colorList[] = {ColorLeds::Black, ColorLeds::Green};

// ==============================================================================================
// ==                                                                                          ==
// ==                                      VARIABLES                                           ==
// ==                                                                                          ==
// ==============================================================================================

/**
 * @brief dernier temps connue du controle de co²
 * 
 */
unsigned long lastTimeCheckCO2 = 0;

bool calibrationRequested = 0;

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

void sendRequestCalibration();

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
// ==                                       FONCTIONS                                          ==
// ==                                                                                          ==
// ==============================================================================================

void sendRequestCalibration() {
        //Serial.println("Demande calibration");
    byte stateLed = LOW;
    unsigned long lastBlinkLedWarning = 0;

    while(senseairS8.progressCalibration()){
        //Vérification si des erreur ont été détecter pendant le calibrage.
        if(senseairS8.checkError()) {
            displayError();
        } else if(millis() - lastBlinkLedWarning >= TEMPS_CLIGNOTEMENT_LED ) {
            if(stateLed == HIGH) {
                M5.dis.clear();
                stateLed = LOW;
            } else {
                M5.dis.fillpix(ColorLeds::Orange);
                stateLed = HIGH;
            }
            lastBlinkLedWarning = millis();
        }
    }
    Serial.println("Calibration terminer sans erreur.");
}

void checkCO2(unsigned long co2) {

    Serial.print("Qualiter de l'air : Air ");
    if (co2 == 0) {
        M5.dis.fillpix(ColorLeds::White);
        Serial.println("En attente.");
    } else if (co2 == 99) {
        //Mettre les led dans un état de clignotement à l'infinie si une erreur est survenue.
        displayError();
    } else if (co2 < AIR_MOYEN) {
        Serial.println("Excellent.");

        // Affiche le taux obtenu sur l'écran de leds
        colorList[1] = ColorLeds::Green;
        //displayCO2(String(co2),colorList);
        displayCO2(String(co2),colorList);

        //Permet d'afficher toute les led du m5stack atom en vert
        M5.dis.fillpix(ColorLeds::Green);
    } else if (co2 >= AIR_MOYEN && co2 < AIR_MEDIOCRE) {
        Serial.println("Moyen.");

        // Affiche le taux obtenu sur l'écran de leds
        colorList[1] = ColorLeds::Orange;
        displayCO2(String(co2),colorList);

        //Permet d'afficher toute les led du m5stack atom en orange
        M5.dis.fillpix(ColorLeds::Orange);
    }else if (co2 >= AIR_MEDIOCRE && co2 < AIR_VICIE) {
        Serial.println("Mediocre.");
        
        // Affiche le taux obtenu sur l'écran de leds
        colorList[1] = ColorLeds::Red;
        displayCO2(String(co2),colorList);

        //Permet d'afficher toute les led du m5stack atom en rouge
        M5.dis.fillpix(ColorLeds::Red);
    } else {
        Serial.println("Vicie.");

        // Affiche le taux obtenu sur l'écran de leds
        colorList[1] = ColorLeds::Red;
        displayCO2(String(co2),colorList);
        
        //Permet d'afficher toute les led du m5stack atom en rouge
        M5.dis.fillpix(ColorLeds::Red);
    }
}
void displayError() {
    while(1) {
        M5.dis.fillpix(ColorLeds::Red);
        delay(TEMPS_CLIGNOTEMENT_LED);
        M5.dis.clear();
        delay(TEMPS_CLIGNOTEMENT_LED);
    }
}

void setup() {
    // ports série et de communication capteur
    M5.begin(true,false,true);
    M5.IMU.Init();  //Initialisation sensor IMU
    //M5.Btn
    Serial1.begin(9600, SERIAL_8N1, PIN_SERIAL_RX, PIN_SERIAL_TX);
    checkCO2(0); //Mise de la led en attente.
    //attachInterrupt(39, eventInterruptPushButton, FALLING);
    for (int i = 10; i < 20; i++) {
        M5.dis.clear();
        displayCO2(String(i),colorList);
        delay(1000 * 5);
    }
}



void loop() {
    
    //Récupération du taux de CO² toutes les 10 secondes définis sur la constante 10000
    //millis actuelle - dernier temps connus >= 10000
  /*  if(millis() - lastTimeCheckCO2 >= ATTENTE_CHECK_CO2 && !calibrationRequested) {
        //Récupération du taux de CO²
        unsigned long co2 = senseairS8.reqCO2();
        Serial.println("CO² = " + String(co2));

        //Controle du co2
        checkCO2(co2);
        lastTimeCheckCO2 = millis();

    } else if(calibrationRequested) {
        //Demande de calibration
        sendRequestCalibration();
        calibrationRequested = 0;
        //M5.update();//Forcer la mise à jour de façon précose affin d'empêcher une 2
    }

    //Cette instruction met à jour l'état du boutton.
    M5.update();

    //Si le boutton est appuyer pendant plus de x secondes alors, nous demandons une calibration du capteur.
    if(M5.Btn.pressedFor(TEMPS_BTN_APPUYER) && !calibrationRequested) {
        Serial.println("Calibration du capteur demander");
        calibrationRequested = 1;
    }*/
   /* for (int i = 0; i < 255; i++) {
        M5.dis.fillpix(CHSV(100, 255, i));
    }
    for (int i = 255; i > 0; i--) {
        M5.dis.fillpix(CHSV(100, 255, i));
    }
    M5.update();*/
}