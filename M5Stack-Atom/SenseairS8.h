
/**
 * @file SenseairS8.h
 * @author Julie Brindejont (julie.brindejont@gmail.com)
 * @brief Projet CO² Labfab
 * @version 1.1.0
 * @date 2022-02-24
 * 
 * @CC-BY-SA - 2022
 * 
 * Fichier source contenant le nécessaire pour dialoguer avec le capteur de CO² (SenseairS8)
 * Pour plus de clarté et de facilité, j'ai créer une classe (SenseairS8).
 * 
 * Certainne méthode de ce fichier sont inspiré d'un projet de détecteur CO² déjà existant.
 * @see 
 */
#ifndef SENSEAIR_S8_H
#define SENSEAIR_S8_H

// ==============================================================================================
// ==                                                                                          ==
// ==                                      CONSTANTES                                          ==
// ==                                                                                          ==
// ==============================================================================================
#define DEBUG 1

//Je pense qu'un délai 3 minutes au l'air frais doît être nécessaire
//En effet si je lance la calibration dès le début, celle-ci échoue. (45 secondes d'attente ne suffis pas.)
#define DELAI_ATTENTE_CAL   3 * 60 * 1000 //Délai de traitement pour le calibrage (180 000)
#define DELAI_CAL           6000 //Secondes correspondant à 2temp de la lampe du capteur. (6sec pour être sûr.)


//---------------------  Requête Modbus pour le capteur SenseairS8 -------------------------------
#define REQ_CO2       {0xFE, 0X04, 0X00, 0X03, 0X00, 0X01, 0XD5, 0XC5}
#define REQ_CLEAN_HR1 {0xFE, 0x06, 0x00, 0x00, 0x00, 0x00, 0x9D, 0xC5} 
#define REQ_CAL       {0xFE, 0x06, 0x00, 0x01, 0x7C, 0x06, 0x6C, 0xC7}
#define REQ_READ_HR1  {0xFE, 0x03, 0x00, 0x00, 0x00, 0x01, 0x90, 0x05}

//---------------------  Énumération pour les tâches a effectuer pendant la calibration -------------------------------
// Utile dans le cas d'une fonction non bloquante.
typedef enum {
    ClearHR1 = 0x01,
    Calibration = 0x02,
    ReadHR1 = 0x03

} StepCalibration;

// ==============================================================================================
// ==                                                                                          ==
// ==                                        CLASSE                                            ==
// ==                                                                                          ==
// ==============================================================================================

/**
 * @brief Classe permettant de gérer le capteur SenseairS8
 * 
 */
class ClassSenseairS8 {
    public:
        //---------------------------  Méthode Public concernant les requests --------------------------------
        unsigned long reqCO2();
        unsigned long ReqCleanHR1();
        unsigned long ReqCalibration();
        unsigned long ReqReadHR1();

        //---------------------------  Méthode Public utilitaire --------------------------------
        //TODO: Méthode Béta, semble fonctionnel.
        bool progressCalibration(unsigned long); //Ceci est une méthode non bloquante utilisé avec millis().

        //---------------------------  Méthode Public pour gérer des erreurs. --------------------------------
        bool checkError() { return _errorInCalibration; };
        void reqCalibration();

    private:
        //---------------------------  Méthode Public concernant les requests --------------------------------
        //Méthode gérant le protocol Modbus.
        void _send_Request(byte*, int);
        void _read_Response(int);
        unsigned short int _crc(unsigned char*, int);
        unsigned long _get_Value(int);

    private:
        //---------------------------  Paramètre privé. --------------------------------
        byte            _response[20];
        unsigned long   _readCRC;      
        unsigned long   _startTime                          = 0;
        int             _bloquerFonctionPendantCalibration  = 1;
        bool            _errorInCalibration                 = 0;
        StepCalibration _stepCalibration                    = StepCalibration::ClearHR1;


};


/**
 * @brief Cette fonction est non bloquante,
 * Elle permet de calibrer le capteur.
 * ATTENTION Cette fonction doit être appellée en tant que condition dans une boucle while.
 * Ex: 
 * while(senseairS8.progressCalibration()){
 *     //instruction.
 * }
 * 
 * @param ms 
 * @return true 
 * @return false 
 */
bool ClassSenseairS8::progressCalibration(unsigned long ms = DELAI_ATTENTE_CAL) {
    unsigned long currentTime = millis();

    //Empêche le bloqauge pendant le traitement du calibrage par cette fonction.
    if (_bloquerFonctionPendantCalibration == 1) {
        _bloquerFonctionPendantCalibration = 0;
    }

    //Initialisation startime.
    if (_startTime == 0) {
        //Espérent éviter la coupure prématuré.
        _startTime = currentTime;
    }
    //Initialisation des milisecondes nécessaire pour certainne instruction.
    const unsigned long delayForCleanHr1 = ms - DELAI_CAL - 1000; //30 - 6 - 0.2 = 23.8 sec (demande l'effacement de HR1 à ce délai.)
    const unsigned long delayForCalibration = ms - DELAI_CAL - 500; //30 - 6 - 0.1 = 23.9 sec (lance la calibration à ce délai.)
    const unsigned long delayForReadHR1 = ms - 500; //30 - 0.1 = 29.9 sec (lance la lecture de registre HR1 à ce délai.)

    //Cette condition devrais faire en sorte de réinitialiser startTime.
    //Sans pour autant recommancer le calibrage de 0.
    if (currentTime == 0 && _startTime != 0) {
        _startTime = 0;
        return 1;//je renvoie 1, inutile de continuer.
    }

   //Contrôle des instructions nécessaire pour le calibrage et appelle de celles-ci.
    if (_stepCalibration == StepCalibration::ClearHR1 && currentTime - _startTime == delayForCleanHr1) {
        ReqCleanHR1();
        _stepCalibration = StepCalibration::Calibration;
    }else if (_stepCalibration == StepCalibration::Calibration && currentTime - _startTime == delayForCalibration) {
         ReqCalibration();
        _stepCalibration = StepCalibration::ReadHR1;
    }else if (_stepCalibration == StepCalibration::ReadHR1 && currentTime - _startTime == delayForReadHR1) {
        ReqReadHR1();
        _stepCalibration = StepCalibration::ClearHR1;//Réinitialisation de stepCalibration.
    }

     //Le temps est écoulé, mettre à jour startTime  et renvoyer 0.
    if (currentTime - _startTime >= ms) {
        _startTime = 0; 
        return 0;
    }
    return 1;
}


/**
 * @brief Fonction émettant une requete basé sur le protocol Modbus,
 * Cette fonction est un condancé des fonction 'send_Request', 'read_Response', et 'get_value'
 * Elle renvoie le taux de CO²
 * 
 * @return  taux de CO² ou 99 si erreur.
 */

unsigned long ClassSenseairS8::reqCO2() {
    //Envoie de la requete.
    byte Request[] = REQ_CO2;
    _send_Request(Request, 8);
    //Réception de la réponse.
    _read_Response(7);
   return _get_Value(7);
}

/**
 * @brief Fonction émettant une requete basé sur le protocol Modbus,
 * Cette fonction est un condancé des fonction 'send_Request', 'read_Response', et 'get_value'
 * Elle efface le registre HR1
 * 
 * @return unsigned long 
 */
unsigned long ClassSenseairS8::ReqCleanHR1() {
    //Effacer le registre HR1 (acknowledgement register) à 0
    // valeur de retour 0. sois : 0xFE, 0x06, 0x00, 0x00, 0x00, 0x00, 0x9D, 0xC6
    byte Request[] = REQ_CLEAN_HR1;
    //Envoie de la requete.
    _send_Request(Request, 8);
    //Réception de la réponse.
    _read_Response(8);

    if (_bloquerFonctionPendantCalibration) {
        delay(500);
    }
    unsigned long value = _get_Value(8);

    if(value != 0) {
        Serial.print("Une erreur est survenue pendant l'effacement du registre HR1.");
        _errorInCalibration = 1;
    }
    return value;
}

/**
 * @brief Fonction émettant une requete basé sur le protocol Modbus,
 * Cette fonction est un condancé des fonction 'send_Request', 'read_Response', et 'get_value'
 * Elle effectue la calibration du détecteur SenseairS8.
 * 
 * @return unsigned long 
 */
unsigned long ClassSenseairS8::ReqCalibration() {
    //Calibration
    byte RequestCal[] = REQ_CAL;
    _send_Request(RequestCal, 8);
    //Réception de la réponse.
    _read_Response(8);

    if (_bloquerFonctionPendantCalibration) {
        delay(DELAI_CAL);
    }
    unsigned long value = _get_Value(8);
    if(value != 380) {
        Serial.print("Une erreur est survenue pendant le callibrage.");
        _errorInCalibration = 1;
    }

    return value;
}

/**
 * @brief Fonction émettant une requete basé sur le protocol Modbus,
 * Cette fonction est un condancé des fonction 'send_Request', 'read_Response', et 'get_value'
 * Elle effectue une lecture du registre HR1
 * 
 * @return unsigned long 
 */
unsigned long ClassSenseairS8::ReqReadHR1() {

    //Lire le contenus du registre HR1
    byte RequestRead[] = REQ_READ_HR1;
    _send_Request(RequestRead, 8);
    //Réception de la réponse.
    _read_Response(7);

    if (_bloquerFonctionPendantCalibration) {
        delay(500);
    }
    unsigned long value = _get_Value(7);

    if(value != 32) {
        Serial.print("Une erreur est survenue pendant la lecture du registre HR1.");
        _errorInCalibration = 1;
    }
    return value;
}
// ==============================================================================================
// ==                                                                                          ==
// ==                 METHODE PRIVER (gére le protocol modbus)                                 ==
// ==                                                                                          ==
// ==============================================================================================
/**
 * @brief Comme son nom l'indique cette fonction envoie une requete au capteur CO²
 * 
 * @param Request 
 * @param Re_len 
 */
void ClassSenseairS8::_send_Request(byte* Request, int Re_len) {
    //Si au bout de 10 fois, aucun retour n'a été reçu de la part
    //quiter la boucle et afficher erreur.
    int i = 0;
    while (!Serial1.available() && i != 10) 
    {
        i++;
        Serial1.write(Request, Re_len);   // Envoie de la requête au capteur S8
        delay(50);
    }

    if (i == 10 && !Serial1.available())  {
        Serial.println("Erreur: Le capteur ne semble pas avoir reçu la requete.");
        Serial.println("Vérifié les branchement RX/TX.");
    }


    //Eventuellement, mettre cette séquence en debuggage.
    if (DEBUG) {
        Serial.print("Requete : ");
        for (int i = 0; i < Re_len; i++)    // Récupère les octets.
        {
            Serial.print("0x");
            Serial.print(Request[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
}

/**
 * @brief Récupère le retour du capteur CO²
 * 
 * @param RS_len longeur de la réponse cencé être reçue.
 * @return byte* Renvoie un tableau.
 */
void ClassSenseairS8::_read_Response(int RS_len) {
    
    if (Serial1.available() == 1) {
        Serial.println("Erreur: Le programme à reçu qu'un seul caractere. qui semble correspondre à une broche PWM.");
        Serial.println("Vérifié les branchement RX/TX.");
        byte caractere = Serial1.read();
        Serial.println("Caractere détecter :" + String(caractere));

        return; //je fais un return sinon, je suis bonne pour une boucle infinie.
    }
    int i = 0;
    //Boucle, pour purger le buffer, si celui-ci est inferieur à 7.
    while (Serial1.available() < 7)
    {
        i++;
        if (i > 10)
        {
            while (Serial1.available())
               Serial1.read();
            break;
        }
        delay(50);
    }

    if (DEBUG) {
        Serial.print("Reponse : ");
        for (int i = 0; i < RS_len; i++)    // Récupère les octets.
        {
            _response[i] = Serial1.read();

            Serial.print("0x");
            Serial.print(_response[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
}

unsigned short int ClassSenseairS8::_crc(unsigned char* buf, int len)
{
    unsigned short int crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++) {
        crc ^= (unsigned short int)buf[pos];   // XOR byte into least sig. byte of crc
        for (int i = 8; i != 0; i--) {         // Loop over each bit
            if ((crc & 0x0001) != 0) {           // If the LSB is set
                crc >>= 1;                         // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else                            // else LSB is not set
                crc >>= 1;                    // Just shift right
        }
    }  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    return crc;
}

unsigned long ClassSenseairS8::_get_Value(int RS_len)
{
    
    // Check the CRC //
    _readCRC = (uint16_t)_response[RS_len - 1] * 256 + (uint16_t)_response[RS_len - 2];
    if (_crc(_response, RS_len - 2) == _readCRC) {
        // Read the Value //
        unsigned long val = (uint16_t)_response[3] * 256 + (uint16_t)_response[4];
        return val * 1;       // S8 = 1. K-30 3% = 3, K-33 ICB = 10
    }

    Serial.println("Erreur pendant la récupération du CRC.");
    return 99;
}

ClassSenseairS8 senseairS8;

#endif /* SENSEAIR_S8*/
