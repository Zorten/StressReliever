#include <LedControl.h>
#include "Levels.h"
#include <LiquidCrystal.h>

///======VARIABLES======//

//variables for lcd init
const int rs = 13, en = 5, d4 = 4, d5 = 3, d6 = 2, d7 = 1;
//Set up LCD
  LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//buttons
const int up = 9;
const int right = 8;
const int down = 7;

//buzzer
const int buzzer = 6;

//distinct frequencies to be played by buzzer
const double F4_freq = 349.23;
const double G4_freq = 392.00;
const double A4_freq = 440.00;
const double B4_freq = 493.88;
const double C5_freq = 523.25;

//======STATE MACHINE FUNCTIONS======//
enum Breathe_states { Breathe_init, Breathe_waitPress, Breathe_waitRelease, Breathe_inhale, Breathe_hold, Breathe_exhale} Breathe_state;
void TickFct_Breathe(){
  switch(Breathe_state) { // Transitions
  
     case Breathe_init:// Initial transition
        Breathe_state = Breathe_waitPress;
        lcd.clear();
        lc.clearDisplay(0);
        break;
        
     case Breathe_waitPress: // - wait for user to press button
        if (digitalRead(right) == LOW ){
          Breathe_state = Breathe_waitPress;  
        }
        else if (digitalRead(right) == HIGH){
          Breathe_state = Breathe_waitRelease; 
        }
        else{
          Breathe_state = Breathe_waitPress;
        }
        break;
        
     case Breathe_waitRelease: //wait for user to release button
        if (digitalRead(right) == HIGH){
          Breathe_state = Breathe_waitRelease;  
        }
        else{
          Breathe_state = Breathe_inhale;
        }
        break;

     case Breathe_inhale:
        Breathe_state = Breathe_hold; 
        break;

     case Breathe_hold: 
        Breathe_state = Breathe_exhale;
        break;

     case Breathe_exhale:
        Breathe_state = Breathe_init;
        
     default:
        Breathe_state = Breathe_init;
   } // Transitions

  switch(Breathe_state) { // state actions
    
     case Breathe_inhale:
        lcd.print("INHALE");
        lcd.setCursor(0, 1);
        lcd.print("6");
        lc.setIntensity(0,0);
        allOn();
        delay(1000);
        lcd.clear();
        lcd.print("INHALE");
        lcd.setCursor(0, 1);
        lcd.print("5");
        lc.setIntensity(0, 1);
        delay(1000);
        lcd.clear();
        lcd.print("INHALE");
        lcd.setCursor(0, 1);
        lcd.print("4");
        lc.setIntensity(0, 2);
        delay(1000);
        lcd.clear();
        lcd.print("INHALE");
        lcd.setCursor(0, 1);
        lcd.print("3");
        lc.setIntensity(0, 3);
        delay(1000);
        lcd.clear();
        lcd.print("INHALE");
        lcd.setCursor(0, 1);
        lcd.print("2");
        delay(1000);
        lc.setIntensity(0, 4);
        lcd.clear();
        lcd.print("INHALE");
        lcd.setCursor(0, 1);
        lcd.print("1");
        delay(1000);
        lcd.clear();
        break;

     case Breathe_hold:
        lcd.print("HOLD");
        delay(4000);
        lcd.clear();
        break;

     case Breathe_exhale:
        lcd.print("EXHALE");
        lc.setIntensity(0,4);
        allOn();
        delay(1000);
        lc.setIntensity(0, 3);
        delay(1000);
        lc.setIntensity(0, 2);
        delay(1000);
        lc.setIntensity(0, 1);
        delay(1000);
        lc.setIntensity(0, 0);
        delay(2000);
        lcd.clear();
        break;        
        
     default:
        break;
  } //state actions
}
//======STATE MACHINE FUNCTIONS END======//

void setup() {
  // put your setup code here, to run once:
  
  lcd.begin(16, 2);

  //Set up LED Matrix
  lc.shutdown(0, false);
  lc.setIntensity(0, 0);
  lc.clearDisplay(0);

  // Set Buttons as INPUT
  pinMode(up, INPUT);
  pinMode(right, INPUT);
  pinMode(down, INPUT);

  // Set buzzer as output
  pinMode(buzzer, OUTPUT);

  //set initial states for SMs
  Breathe_state = Breathe_init;
}

void loop() {
  // put your main code here, to run repeatedly:
  TickFct_Breathe();
}
