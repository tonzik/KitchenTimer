/*
 * 
 * Kitchen Timer
 * by Tonza
 * 
 */

// ------------------------------------------------------------------
// VAADITUT KIRJASTOT | REQUIRED LIBRARIES
// ------------------------------------------------------------------

// Segmenttinäytön kirjasto | Library for 7-segment display
// https://github.com/tonzik/libSeg47
#include <libSeg47.h>

// RTTTL äänien kirjasto | Library for RTTTL Tones
// https://github.com/tonzik/libRTTTL_Tunes
#include <libRTTTL_Tunes.h>

// Painikkeelle kirjasto | Library for Button function
// https://github.com/tonzik/libButton
#include <libButton.h>

// ------------------------------------------------------------------
// ALKUMÄÄRITYKSET | CONFIGURATION
// ------------------------------------------------------------------

// 4-osaisen segmenttinäytön asetukset | 4-digit 7-segment display settings
libSeg47 segdisp(9,8, 7, 6, 13, 12, 11, 10, 5, 5);

// Pulssi-enkooderin asetukset | Rotary encoder settings
#define rotCLK 2 // CLK
#define rotDT 3 // DT
Button rotBTN (4, 2000); // BTN

int dtState;
int clkState;
int dtLastState;

// Kauttimen asetukset | Buzzer settings
libRTTTL_Tunes player(A0);

// Muut asetukset // Other settings
int timeset = 0;
int counter = 0;
int timerMsg = 0;
int totalSeconds = 0;
int minutes = 0;
int seconds = 0;
int countdown = 0;
int pause = 0;

String stringOne, stringTwo, stringThree, stringZero;

int period = 1000; // Ajastimen päivitysväli 1s | Timer interval 1s
unsigned long time_now = 0;

// ------------------------------------------------------------------
// SETUP
// ------------------------------------------------------------------

void setup() {
  pinMode(rotCLK, INPUT); // Rotary encoder CLK
  pinMode(rotDT, INPUT); // Rotary encoder DT
}

// ------------------------------------------------------------------
// AJASTIMEN ASETUS | SET TIMER
// ------------------------------------------------------------------

void setKitchenTimer() {
  segdisp.showNro(timeset);
  dtState = digitalRead(rotDT);
  clkState = digitalRead(rotCLK);

  // Luetaan pulssi-enkooderin muutos | Read the pulse encoder change
  if ((dtState != dtLastState)&&(dtState==LOW)) {
    if (clkState == LOW) { 
      counter = counter + 15; // Myötäpäivään lisätään | Clockwise increases
    } 
    else {
      counter = counter - 15; // Vastapäivään vähennetään | Counterclockwise decreases
    }

    if (counter < 0) {
      counter = 0; // Ei sallita negatiivista lukua | Negative number is not allowed
    }

    timeset = calcSecMin(counter);
  }
  dtLastState = dtState;

  // Käynnistetään ajastin | Start the timer
  if (rotBTN.pressed()) {
    totalSeconds = counter + 1;
    timerMsg = 2;
  }
}

// ------------------------------------------------------------------
// MUUTETAAN AIKA MINUUTEIKSI JA SEKUNNEIKSI | CALCULATE TOTAL SECONDS TO MINUTES AND SECONDS
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

  // Jos luku on pienempi kuin 10, nolla lisätään | If the number is less than 10, zero is added
  // Esimerkiksi | For example 2.5 - > 02.05
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
  return calc;
}

// ------------------------------------------------------------------
// AJASTIMEN DEMO-MOODI | TIMER DEMO MODE
// ------------------------------------------------------------------

void timerDemo() {
  int DSP = 1234;
  segdisp.showNro(DSP);
}

// ------------------------------------------------------------------
// AJASTIN PÄÄLLÄ | TIMER ON
// ------------------------------------------------------------------

void timerOn() {
  time_now = millis();

  // Tarkistetaan onko paussi on/off | Check whether the pause is on / off
  // Paussi on päällä | Pause is on
  if (pause == 1) {
    totalSeconds = totalSeconds + 0; // Näytön lukema ei päivity | The screen will not update
  }
  // Paussi ei ole päällä | Pause is off
  else {
     totalSeconds --; //  Näytön lukema päivittyy | The screen is updated
  }
  
  minutes = totalSeconds / 60;
  seconds = totalSeconds % 60;
  stringZero = "0";
  stringOne = "";
  stringTwo = "";
  stringThree = "";

  // Jos luku on pienempi kuin 10, nolla lisätään | If the number is less than 10, zero is added
  // Esimerkiksi | For example 2.5 - > 02.05
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

  // Ajastus on päättynyt | The timer has run out
  if (totalSeconds == 0) {
    segdisp.showNro(0);
    player.playTuneNumber(1);
  }

  if (totalSeconds == -5) {
    timerMsg = 0; 
  }

  while(millis() < time_now + period) {
    // Päivitä näyttö 1s välein | Update the display every 1s
    segdisp.showNro(countdown);

    // Ajastimen paussi - on/off | Timer pause - on/off
    if (rotBTN.pressed() & totalSeconds > 0) {
      if (pause == 0) {
        pause = 1;
      }else{
        pause = 0;
      }
    }
  }
}

// ------------------------------------------------------------------
// OHJELMAN PÄÄLUUPPI | MAIN LOOP
// ------------------------------------------------------------------

void loop() {

  // timerMsg = 0 | Ajastimen demo-moodi  | Timer demo mode
  // timerMsg = 1 | Ajastimen asetus      | Set timer
  // timerMsg = 2 | Ajastin on päällä     | Timer is on

  if (timerMsg == 0) {
    timerDemo();
  }
  
  if (timerMsg == 1) {
    setKitchenTimer();
  }
  
  if (timerMsg == 2) {
    timerOn();
  }
  
  if (rotBTN.pressed() && timerMsg == 0) {
    timerMsg = 1;
  }
}
