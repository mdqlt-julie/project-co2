#include <TFT_eSPI.h>
#include "Modbus.h"

// ==============================================================================================
// ==                                                                                          ==
// ==                                      CONSTANTES                                          ==
// ==                                                                                          ==
// ==============================================================================================

//Définition des broches nécessaire au bon fonctionnement du code
#define RXD2  13 //Allant sur le port UART TxD du capteur (correspond au fil bleu)
#define TXD2  12 //Allant sur le port UART RxD du capteur (correspond au fil blanc)

//Définition des seuil qualité de l'air
#define AIR_MOYEN 800
#define AIR_MEDIOCRE 1000
#define AIR_VICIE 1500

// ==============================================================================================
// ==                                                                                          ==
// ==                               PROTOTYPE DES FONCTIONS                                    ==
// ==                                                                                          ==
// ==============================================================================================

void prepareEcran();

//------------------------------- Prototype concernant le capteur -------------------------------
/**
 * @brief Fonction émettant une requete basé sur le protocol Modbus,
 * puis ce met en attente d'une réponse provenant du capteur CO²
 * Cette fonction est un condancé des fonction 'send_Request', 'read_Response', et 'get_value'
 * d'où la necessité d'une structure.
 * 
 * @param RequestModbus Requete Modbus sous forme de tableau
 * @return unsigned long Renvoie la valeur retourné par le capteur.
 */
unsigned long getCo2();

/**
 * @brief Controle le niveau de CO²
 * 
 */
void checkCO2(unsigned long co2);

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
//Requete modbus pour consulter le niveau de CO².
byte ReqCO2[] = { 0xFE, 0X04, 0X00, 0X03, 0X00, 0X01, 0XD5, 0XC5 }; 

TFT_eSPI tft = TFT_eSPI(135, 240);
unsigned long ancienCO2 = 0;
int seuil = 0;

// ==============================================================================================
// ==                                                                                          ==
// ==                                       FONCTIONS                                          ==
// ==                                                                                          ==
// ==============================================================================================


// nettoie l'écran et affiche les infos utiles
void prepareEcran() {
    tft.fillScreen(TFT_BLACK);
    // texte co2 à  gauche
    tft.setTextSize(4);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("CO", 20, 110);
    tft.setTextSize(3);
    tft.drawString("2", 55, 115);

    // texte PPM à  droite ppm
    tft.drawString("ppm", 190, 105);

    // écriture du chiffre
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(8);
}


unsigned long  getCo2() {
    //Envoie de la requete.
    _send_Request(ReqCO2, 8);
    //Réception de la réponse.
    _read_Response(7);
   return _get_Value(7);
}

void printStringTft(int idSeul, uint16_t color, const char* string) {
  tft.setTextColor(color, TFT_BLACK);
  if (seuil != idSeul) {
      tft.setTextSize(2);
      tft.fillRect(0, 61, tft.width(), 25, TFT_BLACK);
      tft.drawString(string, tft.width() / 2, tft.height() / 2 + 10);
  }
  seuil = idSeul;
  Serial.println(string);
}

void checkCO2(unsigned long co2) {

  Serial.print("Qualiter de l'air : ");
  if (co2 < AIR_MOYEN) {
    printStringTft(1, TFT_GREEN, "Air Excellent");
  } else if (co2 >= AIR_MOYEN && co2 < AIR_MEDIOCRE) {
    printStringTft(2, TFT_ORANGE, "Air Moyen");
  }else if (co2 >= AIR_MEDIOCRE && co2 < AIR_VICIE) {
    printStringTft(3, TFT_RED, "Air Mediocre");
  } else {
    printStringTft(4, TFT_RED, "Air Vicie");
  }
}
void setup() {
    Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);

    // initialise l'écran
    tft.init();
    delay(20);
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM); // imprime la string middle centre

    // préparation de l'écran
    prepareEcran();
    seuil = 0;
    Serial.println("Initialisation terminer");

}

void loop() {
  // put your main code here, to run repeatedly:
    //Récupération du taux de CO²
    unsigned long co2 = getCo2();
    Serial.println("CO² = " + String(co2));

    // efface le chiffre du texte
    if (co2 != ancienCO2) {
        tft.fillRect(0, 0, tft.width(), 60, TFT_BLACK);
    }
    //Controle du co2
    checkCO2(co2);

    tft.setTextSize(8);
    tft.drawString(String(co2), tft.width() / 2, tft.height() / 2 - 30);

    ancienCO2 = co2;
    delay(10000);
}
