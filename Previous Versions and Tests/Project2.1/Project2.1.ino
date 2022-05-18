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

//to keep track of button presses 
int rightPress = 0;
int upPress = 0;
int downPress = 0;

//Array of phrases
const int SIZE = 8;
String phrases[SIZE] = {"And still,","I rise", "Fall 7 times,", "Stand up 8", "It is ok", "To not be ok", "You are your", "Best thing"};

//Index to traverse array
int PhraseIndex = 0;

//distinct frequencies to be played by buzzer
const double F4_freq = 349.23;
const double G4_freq = 392.00;
const double A4_freq = 440.00;
const double B4_freq = 493.88;
const double C5_freq = 523.25;

//======STATE MACHINE FUNCTIONS======//



//This state function executes the breathing
//exercise option
enum Breathe_states { Breathe_init, Breathe_waitPress, Breathe_waitRelease, Breathe_inhale, Breathe_hold, Breathe_exhale} Breathe_state;
void TickFct_Breathe(){
  switch(Breathe_state) { // Transitions
  
     case Breathe_init:// Initial transition
        Breathe_state = Breathe_waitPress;
        lc.clearDisplay(0);
        lcd.clear();
        lcd.print("Breathing");
        lcd.setCursor(0, 1);
        lcd.print("Exercise");
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
          lcd.clear();
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
        lcd.print("5");
        lc.setIntensity(0,0);
        allOn();
        delay(1000);
        lcd.clear();
        lcd.print("INHALE");
        lcd.setCursor(0, 1);
        lcd.print("4");
        lc.setIntensity(0, 1);
        delay(1000);
        lcd.clear();
        lcd.print("INHALE");
        lcd.setCursor(0, 1);
        lcd.print("3");
        lc.setIntensity(0, 2);
        delay(1000);
        lcd.clear();
        lcd.print("INHALE");
        lcd.setCursor(0, 1);
        lcd.print("2");
        lc.setIntensity(0, 3);
        delay(1000);
        lcd.clear();
        lcd.print("INHALE");
        lcd.setCursor(0, 1);
        lcd.print("1");
        delay(1000);
        lc.setIntensity(0, 4);
        lcd.clear();
        break;

     case Breathe_hold:
        lcd.print("HOLD");
        lcd.setCursor(0, 1);
        lcd.print("4");
        delay(1000);
        lcd.clear();
        lcd.print("HOLD");
        lcd.setCursor(0, 1);
        lcd.print("3");
        delay(1000);
        lcd.clear();
        lcd.print("HOLD");
        lcd.setCursor(0, 1);
        lcd.print("2");
        delay(1000);
        lcd.clear();
        lcd.print("HOLD");
        lcd.setCursor(0, 1);
        lcd.print("1");
        delay(1000);
        lcd.clear();
        break;

     case Breathe_exhale:
        lcd.print("EXHALE");
        lcd.setCursor(0, 1);
        lcd.print("5");
        lc.setIntensity(0,4);
        allOn();
        delay(1000);
        lc.setIntensity(0, 3);
        lcd.clear();
        lcd.print("EXHALE");
        lcd.setCursor(0, 1);
        lcd.print("4");
        delay(1000);
        lc.setIntensity(0, 2);
        lcd.clear();
        lcd.print("EXHALE");
        lcd.setCursor(0, 1);
        lcd.print("3");
        delay(1000);
        lc.setIntensity(0, 1);
        lcd.clear();
        lcd.print("EXHALE");
        lcd.setCursor(0, 1);
        lcd.print("2");
        delay(1000);
        lc.setIntensity(0, 0);
        lcd.clear();
        lcd.print("EXHALE");
        lcd.setCursor(0, 1);
        lcd.print("1");
        delay(1000);
        lcd.clear();
        break;        
        
     default:
        break;
  } //state actions
}

//This function executes the motivational phrases option
enum Phrase_states { Phrase_init, Phrase_waitPress, Phrase_waitRelease, Phrase_display, Phrase_waitPress2, Phrase_waitRelease2, Phrase_next, Phrase_previous} Phrase_state;
void TickFct_Phrases(){
  switch(Phrase_state) { // Transitions
  
     case Phrase_init:// Initial transition
        Phrase_state = Phrase_waitPress;
        lc.clearDisplay(0);
        lcd.clear();
        lcd.print("Motivational");
        lcd.setCursor(0, 1);
        lcd.print("Phrases");
        PhraseIndex = 0;
        break;
        
     case Phrase_waitPress: // - wait for user to press button
        if (digitalRead(right) == LOW ){
          Phrase_state = Phrase_waitPress;  
        }
        else if (digitalRead(right) == HIGH){
          Phrase_state = Phrase_waitRelease; 
        }
        else{
          Phrase_state = Phrase_waitPress;
        }
        break;
        
     case Phrase_waitRelease: //wait for user to release button
        if (digitalRead(right) == HIGH){
          Phrase_state = Phrase_waitRelease;  
        }
        else{
          Phrase_state = Phrase_display;
          lcd.clear();
        }
        break;

     case Phrase_display:
        Phrase_state = Phrase_waitPress2;

     case Phrase_waitPress2:
        if (digitalRead(right) == LOW && digitalRead(down) == LOW && digitalRead(up) == LOW){
          Phrase_state = Phrase_waitPress2;  
        }
        else if (digitalRead(right) == HIGH){
          Serial.print("Right press\n");
          rightPress = 1;
          Phrase_state = Phrase_waitRelease2; 
        }
        else if (digitalRead(up) == HIGH){
          Serial.print("Up press\n");
          upPress = 1;
          Phrase_state = Phrase_waitRelease2; 
        }
        else if (digitalRead(down) == HIGH){
          Serial.print("Down press\n");
          downPress = 1;
          Phrase_state = Phrase_waitRelease2; 
        }
        else{
          Phrase_state = Phrase_waitPress;
        }
        break;

     case Phrase_waitRelease2:
        if (digitalRead(right) == HIGH || digitalRead(down) == HIGH || digitalRead(up) == HIGH){
          Phrase_state = Phrase_waitRelease2;  
        }
        else{
          if (upPress){
            Phrase_state = Phrase_next; 
            upPress = 0; 
          }
          else if (downPress){
            Phrase_state = Phrase_previous; 
            downPress = 0; 
          }
          else if (rightPress){
            Phrase_state = Phrase_init; 
            rightPress = 0; 
          }
          else{
            Phrase_state = Phrase_waitPress2;  
          }  
        }
        break;

     case Phrase_next:
        Phrase_state = Phrase_display;
        lcd.clear();
        break;

     case Phrase_previous:
        Phrase_state = Phrase_display;
        lcd.clear();
        break;
        
     default:
        Phrase_state = Phrase_init;
   } // Transitions

  switch(Phrase_state) { // state actions

     case Phrase_display:
        lcd.print(phrases[PhraseIndex]);
        lcd.setCursor(0, 1);
        lcd.print(phrases[PhraseIndex+1]);
        break;

     case Phrase_next:
        if (PhraseIndex < SIZE - 2){
          Serial.print("Index up\n");
          PhraseIndex+= 2;  
        }
        else{
          PhraseIndex = SIZE - 2;  
        }
        break;

     case Phrase_previous:
        if (PhraseIndex > 0){
          Serial.print("Index down\n");
          PhraseIndex-= 2;  
        }
        else{
          PhraseIndex = 0;  
        }
        break;
        
     default:
        break;
  } //state actions
}

//======STATE MACHINE FUNCTIONS END======//


//======SETUP======//
void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  
  lcd.begin(16, 2);
  lcd.clear();

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
  Phrase_state = Phrase_init;
}
//======END SETUP======//

void loop() {
  // put your main code here, to run repeatedly:
  TickFct_Phrases();
}
