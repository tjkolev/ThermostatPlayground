#include <Arduino.h>
#include <Wire.h>

void setup1() {
  Serial.begin(9600);
  delay(500);

  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop1() {
  // put your main code here, to run repeatedly:
  Serial.println("LED on");
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second

  Serial.println("LED off");
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
}

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


void setup2() {
  Serial.begin(9600);
  delay(500);

  pinMode(LED_BUILTIN, OUTPUT);

  Wire.begin(9);

  Wire.setClock(10000); // 10KHz, go slow
  Wire.onReceive(receiveEvent);
}

unsigned long lastLoopRun = 0;
void loop2() {
  if(millis() - lastLoopRun >= 2000) {
    Serial.println("...");
    lastLoopRun = millis();
  }

  yield();
}

void setup() {
  Serial.begin(9600);
  delay(500);

  pinMode(A1, INPUT);
}

int lastVal = -1;
int pwmThreshold = 60;

int pcount = 0;
unsigned long lastTickMs = 0;
unsigned long codePauseMs = 1800;
unsigned long blinkPauseMs = 500;
unsigned long detectionTimeMs = 30000;

unsigned long rangeBlinkGapMs[] = { 700, 900 };
unsigned long rangeCodeGapMs[] = { 1900, 2100};

int detectBlinkCode() {
  unsigned long tmNowMs = 0;
  unsigned long tmSpan = 0;
  unsigned long tmStartMs = millis();
  unsigned long tmLastSampleMs = 0;
  bool isCounting = false;
  int countStatus = 0x01;

  while(true) {
    tmNowMs = millis();
    if(tmNowMs - tmStartMs > detectionTimeMs) {
      return (-1 * countStatus);
    }

    if(tmNowMs - tmLastSampleMs < 2) {
      yield();
      continue; // too soon to sample
    }

    int aval = analogRead(A1);
    if(aval > pwmThreshold) {
      continue; // not interested in those
    }
    countStatus |= 0x02;

    tmSpan = tmNowMs - tmLastSampleMs;
    //Serial.print("Span: "); Serial.println(tmSpan);

    if(rangeCodeGapMs[0] <= tmSpan && tmSpan <= rangeCodeGapMs[1]) {
      if(isCounting) {
        // blink code done
        pcount++;
        Serial.print("Counted "); Serial.println(pcount);
        return pcount;
      }
      else {
        // first time detecting code gap. now we start counting.
        Serial.println("Start counting.");
        isCounting = true;
        pcount = 0;
        countStatus |= 0x04;
      }
    }
    else if(rangeBlinkGapMs[0] <= tmSpan && tmSpan <= rangeBlinkGapMs[1]) {
      // we have a blink
      pcount++;
      Serial.println('+');
      countStatus |= 0x08;
    }

    tmLastSampleMs = tmNowMs;
  }
}

int detectBlinkCode1() {
  unsigned long tmNowMs = 0;
  unsigned long tmSpan = 0;
  unsigned long tmStartMs = millis();
  while(millis() - tmStartMs <= detectionTimeMs) {
    int aval = analogRead(A1);
    if(aval > pwmThreshold) {
      continue;
    }

    tmNowMs = millis();
    tmSpan = tmNowMs - lastTickMs;
    if(tmSpan > codePauseMs) {
      // blink code done. Reset.
      Serial.print("Counted "); Serial.println(pcount);
      pcount = 1; // This starts the 1.
      lastTickMs = tmNowMs;
    }
    else if(tmSpan > blinkPauseMs) {
      // we have a blink
      pcount++;
      lastTickMs = tmNowMs;
      Serial.println('+');
    }
    else { //if(tmSpan <= codePauseMs) {
      // remember the last one
      //lastTickMs = tmNowMs;
      Serial.print('.');
    }
  }

  return pcount;
}

int code;
void loop() {
  if(millis() - lastLoopRun >= 15000) {
    code = detectBlinkCode();
    Serial.print("Detected "); Serial.println(code);
    lastLoopRun = millis();
  }

  yield();
}

void loop3() {
  if(millis() - lastLoopRun >= 1) {
    int aval = analogRead(A1);
    if(aval != lastVal) {
      Serial.print(millis()); Serial.print('\t');
      Serial.println(aval);
      lastVal = aval;
    }
    lastLoopRun = millis();
  }

  yield();
}