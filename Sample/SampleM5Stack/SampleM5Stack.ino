/*
 Name:		SampleM5Stack.ino
 Created:	14/12/2021 15:04:07
 Author:	MDQ09

Pour installer le supporte de la carte m5stack atom pour arduino, voir ici
https://docs.m5stack.com/en/quick_start/atom/arduino



 Lien utile:
 https://www.arduino.cc/en/Reference/softwareSerial
 http://nousaerons.fr/makersco2/#portfolioModa2000
 http://www.wiki-rennes.fr/Capteur_de_CO2_Climi
 http://nousaerons.fr/
 http://nousaerons.fr/makersco2/documents/co2meter_nousaerons_cablage_v9-1.pdf
*/
//SoftwareSerial mySerial(2, 3); // RX, TX
const int ledRedPin = 25;
const int ledGreenPin = 21;
const int serialRX = 33;
const int serialTX = 23;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, serialRX,serialTX);
  pinMode(ledGreenPin, OUTPUT);//vert
  pinMode(ledRedPin, OUTPUT);//rouge


 if(Serial1) {
    Serial.print("It's ok\n");
    digitalWrite(ledGreenPin, HIGH);
 }else {
    Serial.print("Error\n");
    digitalWrite(ledRedPin, HIGH);
 }
}

// the loop function runs over and over again until power down or reset
void loop() {
   if(Serial1.available()) {
    int inByte = Serial1.read();
    Serial.print("J'ai reçu : ");
    Serial.println(inByte, DEC);
    digitalWrite(ledGreenPin, HIGH);
    digitalWrite(ledRedPin, LOW);
    delay(4500);
 }else {
    digitalWrite(ledRedPin, HIGH);
    digitalWrite(ledGreenPin, LOW);
 }
}
