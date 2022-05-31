#include <LedControl.h>
#include "Levels.h"
#include <string.h>
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
const int PSIZE = 4;
char phrases[PSIZE][50] = {"And still, I rise", "Fall 7 times, stand up 8", "It is ok to not be ok", "You are your best thing"};
int phraseSize = 0;
char letter = ""; 

//Array of function names
const int FSIZE = 4;
String funcNames[FSIZE] = {"Breathing", "Exercise", "Motivational", "Phrases"};
int choice = 0;

//Indexes to traverse arrays
int PhraseIndex = 0;
int MenuIndex = 0;


//distinct frequencies to be played by buzzer
const double F4_freq = 349.23;
const double G4_freq = 392.00;
const double A4_freq = 440.00;
const double B4_freq = 493.88;
const double C5_freq = 523.25;

//======END VARIABLES======//


//======STATE MACHINE FUNCTIONS======//

//This state function executes the breathing
//exercise option
enum Breathe_states { Breathe_init, Breathe_waitPress, Breathe_waitRelease, Breathe_inhale, Breathe_hold, Breathe_exhale} Breathe_state;
void TickFct_Breathe(){
  switch(Breathe_state) { // Transitions
  
     case Breathe_init:// Initial transition
//        Breathe_state = Breathe_waitPress;
        Breathe_state = Breathe_inhale;
        lc.clearDisplay(0);
        lcd.clear();
//        lcd.print("Breathing");
//        lcd.setCursor(0, 1);
//        lcd.print("Exercise");
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
        lc.setIntensity(0,4);
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
        lc.clearDisplay(0);
        choice = 0;
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
        Phrase_state = Phrase_display;
        lc.clearDisplay(0);
        lc.setIntensity(0, 3);
        happyFace();
        lcd.clear();
//        lcd.setCursor(0,0);
//        lcd.print("Motivational");
//        lcd.setCursor(0, 1);
//        lcd.print("Phrases");
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
        break;

     case Phrase_waitPress2:
        if(digitalRead(right) == HIGH){
          rightPress = 1;
          Phrase_state = Phrase_waitRelease2; 
        }
        else if (digitalRead(up) == HIGH){
          upPress = 1;
          Phrase_state = Phrase_waitRelease2; 
        }
        else if (digitalRead(down) == HIGH){
          downPress = 1;
          Phrase_state = Phrase_waitRelease2; 
        }
        else{
          Phrase_state = Phrase_waitPress2;
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
            choice = 0;
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
        break;
   } // Transitions

  switch(Phrase_state) { // state actions  

     case Phrase_display:
          lcd.setCursor(16, 0);
          lcd.autoscroll();
          phraseSize = strlen(phrases[PhraseIndex]);
          for (int i = 0; i < phraseSize; i++) {
            letter = *(*(phrases + PhraseIndex) + i);
            lcd.print(letter);
            delay(200);
          }
          lcd.noAutoscroll();
        break;

     case Phrase_next:
        if (PhraseIndex < PSIZE - 1){
          PhraseIndex++;  
        }
        else{
          PhraseIndex = PSIZE - 1;  
        }
        break;

     case Phrase_previous:
        if (PhraseIndex > 0){
          PhraseIndex--;  
        }
        else{
          PhraseIndex = 0;  
        }
        break;
        
     default:
        break;
  } //state actions
}


//This function acts as a menu for the user
//displaying choices and setting a variable based on what was chosen
enum Menu_states { Menu_init, Menu_display, Menu_waitPress, Menu_waitRelease, Menu_previous, Menu_next, Menu_set} Menu_state;
void TickFct_Menu(){
  switch (Menu_state){ //Transitions
    case Menu_init:
        lc.clearDisplay(0);
        lcd.clear();
        MenuIndex = 0;
        choice = 0;      
        Menu_state = Menu_display;
        break;

    case Menu_display:
        Menu_state = Menu_waitPress;
        break;

    case Menu_waitPress:
        if(digitalRead(right) == HIGH){
          rightPress = 1;
          Menu_state = Menu_waitRelease; 
        }
        else if (digitalRead(up) == HIGH){
          upPress = 1;
          Menu_state = Menu_waitRelease; 
        }
        else if (digitalRead(down) == HIGH){
          downPress = 1;
          Menu_state = Menu_waitRelease; 
        }
        else{
          Menu_state = Menu_waitPress;
        }
        break;

     case Menu_waitRelease:
        if (digitalRead(right) == HIGH || digitalRead(down) == HIGH || digitalRead(up) == HIGH){
          Menu_state = Menu_waitRelease;  
        }
        else{
          if (upPress){
            Menu_state = Menu_next; 
            upPress = 0; 
          }
          else if (downPress){
            Menu_state = Menu_previous; 
            downPress = 0; 
          }
          else if (rightPress){
            Menu_state = Menu_set; 
            rightPress = 0; 
          }
          else{
            Menu_state = Menu_waitPress;  
          }  
        }
        break;

     case Menu_next:
        Menu_state = Menu_display;
        lcd.clear();
        break;

     case Menu_previous:
        Menu_state = Menu_display;
        lcd.clear();
        break;

     case Menu_set:
        Menu_state = Menu_init;
        lcd.clear();
        break;
    
    default: 
        Menu_state = Menu_init;
      break;
  }
  
  switch (Menu_state){ //Actions
    case Menu_display:
        lcd.print(funcNames[MenuIndex]);
        lcd.setCursor(0, 1);
        lcd.print(funcNames[MenuIndex+1]);
        break;

    case Menu_next:
        if (MenuIndex < FSIZE - 2){
          MenuIndex+= 2;  
        }
        else{
          MenuIndex = FSIZE - 2;  
        }
        break;

     case Menu_previous:
        if (MenuIndex > 0){
          MenuIndex-= 2;  
        }
        else{
          MenuIndex = 0;  
        }
        break;

     case Menu_set:
        if (MenuIndex == 0 || MenuIndex == 1){
          choice = 1;  
        }
        else if (MenuIndex == 2 || MenuIndex == 3){
          choice = 2;  
        }
        else{
          choice = 0;  
        }
    
     default: 
        break;
  }
  
}

//======END STATE MACHINE FUNCTIONS======//


//======SETUP======//
void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);

  //set up LCD 
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
  Menu_state = Menu_init;
}
//======END SETUP======//

void loop() {
  // put your main code here, to run repeatedly:
  if (choice == 0){
    TickFct_Menu();  
  }
  else if (choice == 1){
    TickFct_Breathe();  
  }
  else if (choice == 2){
    TickFct_Phrases();  
  }
  else{
    choice = 0;  
  }
}
