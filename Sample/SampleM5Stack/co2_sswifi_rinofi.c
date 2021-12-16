/************************************************
 *
 *   Capteur de CO2 par Grégoire Rinolfi
 *   https://co2.rinolfi.ch
 *
 ***********************************************/
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>

TFT_eSPI tft = TFT_eSPI(135, 240);

/************************************************
 *
 *   Paramètres utilisateur
 *
 ***********************************************/

#define TXD2 21         // série capteur TX
#define RXD2 22         // série capteur RX
#define BOUTON_CAL 35
#define DEBOUNCE_TIME 1000

 /************************************************
  *
  *   Code de gestion du capteur CO2 via ModBus
  *   inspiré de : https://github.com/SFeli/ESP32_S8
  *
  ***********************************************/
volatile uint32_t DebounceTimer = 0;

byte CO2req[] = { 0xFE, 0X04, 0X00, 0X03, 0X00, 0X01, 0XD5, 0XC5 };
byte ABCreq[] = { 0xFE, 0X03, 0X00, 0X1F, 0X00, 0X01, 0XA1, 0XC3 };
byte disableABC[] = { 0xFE, 0X06, 0X00, 0X1F, 0X00, 0X00, 0XAC, 0X03 };  // écrit la période 0 dans le registre HR32 �  adresse 0x001f
byte enableABC[] = { 0xFE, 0X06, 0X00, 0X1F, 0X00, 0XB4, 0XAC, 0X74 }; // écrit la période 180
byte clearHR1[] = { 0xFE, 0X06, 0X00, 0X00, 0X00, 0X00, 0X9D, 0XC5 }; // ecrit 0 dans le registe HR1 adresse 0x00
byte HR1req[] = { 0xFE, 0X03, 0X00, 0X00, 0X00, 0X01, 0X90, 0X05 }; // lit le registre HR1 (vérifier bit 5 = 1 )
byte calReq[] = { 0xFE, 0X06, 0X00, 0X01, 0X7C, 0X06, 0X6C, 0XC7 }; // commence la calibration background
byte Response[20];
uint16_t crc_02;
int ASCII_WERT;
int int01, int02, int03;
unsigned long ReadCRC;      // CRC Control Return Code 

void send_Request(byte* Request, int Re_len)
{
    while (!Serial1.available())
    {
        Serial1.write(Request, Re_len);   // Send request to S8-Sensor
        delay(50);
    }

    Serial.print("Requete : ");
    for (int02 = 0; int02 < Re_len; int02++)    // Empfangsbytes
    {
        Serial.print(Request[int02], HEX);
        Serial.print(" ");
    }
    Serial.println();

}

void read_Response(int RS_len)
{
    int01 = 0;
    while (Serial1.available() < 7)
    {
        int01++;
        if (int01 > 10)
        {
            while (Serial1.available())
                Serial1.read();
            break;
        }
        delay(50);
    }

    Serial.print("Reponse : ");
    for (int02 = 0; int02 < RS_len; int02++)    // Empfangsbytes
    {
        Response[int02] = Serial1.read();

        Serial.print(Response[int02], HEX);
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

unsigned long get_Value(int RS_len)
{

    // Check the CRC //
    ReadCRC = (uint16_t)Response[RS_len - 1] * 256 + (uint16_t)Response[RS_len - 2];
    if (ModBus_CRC(Response, RS_len - 2) == ReadCRC) {
        // Read the Value //
        unsigned long val = (uint16_t)Response[3] * 256 + (uint16_t)Response[4];
        return val * 1;       // S8 = 1. K-30 3% = 3, K-33 ICB = 10
    }
    else {
        Serial.print("CRC Error");
        return 99;
    }
}

// interruption pour lire le bouton d'étalonnage
bool demandeEtalonnage = false;
void IRAM_ATTR etalonnage() {
    if (millis() - DEBOUNCE_TIME >= DebounceTimer) {
        DebounceTimer = millis();

        Serial.println("Etalonnage manuel !!");

        tft.fillScreen(TFT_BLACK);
        tft.setTextSize(3);
        tft.setTextColor(TFT_WHITE);
        tft.drawString("Etalonnage", tft.width() / 2, tft.height() / 2);

        demandeEtalonnage = true;
    }
}

// nettoie l'écran et affiche les infos utiles
void prepareEcran() {
    tft.fillScreen(TFT_BLACK);
    // texte co2 �  gauche
    tft.setTextSize(4);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("CO", 20, 110);
    tft.setTextSize(3);
    tft.drawString("2", 55, 115);

    // texte PPM �  droite ppm
    tft.drawString("ppm", 190, 105);

    // écriture du chiffre
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(8);
}

void setup() {
    // bouton de calibration
    pinMode(BOUTON_CAL, INPUT);

    // ports série de debug et de communication capteur
    Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);

    // initialise l'écran
    tft.init();
    delay(20);
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM); // imprime la string middle centre

    // vérifie l'état de l'ABC
    send_Request(ABCreq, 8);
    read_Response(7);
    Serial.print("Période ABC : ");
    Serial.printf("%02ld", get_Value(7));
    Serial.println();
    int abc = get_Value(7);

    // active ou désactive l'ABC au démarrage
    if (digitalRead(BOUTON_CAL) == LOW) {
        if (abc == 0) {
            send_Request(enableABC, 8);
        }
        else {
            send_Request(disableABC, 8);
        }
        read_Response(7);
        get_Value(7);
    }

    tft.setTextSize(2);
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.drawString("Autocalibration", tft.width() / 2, 10);
    if (abc != 0) {
        tft.drawString(String(abc) + "h", tft.width() / 2, 40);
    }
    else {
        tft.drawString("OFF", tft.width() / 2, 40);
    }

    delay(3000); // laisse un temps pour lire les infos

    // préparation de l'écran
    prepareEcran();

    //interruption de lecture du bouton
    attachInterrupt(BOUTON_CAL, etalonnage, FALLING);
}

unsigned long ancienCO2 = 0;
int seuil = 0;

void loop() {

    // effectue l'étalonnage si on a appuyé sur le bouton
    if (demandeEtalonnage) {
        demandeEtalonnage = false;
        // nettoye le registre de verification
        send_Request(clearHR1, 8);
        read_Response(8);
        delay(100);
        // demande la calibration
        send_Request(calReq, 8);
        read_Response(8);
        delay(4500); // attend selon le cycle de la lampe

        // lit le registre de verification
        send_Request(HR1req, 8);
        read_Response(7);
        int verif = get_Value(7);
        Serial.println("resultat calibration " + String(verif));
        if (verif == 32) {
            tft.setTextColor(TFT_GREEN);
            tft.drawString("OK", tft.width() / 2, tft.height() / 2 + 30);
        }
        else {
            tft.setTextColor(TFT_RED);
            tft.drawString("Erreur", tft.width() / 2, tft.height() / 2 + 20);
        }
        delay(3000);
        prepareEcran();
        seuil = 0;
    }

    // lecture du capteur
    send_Request(CO2req, 8);
    read_Response(7);
    unsigned long CO2 = get_Value(7);

    String CO2s = "CO2: " + String(CO2);
    Serial.println(CO2s);

    // efface le chiffre du texte
    if (CO2 != ancienCO2) {
        tft.fillRect(0, 0, tft.width(), 60, TFT_BLACK);
    }

    if (CO2 < 800) {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        if (seuil != 1) {
            tft.setTextSize(2);
            tft.fillRect(0, 61, tft.width(), 25, TFT_BLACK);
            tft.drawString("Air Excellent", tft.width() / 2, tft.height() / 2 + 10);
        }
        seuil = 1;
    }
    else if (CO2 >= 800 && CO2 < 1000) {
        tft.setTextColor(TFT_ORANGE, TFT_BLACK);
        if (seuil != 2) {
            tft.setTextSize(2);
            tft.fillRect(0, 61, tft.width(), 25, TFT_BLACK);
            tft.drawString("Air Moyen", tft.width() / 2, tft.height() / 2 + 10);
        }
        seuil = 2;
    }
    else if (CO2 >= 1000 && CO2 < 1500) {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        if (seuil != 3) {
            tft.setTextSize(2);
            tft.fillRect(0, 61, tft.width(), 25, TFT_BLACK);
            tft.drawString("Air Mediocre", tft.width() / 2, tft.height() / 2 + 10);
        }
        seuil = 3;
    }
    else {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        if (seuil != 4) {
            tft.setTextSize(2);
            tft.fillRect(0, 61, tft.width(), 25, TFT_BLACK);
            tft.drawString("Air Vicie", tft.width() / 2, tft.height() / 2 + 10);
        }
        seuil = 4;
    }

    tft.setTextSize(8);
    tft.drawString(String(CO2), tft.width() / 2, tft.height() / 2 - 30);

    ancienCO2 = CO2;
    delay(10000); // attend 10secondes avant la prochaine mesure
}
