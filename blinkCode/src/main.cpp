#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  delay(500);

  pinMode(A1, INPUT);
}

int lastVal = -1;
int pwmThreshold = 60;

//unsigned long lastTickMs = 0;
//unsigned long codePauseMs = 1800;
//unsigned long blinkPauseMs = 500;
unsigned long detectionTimeMs = 10000;

unsigned long rangeBlinkGapMs[] = { 700, 900 };
unsigned long rangeCodeGapMs[] = { 1900, 2100};

int detectBlinkCode() {
  int pcount = 0;
  unsigned long tmNowMs = 0;
  unsigned long tmSpan = 0;
  unsigned long tmStartMs = millis();
  unsigned long tmLastSampleMs = 0;
  unsigned long sampleRateMs = 3;
  bool isCounting = false;
  int countStatus = 0x01;

  while(true) {
    tmNowMs = millis();
    if(tmNowMs - tmStartMs > detectionTimeMs) {
      return (-1 * countStatus);
    }

    if(tmNowMs - tmLastSampleMs < sampleRateMs) {
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

unsigned long lastLoopRun = 0;
void loop() {
  if(millis() - lastLoopRun >= 15000) {
    int code = detectBlinkCode();
    Serial.print(millis());Serial.print(" Detected "); Serial.println(code);
    lastLoopRun = millis();
  }

  yield();
}
