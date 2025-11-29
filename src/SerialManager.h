#ifndef SerialManager_h
#define SerialManager_h

#include <HardwareSerial.h>
#include <Converters.h>

#define SERIAL_FLUSH_TX_ONLY false
#define SER_TIMEOUT 300 //leave 300ms for the machine to answer

HardwareSerial DaikinSerial(SERIAL_PORT);

char registryIDs[32]; //Holds the registries to query

Converters converter;
bool busy = false;

JsonDocument jsonData;
String outJson;    

void initSerial() {  
  DaikinSerial.begin(9600, SERIAL_8E1, SERIAL_RX_PIN, SERIAL_TX_PIN);
}

void logBuffer(unsigned char *buffer, size_t len) {
  char bufflog[250] = {0};
  for (size_t i = 0; i < len; i++) {
    sprintf(bufflog + i * 5, "0x%02x ", buffer[i]);
  }
  debugSerial.print(bufflog);
}

int get_reply_len(char regID, char protocol = 'I') {
  if (protocol == 'I') {
    // Backward compatible behavior. Actual length is dynamic and returned
    // on 3rd byte of the response.
    return 12;
  } else {
    // Protocol S has hard-coded values based on the requested registry
    switch (regID) {
      case 0x50:
        return 6;
        break;
      case 0x56:
        return 4;
        break;
      default:
        return 18;
        break;
    }
  }
}

bool queryRegistry(char regID, unsigned char *buffer, char protocol = 'I') {
  //preparing command:
  unsigned char prep[] = {0x03, 0x40, regID, 0x00};
  prep[3] = getCRC(prep, 3);
  int queryLength = 4;

  if (protocol == 'S') {
    prep[0] = 0x02;
    prep[1] = regID;
    prep[2] = getCRC(prep, 2);
    prep[3] = 0;
    queryLength = 3;
  }

  debugSerial.printf("Querying register 0x%02x... ", regID);
  //Sending command to serial
  DaikinSerial.flush(SERIAL_FLUSH_TX_ONLY); //Prevent possible pending info on the read
  DaikinSerial.write((uint8_t*) prep, queryLength);
  ulong start = millis();

  int len = 0;
  int replyLen = get_reply_len(regID, protocol);

  while ((len < replyLen) && (millis() < (start + SER_TIMEOUT))) {
    if (DaikinSerial.available()) {
      buffer[len++] = DaikinSerial.read();
      if (protocol == 'I' && len == 3) {
        // Override reply length with the actual one (not counting already read bytes, see doc/Daikin I protocol.md)
        replyLen = buffer[2] + 2;
      }
      // Error reply common to both protocols
      if (len == 2 && buffer[0] == 0x15 && buffer[1] == 0xea) {
        // HP didn't understand the command
        debugSerial.printf("Error 0x15 0xEA returned from HP\n");
        delay(500);
        return false;
      }
    }
  }
  if (millis() >= (start + SER_TIMEOUT)) {
    if (len == 0) {
      debugSerial.printf("Time out! Check connection\n");
    } else {
      debugSerial.printf("ERR: Time out on register 0x%02x! got %d/%d bytes\n", regID, len, replyLen);
      logBuffer(buffer, len);
    }
    delay(500);
    return false;
  }
  logBuffer(buffer, len);
  if (getCRC(buffer, len - 1) != buffer[len - 1]) {
    debugSerial.printf("ERROR: Wrong CRC on register 0x%02x. Calculated 0x%2x but got 0x%2x\nBuffer: ", regID, getCRC(buffer, len - 1), buffer[len - 1]);
    logBuffer(buffer,len);
    return false;
  } else {
    debugSerial.println(".. CRC OK!");
    return true;
  }
}

//Converts to string and add the value to the JSON message
void updateValues(char regID) {
  LabelDef *labels[128];
  int num = 0;
  converter.getLabels(regID, labels, num);
  for (int i = 0; i < num; i++) {
    bool alpha = false;
    for (size_t j = 0; j < strlen(labels[i]->asString); j++) {
      char c = labels[i]->asString[j];
      if (!isdigit(c) && c!='.' && !(c=='-' && j==0)) {
        alpha = true;
        break;
      }
    }
    jsonData[labels[i]->label] = labels[i]->asString;
  }
}

void getValues() {
    //jsonData.clear();    
    jsonData["time"] = millis();
  //Querying all registries
  for (size_t i = 0; (i < 32) && registryIDs[i] != 0xFF; i++) {
    unsigned char buff[64] = {0};
    int tries = 0;
    while (!queryRegistry(registryIDs[i], buff, PROTOCOL) && tries++ < 3) {        
      waitLoop(1000);
    }
    unsigned char receivedRegistryID = PROTOCOL == 'S' ? buff[0] : buff[1];
    if (registryIDs[i] == receivedRegistryID) {
      //if replied registerID is coherent with the command    
      converter.readRegistryValues(buff, PROTOCOL); //process all values from the register
      updateValues(registryIDs[i]);       //send them to buffer
      //waitLoop(500);//wait .5sec between registries
    }
  }
  serializeJson(jsonData, outJson);    
}

void initRegistries() {
  //getting the list of registries to query from the selected values
  for (size_t i = 0; i < sizeof(registryIDs); i++) {
    registryIDs[i]=0xff;
  }

  int i = 0;
  for (auto &&label : labelDefs) {
    if (!contains(registryIDs, sizeof(registryIDs), label.registryID)) {
      debugSerial.printf("Adding registry 0x%2x to be queried.\n", label.registryID);
      registryIDs[i++] = label.registryID;      
    }
  }
  if (i == 0) {
    debugSerial.printf("ERROR - No values selected in the include file. Stopping.\n");
    while (true) {
      extraLoop();
    }
  }
}

#endif // SerialManager_h