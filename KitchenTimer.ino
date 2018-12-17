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

// Ääniasetukset | Sound settings
libRTTTL_Tunes player(A0);

const char s0[] PROGMEM = "JingleBells:d=8,o=5,b=112:32p,a,a,4a,a,a,4a,a,c6,f.,16g,2a,a#,a#,a#.,16a#,a#,a,a.,16a,a,g,g,a,4g,4c6";
const char s1[] PROGMEM = "Top Gun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#";
const char s2[] PROGMEM = "LeisureSuit:d=16,o=6,b=56:f.5,f#.5,g.5,g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#.,a#5,32c#,a5,a#.5,c#.,32a5,a#5,32c#,d#,8e,c#.,f.,f.,f.,f.,f,32e,d#,8d,a#.5,e,32f,e,32f,c#,d#.,c#";
const char s3[] PROGMEM = "LaBamba:d=32,o=6,b=45:g#5,a#5,c,16c#,f,c#,16f#,a#,16g#,g#5,c,d#,f#,f#,f,d#,16c#,f,c#,16f#,a#,16g#,f#,f#,f#,16f#,f#,16f,4c#,f#,f#,f#,16f#,f#,16f,16c#,c#,c#,16c#,16d#,16c,16d#,f#,f,d#,16f,8c#";
const char s4[] PROGMEM = "BackFuture:d=4,o=5,b=160:32p,p,8c.,16p,g,16p,16c.6,16p,a#.,16p,16a,16p,16g,16p,8a,16p,8g,16p,8f,16p,1g.,1p,g.,16p,c.,16p,2f#.,16p,32g.,32p,32a.,32p,8g,32p,8e,32p,8c,32p,f#,16p,32g.,32p,32a.,32p,8g.,32p,8d.,32p,8g.,32p,8d.6,32p,d6,16p,c#6,16p,32b.,32p,32c#.6,32p,2d6";
const char s5[] PROGMEM = "AxelF:d=4,o=5,b=125:32p,8g,8p,16a#.,8p,16g,16p,16g,8c6,8g,8f,8g,8p,16d.6,8p,16g,16p,16g,8d#6,8d6,8a#,8g,8d6,8g6,16g,16f,16p,16f,8d,8a#,2g,p";
const char s6[] PROGMEM = "Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
const char s7[] PROGMEM = "KnightRider:d=4,o=5,b=63:32p,16e,32f,32e,8b,16e6,32f6,32e6,8b,16e,32f,32e,16b,16e6,d6,8p,p,16e,32f,32e,8b,16e6,32f6,32e6,8b,16e,32f,32e,16b,16e6,f6,p";
const char s8[] PROGMEM = "Seinfeld:d=16,o=5,b=56:32p,c#.,32c#,f,g#,8b,f.,f#.,8f#,e,8d#,d.,c#,32c#,c#,d#,d#,8e,f.,8f#";
const char s9[] PROGMEM = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";
const char s10[] PROGMEM = "Kombat:d=4,o=6,b=35:32a#5,32a#5,32c#6,32a#5,32d#6,32a#5,32f6,32d#6,32c#6,32c#6,32f6,32c#6,32g#6,32c#6,32f6,32c#6,32g#5,32g#5,32c6,32g#5,32c#6,32g#5,32d#6,32c#6,32f#5,32f#5,32a#5,32f#5,32c#6,32f#5,32c#6,32c6,32a#5,32a#5,32c#6,32a#5,32d#6,32a#5,32f6,32d#6,32c#6,32c#6,32f6,32c#6,32g#6,32c#6,32f6,32c#6,32g#5,32g#5,32c6,32g#5,32c#6,32g#5,32d#6,32c#6,32f#5,32f#5,32a#5,32f#5,32c#6,32f#5,32c#6,32c6,";
const char s11[] PROGMEM = "BetterOff:d=8,o=5,b=140:32p,c6,p,c6,a,p,c6,p,c6,p,b,p,g,g6,16p,g6,16p,e6,c6,p,c6,a,p,c6,p,c6,p,b,p,g,f6,16p,f6,16p,e6,c6,p,c6,a,p,c6,p,c6,p,b,p,g,g6,16p,g6,16p,e6,c6,p,c6,a,p,c6,p,c6,p,b,p,g,f6,16p,f6,16p,e6";
const char s12[] PROGMEM = "Phantom:d=4,o=5,b=140:c,f,c,d#.,8c#,2c#,a#4,d#,8a#4,2c,c,f,c,d#.,8c#,2c#,a#4,d#.,8a#4,2c,p,c,f,g#,c.6,8a#,2a#,a#,d#.6,8a#,2c6,p,c6,2f.6,8d#6,8c#6,8c6,8a#,8g#,8g,8f,2e,c#,c#.,8c,2c";
const char s13[] PROGMEM = "Godfather:d=4,o=5,b=80:8g,8c6,8d#6,8d6,8c6,8d#6,8c6,8d6,c6,8g#,8a#,2g,8p,8g,8c6,8d#6,8d6,8c6,8d#6,8c6,8d6,c6,8g,8f#,2f,8p,8f,8g#,8b,2d6,8p,8f,8g#,8b,2c6,8p,8c,8d#,8a#,8g#,g,8a#,8g#,8g#,8g,8g,8b4,2c";
const char s14[] PROGMEM = "Macarena:d=4,o=5,b=180:32p,f,8f,8f,f,8f,8f,8f,8f,8f,8f,8f,8a,8c,8c,f,8f,8f,f,8f,8f,8f,8f,8f,8f,8d,8c,p,f,8f,8f,f,8f,8f,8f,8f,8f,8f,8f,8a,p,2c.6,a,8c6,8a,8f,p,2p";
const char s15[] PROGMEM = "Smoke:d=4,o=5,b=125:32p,c,d#,f.,c,d#,8f#,f,p,c,d#,f.,d#,c,2p,8p,c,d#,f.,c,d#,8f#,f,p,c,d#,f.,d#,c,p";
const char s16[] PROGMEM = "ComeAs:d=4,o=6,b=50:32f.5,32f#.5,16g.5,32a#.5,32g.5,32a#.5,32g.5,32g.5,32f#.5,32f.5,32c.,32f.5,16f.5,32c.,32f.5,32f#.5,16g.5,32a#.5,32g.5,32a#.5,32g.5,32g.5,32f#.5,32f.5,32c.,32f.5,16f.5,32c.";

char buffer[512]; // make sure this is large enough for the largest string it must hold

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
    
    strcpy_P(buffer, (char*)s0);    // Copy song from progmem to buffer
    player.play_RTTTL(buffer);
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
