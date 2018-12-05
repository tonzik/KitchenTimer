/*
 * 
 * Kitchen Timer v0.1
 * by Tonza
 * 
 */

// ------------------------------------------------------------------
// VAADITUT KIRJASTOT / REQUIRED LIBRARIES
// ------------------------------------------------------------------

#include <libSeg47.h>
#include <Button.h>

// ------------------------------------------------------------------
// ALKUMÄÄRITYKSET
// ------------------------------------------------------------------

// 4-osaisen segmenttinäytön asetukset / 4-digit 7-segment display pins
libSeg47 segdisp(9,8, 7, 6, 13, 12, 11, 10, 5, 5);

// Pulssi-enkooderin asetukset / Rotary encoder pins
#define rotCLK 2 // CLK
#define rotDT 3 // DT
Button rotBTN (4, 2000); // BTN

// Muut asetukset / Other settings
int dtState;
int clkState;
int dtLastState;

int timeset = 0;
int counter = 0;
int timerMsg = 0;
int totalSeconds = 0;
int minutes = 0;
int seconds = 0;
int countdown = 0;

String stringOne, stringTwo, stringThree, stringZero;

int period = 1000;
unsigned long time_now = 0;

// ------------------------------------------------------------------
// SETUP
// ------------------------------------------------------------------

void setup() {
  // put your setup code here, to run once:

  pinMode(rotCLK, INPUT); // Rotary encoder CLK
  pinMode(rotDT, INPUT); // Rotary encoder DT
  Serial.begin(9600);
  Serial.println("Program starts");
  
}

// ------------------------------------------------------------------
// AJASTIMEN ASETUS / SET TIMER
// ------------------------------------------------------------------

void setKitchenTimer() {
  segdisp.showNro(timeset);
  dtState = digitalRead(rotDT);
  clkState = digitalRead(rotCLK);
  
  if ((dtState != dtLastState)&&(dtState==LOW)) {
    if (clkState == LOW) { 
      counter = counter + 30;
    } 
    else {
      counter = counter - 30;
    }

    if (counter < 0) {
      counter = 0;
    }

    timeset = calcSecMin(counter);
  }
  dtLastState = dtState;

  if (rotBTN.pressed()) {
    Serial.println("ajastin painettu");
    totalSeconds = counter + 1;
    timerMsg = 2;
  }
}

// ------------------------------------------------------------------
// MUUTETAAN AIKA MINUUTEIKSI JA SEKUNNEIKSI / CALCULATE TOTAL SECONDS TO MINUTES AND SECONDS
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
// AJASTIMEN DEMO-MOODI / TIMER DEMO
// ------------------------------------------------------------------

void timerDemo() {
  int DSP = 1234;
  segdisp.showNro(DSP);
}

// ------------------------------------------------------------------
// AJASTIN PÄÄLLÄ / TIMER ON
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
  countdown = stringThree.toInt();

  if (totalSeconds == 0) {
    segdisp.showNro(0);
  }

  if (totalSeconds == -5) {
    timerMsg = 0; 
  }

  while(millis() < time_now + period) {
    //wait approx [period] ms
    segdisp.showNro(countdown);
    //Serial.println(laskenta);
  }
}

// ------------------------------------------------------------------
// OHJELMAN PÄÄLUUPPI / MAIN LOOP
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
  if (rotBTN.pressed()) {
    timerMsg = 1;
  }
}
