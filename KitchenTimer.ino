/*
 * 
 * Kitchen Timer v0.1
 * by Tonza
 * 
 */

// ------------------------------------------------------------------
// REQUIRED LIBRARIES
// ------------------------------------------------------------------

#include <libSeg47.h>
#include <Button.h>

// ------------------------------------------------------------------
// SETTINGS
// ------------------------------------------------------------------

// 4-digit 7-segment display pins
libSeg47 segdisp(9,8, 7, 6, 13, 12, 11, 10, 5, 5);

// Rotary encoder pins
#define outputA 2 // CLK
#define outputB 3 // DT
Button outputC (4, 2000); // BTN

// other settings
int aState;
int bState;
int aLastState;

int timeset = 0;
int counter = 0;
int timerMsg = 0;
int totalSeconds = 0;
int minutes = 0;
int seconds = 0;
int laskenta = 0;

String stringOne, stringTwo, stringThree, stringZero;

int period = 1000;
unsigned long time_now = 0;

// ------------------------------------------------------------------
// SETUP
// ------------------------------------------------------------------

void setup() {
  // put your setup code here, to run once:

  pinMode(outputA, INPUT); // Rotary encoder CLK
  pinMode(outputB, INPUT); // Rotary encoder DT
  Serial.begin(9600);
  Serial.println("Program starts");
  
}

// ------------------------------------------------------------------
// SET TIMER
// ------------------------------------------------------------------

void setKitchenTimer() {
  segdisp.showNro(timeset);
  aState = digitalRead(outputB);
  bState = digitalRead(outputA);
  
  if ((aState != aLastState)&&(aState==LOW)) {
    if (bState == LOW) { 
      counter = counter + 15;
    } 
    else {
      counter = counter - 15;
    }

    if (counter < 0) {
      counter = 0;
    }

    timeset = calcSecMin(counter);
  }
  aLastState = aState;

  if (outputC.pressed()) {
    Serial.println("ajastin painettu");
    totalSeconds = counter + 1;
    timerMsg = 2;
  }
}

// ------------------------------------------------------------------
// CALCULATE TOTAL SECONDS TO MINUTES AND SECONDS
// ------------------------------------------------------------------

int calcSecMin(int totalSeconds) {
  String stringSec, stringMin, stringTot, stringZero;
  stringZero = "0";
  stringSec = "0";
  stringMin = "0";
  stringTot = "0";
  int seconds;
  int minutes;
  int calc;

  minutes = totalSeconds / 60;
  seconds = totalSeconds % 60;

  if (seconds < 10) {
    stringSec = stringZero + seconds;
  } else {
    stringSec = seconds;
  }

  if (minutes < 10) {
    stringMin = stringZero + minutes;
  } else {
    stringMin = minutes;
  }

  stringTot = stringMin + stringSec;
  calc = stringTot.toInt();
  Serial.println(stringTot);
  return calc;
}

// ------------------------------------------------------------------
// TIMER DEMO
// ------------------------------------------------------------------

void timerDemo() {
  int DSP = 1234;
  segdisp.showNro(DSP);
}

// ------------------------------------------------------------------
// TIMER ON
// ------------------------------------------------------------------

void timerOn() {
  time_now = millis();
  totalSeconds --;
  minutes = totalSeconds / 60;
  seconds = totalSeconds % 60;
  stringZero = "0";
  stringOne = "";
  stringTwo = "";
  stringThree = "";
  
    if (seconds < 10) {
      stringOne = stringZero + seconds;
    }else{
      stringOne = seconds;
    }

    if (minutes < 10) {
      stringTwo = stringZero + minutes;
    }else{
      stringTwo = minutes;
    }
  
  stringThree = stringTwo + stringOne;
  laskenta = stringThree.toInt();

  if (totalSeconds == 0) {
    segdisp.showNro(0);
  }

  if (totalSeconds == -5) {
    timerMsg = 0; 
  }

  while(millis() < time_now + period) {
    //wait approx [period] ms
    segdisp.showNro(laskenta);
    //Serial.println(laskenta);
  }
}

// ------------------------------------------------------------------
// MAIN LOOP
// ------------------------------------------------------------------

void loop() {
  // put your main code here, to run repeatedly:

  if (timerMsg == 0) {
    timerDemo();
  }
  if (timerMsg == 1) {
    setKitchenTimer();
  }
  if (timerMsg == 2) {
    timerOn();
  }
  if (outputC.pressed()) {
    timerMsg = 1;
  }
}
