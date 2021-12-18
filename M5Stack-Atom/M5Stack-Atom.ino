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
 * Vert si CO² < 800ppm
 * Orange si entre 800ppm et 1000
 * Rouge si supérieur à 1000
 * 
 * @copyright Copyright (c) 2021
 */
#include "Modbus.h"
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

//Définition des requetes Modbus pour le capteur CO²
#define SIZE_TBL_REQUESTS 2
#define SIZE_REQUEST 8
#define REQ_ABC { 0xFE, 0X03, 0X00, 0X1F, 0X00, 0X01, 0XA1, 0XC3 } //Requete pour vérifier l'autocalibrage.
#define REQ_CO2 { 0xFE, 0X04, 0X00, 0X03, 0X00, 0X01, 0XD5, 0XC5 } //Requete pour consulter le niveau de CO².
#define ABC 0 //indice 0 du tableau de requete
#define CO2 1 //indice 1 du tableau de requete


//J'ai créer une structure qui facilite la création d'une fonction
//faisant un condenssé de 3 fonction en unse seule.
typedef struct {
    byte* Request;
    //int sizeRequest; Étant donnée que chaque requête ont la même taille cette variable est inutile.
    int sizeResponse; //Taille de la réponse en générale, celle-ci fait 7 suivant la doc, mais certaine peuvent être à 8.
} RequeteModbus;

//Création d'une énumération pour définire la couleur des led's.
typedef enum {
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
 * @brief Initialise toutes les requete Modbus.
 * 
 */
void initializeRequests();

/**
 * @brief Fonction permettant d'insérrer une requête dans notre tableau de structure de requêtes.
 * 
 * @param Request 
 * @param sizeResponse 
 */
RequeteModbus initializeRequest(byte* Request, int sizeResponse);
/**
 * @brief Fonction émettant une requete basé sur le protocol Modbus,
 * puis ce met en attente d'une réponse provenant du capteur CO²
 * Cette fonction est un condancé des fonction 'send_Request', 'read_Response', et 'get_value'
 * d'où la necessité d'une structure.
 * 
 * @param RequestModbus Requete Modbus sous forme de tableau
 * @return unsigned long Renvoie la valeur retourné par le capteur.
 */
unsigned long req(RequeteModbus Request);


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

//Définition des requêtes Modbus nécessaire
byte Requests[SIZE_TBL_REQUESTS][SIZE_REQUEST] = {
    REQ_ABC, // [0]
    REQ_CO2 // [1]
};

RequeteModbus TblOfRequests[SIZE_TBL_REQUESTS] = {};



// ==============================================================================================
// ==                                                                                          ==
// ==                                       FONCTIONS                                          ==
// ==                                                                                          ==
// ==============================================================================================


void initializeRequests() {
    int tableSizeResponse[SIZE_TBL_REQUESTS] = {7, 7};

    for (int i = 0; i < SIZE_TBL_REQUESTS; i++) {
        //Serial.println("tableSizeResponse (i) = " + String(tableSizeResponse[i]));
        TblOfRequests[i] = initializeRequest(Requests[i], tableSizeResponse[i]);
    }
}

RequeteModbus initializeRequest(byte* Request, int sizeResponse) {

    RequeteModbus ReqMod;
    ReqMod.Request = Request;
    ReqMod.sizeResponse = sizeResponse;
    return ReqMod;
}

unsigned long  req(RequeteModbus Request) {
    //Envoie de la requete.
    _send_Request(Request.Request, SIZE_REQUEST);
    //Réception de la réponse.
    _read_Response(Request.sizeResponse);
   return _get_Value(Request.sizeResponse);
}

void checkCO2(unsigned long co2) {

    Serial.print("Qualiter de l'air : Air ");
    if (co2 < AIR_MOYEN) {
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

void setup() {
    // ports série et de communication capteur
    M5.begin(true,false,true);
    Serial1.begin(9600, SERIAL_8N1, PIN_SERIAL_RX, PIN_SERIAL_TX);

    //Initialisation du tableau des Requetes Modbus pour le capteur CO²
    initializeRequests();

    // TODO: Étant donnée que nous nous occupons pas du callibrage
    // je ne pense pas que celà sois utiliser de vérifier l'auto-callibrage.
    /*int abc = req(TblOfRequests[ABC]);
    Serial.print("Période ABC : ");
    Serial.printf("%02ld", abc);
    Serial.println();*/
}


void loop() {
    //Récupération du taux de CO²
    unsigned long co2 = req(TblOfRequests[CO2]);
    Serial.println("CO² = " + String(co2));

    //Controle du co2
    checkCO2(co2);

    //Attendre 10 secondes avant la prochaine mesure.
    delay(10000);
}