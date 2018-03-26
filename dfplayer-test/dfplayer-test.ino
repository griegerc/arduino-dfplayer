/**
 * (c) 2018 Christian Grieger
 * GNU GENERAL PUBLIC LICENSE
 */

#define PIN_RX 8
#define PIN_TX 9

#include <SoftwareSerial.h>

SoftwareSerial serial(PIN_RX, PIN_TX);
uint8_t mp3Buffer[10];

void sendMp3Buffer() 
{
  for(byte i=0; i<10; i++) {
    serial.write(mp3Buffer[i]);  
  }
}

void sendMp3Command(uint8_t cmd, uint8_t param1, uint8_t param2, uint8_t feedback)
{
  // initial bytes
  mp3Buffer[0] = 0x7E; // start byte
  mp3Buffer[1] = 0xFF; // version byte
  mp3Buffer[2] = 0x06; // number of bytes with the version byte including the LSB of the param (always 0x06)

  // Assemble command
  mp3Buffer[3] = cmd;
  mp3Buffer[4] = feedback; // currently not used
  mp3Buffer[5] = param1;
  mp3Buffer[6] = param2;

  // calculate checksum
  uint8_t checksum = 0;
  for(byte i=1; i<7; i++) {
    checksum += mp3Buffer[i];
  }
  checksum = 0 - checksum;

  mp3Buffer[7] = highByte(checksum);
  mp3Buffer[8] = lowByte(checksum);

  mp3Buffer[9] = 0xEF; // end byte

  sendMp3Buffer();
}

void mp3Play(uint16_t idx)
{
  sendMp3Command(0x0D, highByte(idx), lowByte(idx), 0);
}

void mp3Volume(uint8_t volume)
{
  volume = max(0, min(30, volume));
  
  sendMp3Command(0x06, 0, volume, 0);
}

void setup() 
{
  serial.begin(9600);

  mp3Volume(20); // [0..30]
  delay(3000); // wait for reading the file system on the sd card

  mp3Play(1);
  delay(5000);

  mp3Play(2);
  delay(5000);

  mp3Play(3);
  delay(5000);
}

void loop()
{
}
