#include <EEPROM.h>
#include "restart.h"

#define EEPROM_CHK 1
#define EEPROM_STATE 0

//WiFiClient espClient;

void saveEEPROM(uint8_t state){
    EEPROM.write(EEPROM_STATE,state);
    EEPROM.commit();
}

void readEEPROM(){
  if ('R' == EEPROM.read(EEPROM_CHK)) {
    digitalWrite(PIN_THERM,EEPROM.read(EEPROM_STATE));
    debugSerial.printf("Restoring previous state: %s",(EEPROM.read(EEPROM_STATE) == HIGH)? "Off":"On" );
  } else {
    debugSerial.printf("EEPROM not initialized (%d). Initializing...",EEPROM.read(EEPROM_CHK));
    EEPROM.write(EEPROM_CHK,'R');
    EEPROM.write(EEPROM_STATE,HIGH);
    EEPROM.commit();
    digitalWrite(PIN_THERM,HIGH);
  }
}


void callbackTherm(byte *payload, unsigned int length) {
  payload[length] = '\0';

  // Is it ON or OFF?
  // Ok I'm not super proud of this, but it works :p
  if (payload[1] == 'F') { 
    //turn off
    digitalWrite(PIN_THERM, HIGH);
    saveEEPROM(HIGH);
    debugSerial.println("Turned OFF");
  } else if (payload[1] == 'N') { 
    //turn on
    digitalWrite(PIN_THERM, LOW);
    saveEEPROM(LOW);
    debugSerial.println("Turned ON");
  } else if (payload[0] == 'R') {
    //R(eset/eboot)
    debugSerial.println("Rebooting");
    delay(100);
    restart_board();
  } else {
    Serial.printf("Unknown message: %s\n", payload);
  }
}

#ifdef PIN_SG1
  //Smartgrid callbacks
  void callbackSg(byte *payload, unsigned int length) {
    payload[length] = '\0';

    if (payload[0] == '0') {
      // Set SG 0 mode => SG1 = INACTIVE, SG2 = INACTIVE
      digitalWrite(PIN_SG1, SG_RELAY_INACTIVE_STATE);
      digitalWrite(PIN_SG2, SG_RELAY_INACTIVE_STATE);
      Serial.println("Set SG mode to 0 - Normal operation");
    } else if (payload[0] == '1') {
      // Set SG 1 mode => SG1 = INACTIVE, SG2 = ACTIVE
      digitalWrite(PIN_SG1, SG_RELAY_INACTIVE_STATE);
      digitalWrite(PIN_SG2, SG_RELAY_ACTIVE_STATE);
      Serial.println("Set SG mode to 1 - Forced OFF");
    } else if (payload[0] == '2') {
      // Set SG 2 mode => SG1 = ACTIVE, SG2 = INACTIVE
      digitalWrite(PIN_SG1, SG_RELAY_ACTIVE_STATE);
      digitalWrite(PIN_SG2, SG_RELAY_INACTIVE_STATE);
      Serial.println("Set SG mode to 2 - Recommended ON");
    } else if (payload[0] == '3') {
      // Set SG 3 mode => SG1 = ACTIVE, SG2 = ACTIVE
      digitalWrite(PIN_SG1, SG_RELAY_ACTIVE_STATE);
      digitalWrite(PIN_SG2, SG_RELAY_ACTIVE_STATE);
      Serial.println("Set SG mode to 3 - Forced ON");
    } else {
      Serial.printf("Unknown message: %s\n", payload);
    }
  }
#endif

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.printf("Message arrived [%s] : %s\n", topic, payload);

  if (strcmp(topic, "espaltherma/POWER") == 0) {
    callbackTherm(payload, length);
  }
  #ifdef PIN_SG1
    else if (strcmp(topic, "espaltherma/sg/set") == 0) {
      callbackSg(payload, length);
    }
  #endif
  else {
    Serial.printf("Unknown topic: %s\n", topic);
  }
}
