/**
   @file CarteM5StackAtom.ino
   @author Julie Brindejont (julie.brindejont@gmail.com)
   @contributor : Pierre-André Souville (affichage PPM), Tony Vanpoucke (animations).
   @brief Projet CO²
   @version 1.1.0
   @date 2022-02-24

   Projet sympathique menant sur la détection de CO²

   Pour cela, nous avons une carte M5Stack atom conncectée en Serial avec le capteur de CO² (Senseair S8).
   Le capteur de CO² communique avec le protocol Modbus.

   Après avoir reçu les infos du capteur, les led s'allumeront d'une certaine couleur.
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
#error "Ce code est supporté uniquement sur les cartes m5stack atom."
#endif

#include "SenseairS8.h"
#include "MatrixDisplayNumbers.h"
#include <M5Atom.h>

// ==============================================================================================
// ==                                                                                          ==
// ==                                      VARIABLES                                           ==
// ==                                                                                          ==
// ==============================================================================================

//Définition des broches nécessaires au bon fonctionnement du code
#define PIN_SERIAL_RX  21 //Allant sur le port UART TxD du capteur
#define PIN_SERIAL_TX  25 //Allant sur le port UART RxD du capteur

//Définition des seuils de qualité de l'air
#define LIMITE_BASSE 300                      // Si en dessous passe le capteur en erreur (air extérieur 400 PPM en moyenne)
#define AIR_MOYEN 800                         // Capteur en vert avant cette limite de PPM
#define AIR_MEDIOCRE 1000                     // Capteur en orange avant cette limite de PPM
#define AIR_VICIE 1500                        // Capteur en rouge avant cette limite de PPM (au delà il clignotte)

//converti directement les secondes en millisecondes.
#define ATTENTE_CHECK_CO2 1 * 1000    //nb secondes * (milisecondes dans une secondes)

//Temps (milisecondes) nécessaire à l'utilisateur pour demander une calibration du capteur
#define TEMPS_BTN_APPUYER 3 * 1000

//Temps (milisecondes) pour faire clignoter si nécessaire les leds.
#define TEMPS_CLIGNOTEMENT_LED 1 * 1000

//Tempo des animations du capteur (milisecondes, 15 par défaut)).
#define TEMPO_ANIMATION 15

//Fourchette de tolérance graphique (plus cette valeur est petite, plus le changement de couleur "collera" au niveau seuil)
#define ANIMATION_OFFSET 150

//Couleur du capteur lors d'un calibrage (0 = rouge / 100 = vert / 150 = bleu).
#define COULEUR_CALIBRATION 150

//Temps de démarrage du capteur en "tour d'animations" (1 = 4secondes / 5 = 20 secondes).
#define TEMPS_DEMARRAGE 5

// ==============================================================================================
// ==                                                                                          ==
// ==                                      CONSTANTES                                          ==
// ==                                                                                          ==
// ==============================================================================================

//  @brief dernier temps connu du contrôle de co²
unsigned long lastTimeCheckCO2 = 0;

//  @brief c'est ici qu'est stockée la couleur du capteur
int colorVal;

//  @brief permet le clignotement des animations
bool AnimSequence;

//  @brief Variable pour savoir si une calibration est demandée
bool calibrationRequested;

//  @brief Variable pour savoir si un affichage du PPM est demandée
bool showMePPM;

// ==============================================================================================
// ==                                                                                          ==
// ==                               PROTOTYPE DES FONCTIONS                                    ==
// ==                                                                                          ==
// ==============================================================================================

//------------------------------- Prototype des fonctions appellée par le programme. -------------------------------
/**
   @brief Fonction nécessaire pour l'initialisation de l'arduino
   Dans notre cas cette fonction initialisera le capteur CO²
*/
void setup();

/**
   @brief Fonction nécessaire qui est appelée en boucle par le programme après le passage dans la fonction setup
   Un peu comme le jeu du ping pong, le 'capteur' sera en écoute, et attend ses instructions.
*/
void loop();


//------------------------------- Prototype concernant le capteur -------------------------------
/**
   @brief Fonction émettant une requête basée sur le protocol Modbus,
   puis se met en attente d'une réponse provenant du capteur CO²
   Cette fonction est un condensé des fonctions 'send_Request', 'read_Response', et 'get_value'
   Elle renvoie le taux de CO²
   @return taux de CO² ou 99 si erreur.
*/

void sendRequestCalibration();

/**
   @brief Contrôle le niveau de CO²
*/
void checkCO2(unsigned long co2);

/**
   @brief Fonction d'une boucle infinie en cas d'erreur de branchement.
*/
void displayError();


// ==============================================================================================
// ==                                                                                          ==
// ==                                       FONCTIONS                                          ==
// ==                                                                                          ==
// ==============================================================================================

// ======================== CONFIGURATION CAPTEUR : VOID SETUP ======================== //

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

// ======================== FONCTIONNEMENT CAPTEUR : VOID LOOP ======================== //

void loop() {

  //Récupération du taux de CO² toutes les x secondes défini sur la constante 10000
  //milliseconde actuelle - dernier temps connu >= 10000
  if (millis() - lastTimeCheckCO2 >= ATTENTE_CHECK_CO2 && !calibrationRequested) {
    //Récupération du taux de CO²
    unsigned long co2 = senseairS8.reqCO2();
    Serial.println();
    Serial.println("CO² = " + String(co2));

    //Controle du co2
    checkCO2(co2);
    lastTimeCheckCO2 = millis();

  } else if (calibrationRequested) {
    //Demande de calibration
    sendRequestCalibration();
    calibrationRequested = 0;
  } else if (showMePPM) {
    //Animation de "fade-out" lors de l'affichage du CO2
    for (int i = 10; i > 0; i--) {
      for (int j = 0; j < 25; j++) {
        M5.dis.drawpix( j, CHSV( colorVal, 255, i * 25));
      }
      delay(TEMPO_ANIMATION);
    }
    //Afficher le taux de CO2 sur la matrice de LED
    unsigned long co2 = senseairS8.reqCO2();
    displayMatrix(String(co2), colorVal);

    //Animation de "fade-in" lors de l'affichage du CO2
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 25; j++) {
        M5.dis.drawpix( j, CHSV( colorVal, 255, i * 12));
      }
      delay(TEMPO_ANIMATION);
    }
    showMePPM = 0;
  }

  //Cette instruction met à jour l'état du bouton.
  M5.update();

  if (M5.Btn.isPressed() && !calibrationRequested && !showMePPM) {
    Serial.println();
    Serial.println("Affichage des PPM demandée");
    showMePPM = 1;
  }
  //Si le bouton est appuyé pendant plus de x secondes, alors nous demandons une calibration du capteur.
  else if (M5.Btn.pressedFor(TEMPS_BTN_APPUYER) && !calibrationRequested) {
    Serial.println();
    Serial.println("Calibration du capteur demandée");
    calibrationRequested = 1;
    showMePPM = 0;
  }
}

// ======================== CALIBRATION DU CAPTEUR CO2 ======================== //

void sendRequestCalibration() {

  byte stateLed = LOW;

  while (senseairS8.progressCalibration()) {
    //Vérification si des erreurs ont été détectées pendant le calibrage.
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
  Serial.println("Calibration terminée sans erreur.");
}

// ======================== INTERPRETATION DU CAPTEUR CO2 ======================== //

void checkCO2(unsigned long co2) {

  Serial.print("Qualité de l'air : ");

  // ============ CAPTEUR : EN ERREUR // code couleur : "neige" noir et blanc ==== //

  if (co2 < LIMITE_BASSE) {
    //Mettre les leds dans un état de clignotement à l'infini si une erreur est survenue.
    Serial.println("[Erreur de calibrage capteur]");
    displayError();

    // ============ CAPTEUR : AIR EXCELLENT // code couleur : vert ==== //

  } else if (co2 >= LIMITE_BASSE && co2 < (AIR_MOYEN - ANIMATION_OFFSET) ) {
    //Permet d'afficher toutes les leds du m5stack atom en vert
    for (int i = 0; i < 25; i++) {
      colorVal = 100;                                          //selectionnez la couleur verte
      M5.dis.drawpix( i, CHSV( colorVal, 255, 255));
    }
    Serial.println("Excellent.");

    // ============ CAPTEUR : AIR MOYEN // code couleur : orange ==== //

  } else if (co2 >=  (AIR_MOYEN - ANIMATION_OFFSET) && co2 < (AIR_MEDIOCRE + ANIMATION_OFFSET) ) {
    //selectionnez une teinte de orange en fonction de l'état du CO2
    colorVal = map(co2, (AIR_MOYEN - ANIMATION_OFFSET), (AIR_MEDIOCRE + ANIMATION_OFFSET), 100, 0);
    //Permet d'afficher toutes les leds du m5stack atom en orange
    for (int i = 0; i < 25; i++) {
      M5.dis.drawpix( i, CHSV( colorVal, 255, 255));
    }
    Serial.println("Moyen.");

    // ============ CAPTEUR : AIR MAUVAIS // code couleur : rouge ==== //

  } else if (co2 >= (AIR_MEDIOCRE + ANIMATION_OFFSET) && co2 < AIR_VICIE) {
    //Permet d'afficher toutes les leds du m5stack atom en rouge
    for (int i = 0; i < 25; i++) {
      colorVal = 0;                                         //selectionnez la couleur rouge
      M5.dis.drawpix( i, CHSV( 0, 255, 255));
    }
    Serial.println("Médiocre.");

    // ============ CAPTEUR : AIR TRES MAUVAIS // code couleur : rouge clignotant ==== //

  } else {
    colorVal = 0;                                         //selectionnez la couleur rouge
    //Permet d'afficher une animation de led rouge clignotante (seuil haut dépassé).
    if (AnimSequence == 1) {
      //Animation de "fade-out" lors du clignottement
      for (int i = 20; i > 0; i--) {
        for (int j = 0; j < 25; j++) {
          M5.dis.drawpix( j, CHSV( colorVal, 255, i * 12));
        }
        delay(TEMPO_ANIMATION);
      }
      AnimSequence = 0;
      for (int i = 0; i < 25; i++) {
        M5.dis.drawpix( i, CHSV( colorVal, 0, 0));
      }
    }
    else if (AnimSequence == 0) {
      //Animation de "fade-in" lors du clignottement
      for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 25; j++) {
          M5.dis.drawpix( j, CHSV( colorVal, 255, i * 12));
        }
        delay(TEMPO_ANIMATION);
      }
      AnimSequence = 1;
      for (int i = 0; i < 25; i++) {
        M5.dis.drawpix( i, CHSV( colorVal, 255, 255));
      }
    }
    Serial.println("Vicié.");
  }
}

// ======================== ERREUR DU CAPTEUR CO2 ======================== //

void displayError() {
  while (1) {
    //Permet d'afficher une animation infinie d'erreur (nécéssitera de redébrancher le capteur).
    int PixNum = random(0, 25);
    int NoisePix = random(20, 200);
    M5.dis.drawpix( PixNum, CHSV( 0, 0, NoisePix));
    delay(TEMPO_ANIMATION);
  }
}
