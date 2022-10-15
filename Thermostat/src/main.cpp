#include <Arduino.h>
#include "pins.h"

#define LED D5 //Define blinking LED pin

#define MAX_INPUT_LEN 31
char inputBuffer[MAX_INPUT_LEN + 1];

void setup()
{
  Serial.begin(115200);		 // Start the Serial communication to send messages to the computer
  delay(100);

  pinMode(D5, OUTPUT);

  analogWriteFreq(100);

  //tone(D5, 19000);
}

void inputOverflowNotice() {
  Serial.println("\nInput overflow.");
}

int blinks = 0;

void processInput() {
  char cmd = inputBuffer[0];
  switch(cmd) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '9':
      blinks = cmd - '0';
  }
}

bool readInput() {
  static int ndx = 0;

  while (Serial.available() > 0) {
    char receivedChar = Serial.read();

    if (receivedChar == '\r') {
      // ignore
    }
    else if (receivedChar == 0x08) {
      // backspace clears input
      ndx = 0;
      Serial.println("\nInput canceled.");
    }
    else if (receivedChar == '\n') {
      // end of input
      inputBuffer[ndx] = '\0'; // end of string
      ndx = 0;
      Serial.print(receivedChar);
      return true;
    }
    else {
      if (ndx < MAX_INPUT_LEN) {
        inputBuffer[ndx++] = receivedChar;
        Serial.print(receivedChar);
      }
      else {
        inputOverflowNotice();
      }
    }
  }

  return false;
}

void blink(int count) {
  analogWrite(LED, 0);
  delay(2000);

  for(int n = 0; n < count; n++) {
    analogWrite(LED, 45);
    delay(400);
    analogWrite(LED, 0);
    if(n < count - 1) {
      delay(800);
    }
  }
}

unsigned long lastCheckMillis = 0;
void loop() {

  if(readInput()) {
    processInput();
    lastCheckMillis = 0; // Make it check the lights
  }

  switch(blinks) {
    case 0:
      analogWrite(LED, 0);
      break;

    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      blink(blinks);
      break;

    case 9:
      analogWrite(LED, 255);
      break;
  }
  // analogWrite(LED, 255);
  // delay(1000);

  // analogWrite(LED, 120);
  // delay(1000);

  // analogWrite(LED, 60);
  // delay(1000);

  // analogWrite(LED, 30);
  // delay(1000);

  // analogWrite(LED, 15);
  // delay(1000);

  // analogWrite(LED, 0);
  // delay(2000);
}
