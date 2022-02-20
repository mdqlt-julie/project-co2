/**
   @file CarteM5StackAtom.ino
   @author Julie Brindejont (julie.brindejont@gmail.com)
   @contributor : Pierre-André Souville, Tony Vanpoucke.
   @brief Projet CO²
   @version 1.0.5
   @date 2022-02-12

   Projet sympatique menant sur la détection de CO²

   Pour cela, nous avons une carte M5Stack atom conncecté en Serial avec le capteur de CO² (Senseair S8).
   Le capteur de CO² communique avec le protocol Modbus.

   Après avoir reçus les infos du capteur, les led s'allumeront d'une certainne couleur.
   Vert si CO² est bon
   Orange si CO² est moyen
   Rouge si CO² est mauvais 
   Rouge clignotant si CO² est très mauvais.

   Blanc au démarrage du capteur
   Bleu au calibrage du capteur
   "Neige" noir et blanc si le capteur est en défaut (branchement ou calibrage)
   
   @CC-BY-SA - 2022
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
#define PIN_SERIAL_RX  21 //Allant sur le port UART TxD du capteur
#define PIN_SERIAL_TX  25 //Allant sur le port UART RxD du capteur

//Définition des seuil qualité de l'air
#define LIMITE_BASSE 300
#define AIR_MOYEN 800
#define AIR_MEDIOCRE 1000
#define AIR_VICIE 1500

//converti directement les secondes en milisecondes.
#define ATTENTE_CHECK_CO2 1 * 1000 //nb secondes * (milisecondes dans une secondes)

//Temps (milisecondes) nécessaire à l'utilisateur pour demander une calibration du capteur
#define TEMPS_BTN_APPUYER 3 * 1000

//Temps (milisecondes) pour clignoter si nécessaire les leds.
#define TEMPS_CLIGNOTEMENT_LED 1 * 1000

//Tempo des animations du capateur (milisecondes, 15 par défaut)).
#define TEMPO_ANIMATION 15

//Fourchette de tolérance graphique (plus cette valeur est petite plus le changement de couleur "collera" au niveau seuil)
#define ANIMATION_OFFSET 150

//Couleur du capteur lors d'un calibrage (0 = rouge / 100 = vert / 150 = bleu).
#define COULEUR_CALIBRATION 150

//Couleur du capteur lors d'un calibrage (0 = rouge / 100 = vert / 150 = bleu).
#define TEMPS_DEMARRAGE 5

// ==============================================================================================
// ==                                                                                          ==
// ==                                      VARIABLES                                           ==
// ==                                                                                          ==
// ==============================================================================================

/**
   @brief dernier temps connue du controle de co²

*/
unsigned long lastTimeCheckCO2 = 0;
unsigned long lastBlinkLedWarning = 0;

bool AnimSequence;
bool calibrationRequested;

// ==============================================================================================
// ==                                                                                          ==
// ==                               PROTOTYPE DES FONCTIONS                                    ==
// ==                                                                                          ==
// ==============================================================================================

//------------------------------- Prototype concernant le capteur -------------------------------
/**
   @brief Fonction émettant une requete basé sur le protocol Modbus,
   puis ce met en attente d'une réponse provenant du capteur CO²
   Cette fonction est un condancé des fonction 'send_Request', 'read_Response', et 'get_value'
   Elle renvoie le taux de CO²

   @return  taux de CO² ou 99 si erreur.
*/
// unsigned long getCO2();

void sendRequestCalibration();

/**
   @brief Controle le niveau de CO²

*/
void checkCO2(unsigned long co2);

//Fonction d'une boucle infinie faisant clignoter la led en cas d'erreur de branchement.
void displayError();

//------------------------------- Prototype des fonctions appellée par le programme. -------------------------------
/**
   @brief Fonction nécessaire pour l'initialisation de l'arduino
   Dans notre cas cette fonction initialisera le capteur CO²
*/
void setup();

/**
   @brief Fonction nécessaire qui est appelé en boucle par le programme après le passage dans la fonction setupe
   Un peut comme le jeu du ping pong, le 'capteur' sera en écoute, et attend ces instruction.
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

  while (senseairS8.progressCalibration()) {
    //Vérification si des erreur ont été détecter pendant le calibrage.
    if (senseairS8.checkError()) {
      displayError();
    } else {
      //Animation du capteur durant le calibrage.
      for (int i = 100; i < 200; i++) {
        for (int j = 0; j < 25; j++) {
          M5.dis.drawpix( j, CHSV( COULEUR_CALIBRATION, 255, i));
        }
        delay(TEMPO_ANIMATION);
      }
      for (int i = 200; i > 100; i--) {
        for (int j = 0; j < 25; j++) {
          M5.dis.drawpix( j, CHSV( COULEUR_CALIBRATION, 255, i));
        }
        delay(TEMPO_ANIMATION);
      }
    }
  }
  Serial.println("Calibration terminée sans erreurs.");
}

void checkCO2(unsigned long co2) {

  Serial.print("Qualité de l'air : ");

  if (co2 < LIMITE_BASSE) {
    //Mettre les led dans un état de clignotement à l'infinie si une erreur est survenue.
    Serial.println("[Erreur de calibrage capteur]");
    displayError();

  } else if (co2 >= LIMITE_BASSE && co2 < (AIR_MOYEN - ANIMATION_OFFSET) ) {
    //Permet d'afficher toute les led du m5stack atom en vert
    for (int i = 0; i < 25; i++) {
      M5.dis.drawpix( i, CHSV( 100, 255, 255));
    }
    Serial.println("Excellent.");

  } else if (co2 >=  (AIR_MOYEN - ANIMATION_OFFSET) && co2 < (AIR_MEDIOCRE + ANIMATION_OFFSET) ) {
    //Permet d'afficher toute les led du m5stack atom en orange

    int Co2Scale = map(co2, (AIR_MOYEN - ANIMATION_OFFSET), (AIR_MEDIOCRE + ANIMATION_OFFSET), 100, 0);

    for (int i = 0; i < 25; i++) {
      M5.dis.drawpix( i, CHSV( Co2Scale, 255, 255));
    }
    Serial.println("Moyen.");

  } else if (co2 >= (AIR_MEDIOCRE + ANIMATION_OFFSET) && co2 < AIR_VICIE) {
    //Permet d'afficher toute les led du m5stack atom en rouge
    for (int i = 0; i < 25; i++) {
      M5.dis.drawpix( i, CHSV( 0, 255, 255));
    }
    Serial.println("Mediocre.");

  } else {
    Serial.println("Vicié.");
    //Permet d'afficher une animation de led rouge cligonttante (seuil haut dépassé).
    if (AnimSequence == 1) {
      AnimSequence = 0;
      for (int i = 0; i < 25; i++) {
        M5.dis.drawpix( i, CHSV( 0, 0, 0));
      }
    }
    else if (AnimSequence == 0) {
      AnimSequence = 1;
      for (int i = 0; i < 25; i++) {
        M5.dis.drawpix( i, CHSV( 0, 255, 255));
      }
    }
  }
}

void displayError() {
  while (1) {
    //Permet d'afficher une animation infinie d'erreur (nécéssitera de redébrancher le capteur).
    int PixNum = random(0, 25);
    int NoisePix = random(20, 200);
    M5.dis.drawpix( PixNum, CHSV( 0, 0, NoisePix));
    delay(TEMPO_ANIMATION);
  }
}

void setup() {
  // ports série et de communication capteur
  M5.begin(true, false, true);
  //M5.Btn
  Serial1.begin(9600, SERIAL_8N1, PIN_SERIAL_RX, PIN_SERIAL_TX);
  Serial.println("En attente.");
  //Animation du capteur à son démarrage.
  for (int x = 0; x < TEMPS_DEMARRAGE; x++) {
    for (int i = 120; i < 255; i++) {
      for (int j = 0; j < 25; j++) {
        M5.dis.drawpix( j, CHSV( 0, 0, i));
      }
      delay(TEMPO_ANIMATION);
    }
    for (int i = 255; i > 120; i--) {
      for (int j = 0; j < 25; j++) {
        M5.dis.drawpix( j, CHSV( 0, 0, i));
      }
      delay(TEMPO_ANIMATION);
    }
  }
}


void loop() {

  //Récupération du taux de CO² toutes les x secondes définis sur la constante 10000
  //millis actuelle - dernier temps connus >= 10000
  if (millis() - lastTimeCheckCO2 >= ATTENTE_CHECK_CO2 && !calibrationRequested) {
    //Récupération du taux de CO²
    unsigned long co2 = senseairS8.reqCO2();
    Serial.println("CO² = " + String(co2));

    //Controle du co2
    checkCO2(co2);
    lastTimeCheckCO2 = millis();

  } else if (calibrationRequested) {
    //Demande de calibration
    sendRequestCalibration();
    calibrationRequested = 0;
  }

  //Cette instruction met à jour l'état du boutton.
  M5.update();

  //Si le boutton est appuyer pendant plus de x secondes alors, nous demandons une calibration du capteur.
  if (M5.Btn.pressedFor(TEMPS_BTN_APPUYER) && !calibrationRequested) {
    Serial.println("Calibration du capteur demandée");
    calibrationRequested = 1;
  }
}
