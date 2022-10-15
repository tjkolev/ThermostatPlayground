#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.setTimeout(500);
}

unsigned long lastLoopRun = 0;
char serBuff[64];
int bytesAvailable = 0;
int bytesRead = 0;
void loop() {
  if(millis() - lastLoopRun >= 2000) {

    int bytesAvailable = Serial.available();
    if(bytesAvailable > 0) {
      bytesRead = Serial.readBytes(serBuff, bytesAvailable);
      serBuff[bytesRead] = 0x00;
      Serial.print((int)(millis()/1000));
      Serial.print(' ');
      Serial.print(serBuff);
    }
    else {
      //Serial.println("...");
    }
    lastLoopRun = millis();
  }

  yield();
}
