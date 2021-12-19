/**
 * @file Modbus.h
 * @author Julie Brindejont (julie.brindejont@gmail.com)
 * @brief Projet CO²
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 * La plus par des fonctions ci-dessous ont été inspiré d'un fichier source.
 * @see https://co2.rinolfi.ch/stl/capteurCO2offline.ino
 */

#ifndef MODBUS_H
#define MODBUS_H

byte Response[20];
unsigned long ReadCRC;      // CRC Control Return Code 
/**
 * @brief Comme son nom l'indique cette fonction envoie une requete au capteur CO²
 * 
 * @param Request 
 * @param Re_len 
 */
void _send_Request(byte* Request, int Re_len)
{
    while(!Serial1.available()) {
        Serial1.write(Request, Re_len);   // Envoie de la requête au capteur S8
        delay(50);
    }

    //Eventuellement, mettre cette séquence en debuggage.
    Serial.print("Requete : ");
    for (int i = 0; i < Re_len; i++)    // Récupère les octets.
    {
        Serial.print("0x");
        Serial.print(Request[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

}

/**
 * @brief Récupère le retour du capteur CO²
 * 
 * @param RS_len longeur de la réponse cencé être reçue.
 * @return byte* Renvoie un tableau.
 */
void _read_Response(int RS_len)
{
    
    int i = 0;
    //Boucle, pour purger le buffer, si celui-ci est inferieur à 7.
    while (Serial1.available() < 7)
    {
        i++;
        if (i > 10)
        {
            Serial.println(Serial1.available());
            while (Serial1.available()) {
                byte inByte = Serial1.read();
                Serial.print(". 0x");
                Serial.print(inByte, HEX);
                Serial.println();
               // Serial1.read();
            }
            break;
        }
        delay(50);
    }

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

unsigned short int ModBus_CRC(unsigned char* buf, int len)
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

unsigned long _get_Value(int RS_len)
{
    
    // Check the CRC //
    ReadCRC = (uint16_t)Response[RS_len - 1] * 256 + (uint16_t)Response[RS_len - 2];
    if (ModBus_CRC(Response, RS_len - 2) == ReadCRC) {
        // Read the Value //
        unsigned long val = (uint16_t)Response[3] * 256 + (uint16_t)Response[4];
        return val * 1;       // S8 = 1. K-30 3% = 3, K-33 ICB = 10
    }

    Serial.print("CRC Error");
    return 99;
}

#endif