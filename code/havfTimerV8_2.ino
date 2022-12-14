//Version 8.2 Final

int pinA = 6;
int pinB = 10;
int pinC = A4;
int pinD = 0;
int pinE = 1;
int pinF = 7;
int pinG = A3;
int pinH = A5;

int d1 = 5;
int d2 = A0;
int d3 = A1;
int d4 = A2;

int mic = 2;
int button = 3;

int led = 4;

int i = 1;
int x = 1;

unsigned long startTime = 0;
unsigned long stopTime = 0;

long previousMillis = 0;
long previousMillis2 = 0;

unsigned long holdTime;

boolean micFlag = false;
boolean buttonFlag = true;
boolean stopFlag = false;

unsigned long endTime = 0;

void setup() {
  attachInterrupt(digitalPinToInterrupt(button), buttonInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(mic), micInterrupt, CHANGE);

  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinH, OUTPUT);

  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);

  pinMode(led, OUTPUT);

  zeroDisplay();

}

void loop() {
  if (buttonFlag) {
    digitalWrite(led, LOW);
    if (!digitalRead(button)) {
      if (millis() - holdTime >= 1000) {
        startCountdown(5);
        buttonFlag = false;
      }
    }

    zeroDisplay();

  }

  else if (micFlag) {
    multiplex((millis() - startTime), 2);
  }

  else if (stopFlag) {
    multiplex(( endTime ), 2 );
  }
}





// Button interrupt function
void buttonInterrupt() {
  buttonFlag = true;
  stopFlag = false;
  holdTime = millis();
}




// Microphone interrupt function

void micInterrupt() {
  if (!micFlag && !stopFlag) {
    if ( millis() - holdTime > 1000 ) {
      startTime = millis();
      micFlag = true;
      buttonFlag = false;
    }
  }

  if (micFlag && (millis() - startTime >= 500)) {
    stopTime = millis();
    micFlag = false;
    stopFlag = true;
    endTime = stopTime - startTime;
  }
}




// Fungerar inte
/*
  bool longButtonPress() {
  if (!digitalRead(button)) {
    if ( millis() - holdTime >= 1000 ) {
      digitalWrite(led, HIGH);
      return true;
    } else {
      return false;
    }
  }
  }
*/


void startCountdown(int seconds) {
  digitalWrite(d1, LOW);
  digitalWrite(d2, LOW);
  digitalWrite(d3, LOW);
  digitalWrite(d4, LOW);

  int n = (seconds + 1);
  while (n > 0) {
    unsigned long currentMillis2 = millis();
    if (currentMillis2 - previousMillis2 >= 1000) {
      previousMillis2 = currentMillis2;
      n--;
    }
    renderDigit(n);
  }
  startTime = millis();
  micFlag = true;
}




// Multiplexes four displays
void multiplex(unsigned long number, int framerate) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= framerate) {
    previousMillis = currentMillis;
    if (x > 4) {
      x = 1;
    }
    else {
      int lengthCount = 0;
      unsigned long divnumber = number;

      while (divnumber != 0) {
        divnumber /= 10;
        lengthCount++;
      }
      int one = number / pow(10, (lengthCount - 1) );
      int two = (number / pow(10, (lengthCount - 2) )) - (one * 10);
      int three = (number / pow(10, (lengthCount - 3) )) - ((one * 100) + (two * 10));
      int four = (number / pow(10, (lengthCount - 4) )) - ((one * 1000) + (two * 100) + (three * 10));

      segmentX(x);

      if (x == 1) {
        renderDigit(one);
        // if millis > 1000
        if (micFlag) {
          if (millis() - startTime > 1000 && millis() - startTime < 10000) {
            digitalWrite(pinH, HIGH);
          } else {
            digitalWrite(pinH, LOW);
          }
        } else if (stopFlag) {
          if (endTime > 1000 && endTime < 10000) {
            digitalWrite(pinH, HIGH);
          } else {
            digitalWrite(pinH, LOW);
          }
        }
      }
      else if (x == 2) {
        renderDigit(two);
        if (micFlag) {
          if (millis() - startTime > 10000 && millis() - startTime < 100000) {
            digitalWrite(pinH, HIGH);
          } else {
            digitalWrite(pinH, LOW);
          }

        } else if (stopFlag) {
          if (endTime > 10000 && endTime < 100000) {
            digitalWrite(pinH, HIGH);
          } else {
            digitalWrite(pinH, LOW);
          }
        }
      }
      else if (x == 3) {
        renderDigit(three);
        if (micFlag) {
          if (millis() - startTime > 100000 && millis() - startTime < 1000000 && !stopFlag) {
            digitalWrite(pinH, HIGH);
          } else {
            digitalWrite(pinH, LOW);
          }
        } else if (stopFlag) {
          if (endTime > 100000 && endTime < 1000000) {
            digitalWrite(pinH, HIGH);
          } else {
            digitalWrite(pinH, LOW);
          }
        }
      }
      else if (x == 4) {
        renderDigit(four);
        digitalWrite(pinH, LOW);
      }
      x++;
    }
  }
}




// Function to activate targit display, 1 is the most left display
void segmentX(int segment) {
  if (segment == 1) {
    digitalWrite(d1, LOW);
    digitalWrite(d2, HIGH);
    digitalWrite(d3, HIGH);
    digitalWrite(d4, HIGH);
  }
  else if (segment == 2) {
    digitalWrite(d1, HIGH);
    digitalWrite(d2, LOW);
    digitalWrite(d3, HIGH);
    digitalWrite(d4, HIGH);
  }
  else if (segment == 3) {
    digitalWrite(d1, HIGH);
    digitalWrite(d2, HIGH);
    digitalWrite(d3, LOW);
    digitalWrite(d4, HIGH);
  }
  else if (segment == 4) {
    digitalWrite(d1, HIGH);
    digitalWrite(d2, HIGH);
    digitalWrite(d3, HIGH);
    digitalWrite(d4, LOW);
  }
}


// Sets all displays to zero
void zeroDisplay() {
  digitalWrite(d1, LOW);
  digitalWrite(d2, LOW);
  digitalWrite(d3, LOW);
  digitalWrite(d4, LOW);
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, HIGH);
  digitalWrite(pinC, HIGH);
  digitalWrite(pinD, HIGH);
  digitalWrite(pinE, HIGH);
  digitalWrite(pinF, HIGH);
  digitalWrite(pinG, LOW);
  digitalWrite(pinH, LOW);
}

// DigitMapping
void renderDigit(int x) {
  //0
  if (x == 0) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, LOW);
  }
  //1
  else if (x == 1) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, LOW);
  }
  //2
  else if (x == 2) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, HIGH);
  }
  //3
  else if (x == 3) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, HIGH);
  }

  //4
  else if (x == 4) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
  }
  //5
  else if (x == 5) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
  }
  //6
  else if (x == 6) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
  }
  //7
  else if (x == 7) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, LOW);
  }
  //8
  else if (x == 8) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
  }
  //9
  else if (x == 9) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
  }
}
