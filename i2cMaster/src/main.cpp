#include <Arduino.h>
#include <Wire.h>

void setup()
{
  Serial.begin(115200);		 // Start the Serial communication to send messages to the computer
  delay(100);

  Wire.begin();
  Wire.setClock(1000); // 10KHz, go slow
}

void sendMessage(const char* msg) {
  Wire.beginTransmission(9);
  Wire.write(msg);
  int errorCode = Wire.endTransmission(false);
  if(errorCode != 0) {
    Serial.print("endTransmission error: "); Serial.println(errorCode);
  }
  else {
    Serial.print(msg);
  }
}

const char* messages[] = {
  "Hello",
  "World",
  "Do you copy?",
  "Over"
};
int msgCount = 4;

unsigned long lastCheckMillis = 0;
unsigned long tmNow;
int counter = 0;
int msgNdx = 0;
void loop() {
  tmNow = millis();
  if(tmNow- lastCheckMillis < 2000) {
    return;
  }

  msgNdx = (counter++) % msgCount;
  Serial.print(tmNow);Serial.print(' ');Serial.println(msgNdx);
  sendMessage(messages[msgNdx]);

  lastCheckMillis = tmNow;
}
