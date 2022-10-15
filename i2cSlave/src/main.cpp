#include <Arduino.h>
#include <Wire.h>

void receiveEvent(int byteCount) {
  Serial.print(millis());
  Serial.print(' ');
  Serial.println(byteCount);
  for(int n = 0; n < byteCount; n++) {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  delay(500);

  pinMode(LED_BUILTIN, OUTPUT);

  Wire.begin(9);

  //Wire.setClock(2000); // 10KHz, go slow
  Wire.onReceive(receiveEvent);
}

unsigned long lastLoopRun = 0;
void loop() {
  if(millis() - lastLoopRun >= 2000) {
    Serial.println("...");
    lastLoopRun = millis();
  }

  yield();
}
