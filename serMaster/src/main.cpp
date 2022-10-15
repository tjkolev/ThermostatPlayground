#include <Arduino.h>
#include <Wire.h>

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.setTimeout(500);

  Serial1.begin(9600);
}

const char* messages[] = {
  "Hello",
  "World",
  "Do you copy?",
  "Over"
};
int msgCount = 4;
int counter = 0;
int msgNdx = 0;

unsigned long lastLoopRun = 0;
char serBuff[64];
int bytesAvailable = 0;
int bytesRead = 0;
void loop() {
  if(millis() - lastLoopRun >= 2000) {

    msgNdx = (counter++) % msgCount;
    Serial.println(messages[msgNdx]);
    Serial1.println(messages[msgNdx]);

    int bytesAvailable = Serial.available();
    if(bytesAvailable > 0) {
      bytesRead = Serial.readBytes(serBuff, bytesAvailable);
      serBuff[bytesRead] = 0x00;
      //Serial.println(serBuff);
      Serial1.println(serBuff);
    }
    else {
      //Serial.println("...");
      Serial1.println("...");
    }
    lastLoopRun = millis();
  }

  yield();
}