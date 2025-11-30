#ifndef Utilities_h
#define Utilities_h

bool contains(char array[], int size, int value) {
  for (int i = 0; i < size; i++) {
    if (array[i] == value)
      return true;
  }
  return false;
}

unsigned char getCRC(unsigned char *src, int len) {
  unsigned char b = 0;
  for (int i = 0; i < len; i++) {
    b += src[i];
  }
  return ~b;
}

void restartBoard() {
    ESP.restart();
}

void waitLoop(uint ms) {
  unsigned long start = millis();
  while (millis() < start + ms) {
    //wait .5sec between registries  
    
  }
}

#endif /*Utilities_h*/
