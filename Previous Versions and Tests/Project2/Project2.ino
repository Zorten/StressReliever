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

//flags to check button presses
int upPress = 0;
int rightPress = 0;
int downPress = 0;
int goUp = 0;
int goRight = 0;
int goDown = 0;

//buzzer
const int buzzer = 6;

//distinct frequencies to be played by buzzer
const double F4_freq = 349.23;
const double G4_freq = 392.00;
const double A4_freq = 440.00;
const double B4_freq = 493.88;
const double C5_freq = 523.25;

//======STATE MACHINE FUNCTIONS======//

//This state machine is meant as a helper function
//to capture button presses
enum Button_states {Button_waitPress, Button_waitRelease, Button_set} Button_state;
void TickFct_ButtonPress(){
  switch (Button_state){ //Transitions

    case Button_waitPress:
      if (digitalRead(right) == LOW && digitalRead(down) == LOW && digitalRead(up) == LOW){
          Serial.print("No button press detected\n");
          Button_state = Button_waitPress;  
        }
      else if (digitalRead(right) == HIGH){
          Serial.print("Right press\n");
          goRight = 1;
          Button_state = Button_waitRelease; 
        }
      else if (digitalRead(up) == HIGH){
          Serial.print("Up press\n");
          goUp = 1;
          Button_state = Button_waitRelease; 
        }
      else if (digitalRead(down) == HIGH){
          Serial.print("Down press\n");
          goDown = 1;
          Button_state = Button_waitRelease; 
        }
      else{
          Button_state = Button_waitPress;
      }
      break;

    case Button_waitRelease:
      if (digitalRead(right) == HIGH || digitalRead(down) == HIGH || digitalRead(up) == HIGH){
        Button_state = Button_waitRelease;  
      }
      else{
        Button_state = Button_set;  
      }
      break;

    case Button_set:
      Button_state = Button_waitPress;
      goUp = 0;
      goRight = 0;
      goDown = 0;
      break;

    default:
      Button_state = Button_waitPress;
  
  }

  switch (Button_state){ //Actions
    case Button_set:
      if (goUp){
        Serial.print("set up press\n");
        upPress = 1;
        goUp = 0; 
      }
      else if (goDown){
        Serial.print("set down press\n");
        downPress = 1;  
        goDown = 0;
      }
      else if (goRight){
        Serial.print("set right press\n");
        rightPress = 1;
        goRight = 0;  
      }
      break;

    default:
      break;
  }
  
}


//This state function executes the breathing
//exercise option
enum Breathe_states { Breathe_init, Breathe_wait, Breathe_inhale, Breathe_hold, Breathe_exhale} Breathe_state;
void TickFct_Breathe(){
  switch(Breathe_state) { // Transitions
  
     case Breathe_init:// Initial transition
        Breathe_state = Breathe_wait;
        lc.clearDisplay(0);
        lcd.clear();
        lcd.print("Breathing");
        lcd.setCursor(0, 1);
        lcd.print("Exercise");
        break;
        
     case Breathe_wait: // - wait for user to press button
        if (rightPress == 0){
          Breathe_state = Breathe_wait;  
        }
        else if (rightPress == 1){
          Serial.print("Transitioning to inhale\n");
          Breathe_state = Breathe_inhale;
          rightPress = 0;
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

     case Breathe_wait:
        //TickFct_ButtonPress();

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

////This function executes the motivational phrases option
//enum Phrase_states { Phrase_init, Phrase_waitPress, Phrase_waitRelease, Phrase_display, Phrase_waitPress2, Phrase_waitRelease2, Phrase_right} Phrase_state;
//void TickFct_Phrase(){
//  switch(Phrase_state) { // Transitions
//  
//     case Phrase_init:// Initial transition
//        Phrase_state = Phrase_waitPress;
//        lc.clearDisplay(0);
//        lcd.clear();
//        lcd.print("Breathing");
//        lcd.setCursor(0, 1);
//        lcd.print("Exercise");
//        break;
//        
//     case Phrase_waitPress: // - wait for user to press button
//        if (digitalRead(right) == LOW ){
//          Phrase_state = Phrase_waitPress;  
//        }
//        else if (digitalRead(right) == HIGH){
//          Phrase_state = Phrase_waitRelease; 
//        }
//        else{
//          Phrase_state = Phrase_waitPress;
//        }
//        break;
//        
//     case Phrase_waitRelease: //wait for user to release button
//        if (digitalRead(right) == HIGH){
//          Phrase_state = Phrase_waitRelease;  
//        }
//        else{
//          Phrase_state = Phrase_inhale;
//          lcd.clear();
//        }
//        break;
//
//     case Phrase_inhale:
//        Phrase_state = Phrase_hold; 
//        break;
//
//     case Phrase_hold: 
//        Phrase_state = Phrase_exhale;
//        break;
//
//     case Phrase_exhale:
//        Phrase_state = Phrase_init;
//        
//     default:
//        Phrase_state = Phrase_init;
//   } // Transitions
//
//  switch(Phrase_state) { // state actions
//
//     case Phrase_inhale:
//        
//        break;
//
//     case Phrase_hold:
//        
//        break;
//
//     case Phrase_exhale:
//        
//        break;        
//        
//     default:
//        break;
//  } //state actions
//}

//======STATE MACHINE FUNCTIONS END======//


//======SETUP======//
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
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
  Button_state = Button_waitPress;
}
//======END SETUP======//

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("=====ABOUT TO CALL BREATHE=====\n");
  TickFct_Breathe();
  Serial.print("=====FINISHED BREATHE=====\n");
}
