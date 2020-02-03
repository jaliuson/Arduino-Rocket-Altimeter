#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < EEPROM.length() ; i++){
    Serial.print(EEPROM.read(i));
    Serial.print("\n");
  }
}

void loop() {
}
