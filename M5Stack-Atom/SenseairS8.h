#ifndef SENSEAIR_S8_H
#define SENSEAIR_S8_H

#define DEBUG 1
#define ReqCo2 { 0xFE, 0X04, 0X00, 0X03, 0X00, 0X01, 0XD5, 0XC5 }

class ClassSenseairS8 {
    public:
        unsigned long getCO2();

    private:
        void _send_Request(byte*, int);
        void _read_Response(int);
        unsigned short int _crc(unsigned char*, int);
        unsigned long _get_Value(int);

    private:
        byte Response[20];
        unsigned long ReadCRC;      // CRC Control Return Code
};

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
            Response[i] = Serial1.read();

            Serial.print("0x");
            Serial.print(Response[i], HEX);
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
    ReadCRC = (uint16_t)Response[RS_len - 1] * 256 + (uint16_t)Response[RS_len - 2];
    if (_crc(Response, RS_len - 2) == ReadCRC) {
        // Read the Value //
        unsigned long val = (uint16_t)Response[3] * 256 + (uint16_t)Response[4];
        return val * 1;       // S8 = 1. K-30 3% = 3, K-33 ICB = 10
    }

    Serial.print("CRC Error");
    return 99;
}
/**
 * @brief Fonction émettant une requete basé sur le protocol Modbus,
 * puis ce met en attente d'une réponse provenant du capteur CO²
 * Cette fonction est un condancé des fonction 'send_Request', 'read_Response', et 'get_value'
 * Elle renvoie le taux de CO²
 * 
 * @return  taux de CO² ou 99 si erreur.
 */

unsigned long ClassSenseairS8::getCO2() {
    //Envoie de la requete.
    byte Request[] = ReqCo2;
    _send_Request(Request, 8);
    //Réception de la réponse.
    _read_Response(7);
   return _get_Value(7);
}
ClassSenseairS8 senseairS8;

#endif /* SENSEAIR_S8*/