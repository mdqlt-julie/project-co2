#include <iostream>
#include <vector>
#include <string>

#include "config.h"

unsigned long ReadCRC;      // CRC Control Return Code 

// Fonction reprise pour calculer le CRC
uint16_t ModRTU_CRC(unsigned char* buf, int len)
{
  uint16_t crc = 0xFFFF;
  
  for (int pos = 0; pos < len; pos++) {
    crc ^= (uint16_t)buf[pos];          // XOR byte into least sig. byte of crc
    for (int i = 8; i != 0; i--) {    // Loop over each bit
        if ((crc & 0x0001) != 0) {      // If the LSB is set
            crc >>= 1;                    // Shift right and XOR 0xA001
            crc ^= 0xA001;
        }
        else                            // Else LSB is not set
            crc >>= 1;                    // Just shift right
        
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;  
}



int main()
{
    //Cette définition correspond à une requete sur 

    //

    byte CO2req[] = { 0xFE, 0X04, 0X00, 0X03, 0X00, 0X01, 0XD5, 0XC5 };

    std::cout << std::hex << ModRTU_CRC(CO2req, 8) << std::endl;
    std::cout << std::hex << ModRTU_CRC(CO2req, 6) << std::endl;

    ReadCRC = (uint16_t)CO2req[8 - 1] * 256 ;

    std::cout << std::hex << ReadCRC << std::endl;

    ReadCRC = (uint16_t)CO2req[8 - 1] * 256  + (uint16_t)CO2req[8 - 2];

    std::cout << std::hex << ReadCRC << std::endl;


}