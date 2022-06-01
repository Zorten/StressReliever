/*  Author: Zergio Ruvalcaba
 *
 *  Project Description: Welcome to Stress Reliever. This prototype product was implemented
 *  on the Arduino Uno, using an 8x8 LED Matrix with the MAX7219, and an LCD 1602.
 *  The purpose of this product is to reduce stress and ease anxiety by providing calm,
 *  fun, and interactive activites. This version of Stress Reliever comes with 4 main functions/activities:
 *    Breathing Exercises
 *    Motivational Phrases
 *    Song Melodies
 *    Arcade Game
 *    
 *  These functions can be traversed on the menu that appears when starting, using
 *  two horizontally aligned buttons to scroll through options, and a third button to 
 *  select desired function.
 *
 *  I acknowledge all content contained herein, excluding template, provided code, or example
 *  code, is my own original work.
 *  
 *  Demo Link: <>
 *  GitHub Repo: <https://github.com/Zorten/StressReliever>
 */



//======HEADER FILES======//
//I used this library to control my LED Matrix. Obtained from here: <https://www.arduino.cc/reference/en/libraries/ledcontrol/>
//I added my own function to check if any one LED is lit up.
#include <LedControl.h>

//I created this header file to hold values and functions that would clutter this code
#include "Levels.h"

//string library
#include <string.h>

//LCD library: <https://www.arduino.cc/reference/en/libraries/liquidcrystal/>
#include <LiquidCrystal.h>

//======END HEADER FILES======//

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
const double D4_freq = 293.66;
const double F4_freq = 349.23;
const double G4_freq = 392.00;
const double A4_freq = 440.00;
const double A4s_freq = 466.16;
const double B4_freq = 493.88;
const double C5_freq = 523.25;
const double C5s_freq = 554;
const double D5_freq = 587.33;
const double E5_freq = 659.00;
const double F5_freq = 698.46;
const double F5s_freq = 740;
const double G5_freq = 783.99;
const double G5s_freq = 831.00;
const double A5_freq = 880.00;
const double B5_freq = 988.00;
const double off = 0;

//to display right arrow
byte rightArrow[] = {B11000, B11100, B11110, B11111, B11111, B11110, B11100, B11000};

//to display left arrow
byte leftArrow[] = {B00011, B00111, B01111, B11111, B11111, B01111, B00111, B00011};

//to keep track of button presses 
int rightPress = 0;
int upPress = 0;
int downPress = 0;

//Array of phrases and variables to traverse/operate
const int PSIZE = 4;
char phrases[PSIZE][50] = {"And still, I rise", "Fall 7 times, stand up 8", "It is ok to not be ok", "You are your best thing"};
int phraseSize = 0;
char letter = ""; 

//Array of function names
const int FSIZE = 8;
String funcNames[FSIZE] = {"Breathing", "Exercise", "Motivational", "Phrases", "Song", "Melodies", "Arcade", "Game"};
int choice = 0; //this variable will be used to check if the user selected a function

//Array of song names and variables to traverse/operate
const int SSIZE = 3;
char songs[SSIZE][50] = {"Merry Go Round of Life", "Take On Me", "Rick Roll"};
int songsSize = 0;

//Arrays with note frequencies for each melody 
int melodySizes[3] = {17, 32, 29}; 
double melody1[17] = {D4_freq, G4_freq, A4s_freq, D5_freq, D5_freq, C5_freq, A4s_freq, A4_freq, A4s_freq, G4_freq, A4s_freq, D5_freq, G5_freq, G5_freq, A5_freq, A4s_freq, F5_freq};
double melody2[32] = {F5s_freq, F5s_freq,D5_freq, B4_freq, off, B4_freq, off, E5_freq, off, E5_freq, off, E5_freq, G5s_freq, G5s_freq, A5_freq, B5_freq, A5_freq, A5_freq, A5_freq, E5_freq, off, D5_freq, off, F5s_freq, off, F5s_freq, off, F5s_freq, E5_freq, E5_freq, F5s_freq, E5_freq};
double melody3[29] = {A4_freq, B4_freq, D5_freq, B4_freq, F5s_freq, F5s_freq, E5_freq, A4_freq, B4_freq, D5_freq, B4_freq, E5_freq, E5_freq, D5_freq, C5s_freq, B4_freq, A4_freq, B4_freq, D5_freq, B4_freq, D5_freq, E5_freq, C5s_freq, B4_freq, A4_freq, A4_freq, A4_freq, E5_freq, D5_freq};

//Array of difficulties
const int DSIZE = 3;
String diff[DSIZE] = {"Easy", "Medium", "Hard"};
int diffChoice = 0;

//Indexes to traverse arrays
int PhraseIndex = 0;
int MenuIndex = 0;
int SongIndex = 0;
int DifficultyIndex = 0;

/////////////////////////////////
//Variables for Game:
////////////////////////////////

//initial position
int initCol = 0; 
int initRow = 3;

//current position
int currCol = initCol;
int currRow = initRow;

//final position 
int finalCol = 7;
int finalRow = 4;

//check if specific led is lit up
int litUp = 0;

//determine if level has been won
bool levelWon = 0;

//initial level
int initLevel = 1;

//total number of levels
int maxLevel = 5;

//keep track of current level
int currLevel = initLevel;

//possible moves
const int moveUp = 1;
const int moveRight = 1;
const int moveDown = 1;

//flags for seeing which move to do 
bool goUp = 0;
bool goRight = 0;
bool goDown = 0;

//determine if game is over
bool gameOver = 0;

//keep track of lives
int lives = 3;

//total number of lives
int maxLives = 3;

//flags to see if wrong/correct move was made
bool wrongMove = 0;
bool validMove = 0;

//used to determine when to toggle exit LED
int loops = 5;

//used to blink exit LED
int toggle = 1;


//======END VARIABLES======//


//======HELPER FUNCTIONS======//

//  Function to display player in specified row and column
//  Also blinks the exit
void player(int row, int col){
    loops--;
    if (loops <= 0){
      toggle = !toggle;
      lc.setLed(0, finalRow, finalCol, toggle);
      loops = 5;
        
    }
    lc.setLed(0, row, col, 1);
    delay(100);
    lc.setLed(0, row,col, 0);
    delay(100);
}

//======END HELPER FUNCTIONS======//

//======STATE MACHINE FUNCTIONS======//

//This state machine function executes the breathing
//exercise option
enum Breathe_states { Breathe_init, Breathe_inhale, Breathe_hold, Breathe_exhale} Breathe_state;
void TickFct_Breathe(){
  switch(Breathe_state) { // Transitions
  
     case Breathe_init:// Initial transition
        Breathe_state = Breathe_inhale;
        lc.clearDisplay(0);
        lcd.clear();
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
        lc.setIntensity(0, 5);
        delay(1000);
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
        lc.clearDisplay(0);
        choice = 0;
        break;        
        
     default:
        break;
  } //state actions
}

//This function executes the motivational phrases option
enum Phrase_states { Phrase_init, Phrase_display, Phrase_waitPress, Phrase_waitRelease, Phrase_next, Phrase_previous} Phrase_state;
void TickFct_Phrases(){
  switch(Phrase_state) { // Transitions
  
     case Phrase_init:// Initial transition
        Phrase_state = Phrase_display;
        lc.clearDisplay(0);
        lc.setIntensity(0, 3);
        happyFace();
        lcd.clear();
        PhraseIndex = 0;
        break;
        
     case Phrase_display:
        Phrase_state = Phrase_waitPress;
        break;

     case Phrase_waitPress:
        if(digitalRead(right) == HIGH){
          rightPress = 1;
          Phrase_state = Phrase_waitRelease; 
        }
        else if (digitalRead(up) == HIGH){
          upPress = 1;
          Phrase_state = Phrase_waitRelease; 
        }
        else if (digitalRead(down) == HIGH){
          downPress = 1;
          Phrase_state = Phrase_waitRelease; 
        }
        else{
          Phrase_state = Phrase_waitPress;
        }
        break;

     case Phrase_waitRelease:
        if (digitalRead(right) == HIGH || digitalRead(down) == HIGH || digitalRead(up) == HIGH){
          Phrase_state = Phrase_waitRelease;  
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
            Phrase_state = Phrase_waitPress;  
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

          lcd.setCursor(0+phraseSize, 1);
          lcd.write(byte(1));
          lcd.setCursor(15+phraseSize, 1);
          lcd.write(byte(0));
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

//This function implements the song melodies function
enum Song_states { Song_init, Song_display, Song_waitPress, Song_waitRelease, Song_next, Song_previous, Song_play} Song_state;
void TickFct_Songs(){
  switch(Song_state) { // Transitions
  
     case Song_init:// Initial transition
        Song_state = Song_display;
        lc.setIntensity(0, 3);
        musicNote();
        lcd.clear();
        SongIndex = 0;
        break;
        
     case Song_display:
        Song_state = Song_waitPress;
        break;

     case Song_waitPress:
        if(digitalRead(right) == HIGH){
          rightPress = 1;
          Song_state = Song_waitRelease; 
        }
        else if (digitalRead(up) == HIGH){
          upPress = 1;
          Song_state = Song_waitRelease; 
        }
        else if (digitalRead(down) == HIGH){
          downPress = 1;
          Song_state = Song_waitRelease; 
        }
        else{
          Song_state = Song_waitPress;
        }
        break;

     case Song_waitRelease:
        if (digitalRead(right) == HIGH || digitalRead(down) == HIGH || digitalRead(up) == HIGH){
          Song_state = Song_waitRelease;  
        }
        else{
          if (upPress){
            Song_state = Song_next; 
            upPress = 0; 
          }
          else if (downPress){
            Song_state = Song_previous; 
            downPress = 0; 
          }
          else if (rightPress){
            Song_state = Song_play; 
            rightPress = 0; 
          }
          else{
            Song_state = Song_waitPress;  
          }  
        }
        break;

     case Song_next:
        Song_state = Song_display;
        lcd.clear();
        break;

     case Song_previous:
        Song_state = Song_display;
        lcd.clear();
        break;

     case Song_play:
        Song_state = Song_init;
        break;
        
     default:
        Song_state = Song_init;
        break;
   } // Transitions

  switch(Song_state) { // state actions  

     case Song_display:
          lcd.setCursor(16, 0);
          lcd.autoscroll();
          songsSize = strlen(songs[SongIndex]);
          for (int i = 0; i < songsSize; i++) {
            letter = *(*(songs + SongIndex) + i);
            lcd.print(letter);
            delay(200);
          }
          lcd.noAutoscroll();

          lcd.setCursor(0+songsSize, 1);
          lcd.write(byte(1));
          lcd.setCursor(15+songsSize, 1);
          lcd.write(byte(0));
          break;

     case Song_next:
        if (SongIndex < SSIZE - 1){
          SongIndex++;  
        }
        else{
          SongIndex = SSIZE - 1;  
        }
        break;

     case Song_previous:
        if (SongIndex > 0){
          SongIndex--;  
        }
        else{
          SongIndex = 0;  
        }
        break;

     case Song_play:
        lcd.noDisplay();
        if (SongIndex == 0){
          for (int i = 0; i < melodySizes[SongIndex]; i++){
          tone(buzzer, melody1[i]);
          delay(550);
          noTone(buzzer); 
          }
        }
        else if (SongIndex == 1){
          for (int i = 0; i < melodySizes[SongIndex]; i++){
          tone(buzzer, melody2[i]);
          delay(215);
          noTone(buzzer); 
          }  
        }
        else if (SongIndex == 2){
          for (int i = 0; i < melodySizes[SongIndex]; i++){
          tone(buzzer, melody3[i]);
          delay(250);
          tone(buzzer, off);
          delay(50);
          noTone(buzzer); 
          }
        }
        lcd.display();
        choice = 0;
        break;
        
     default:
        break;
  } //state actions
}


//This function lets the user select a difficulty for the arcade game
enum Difficulty_states { Difficulty_init, Difficulty_display, Difficulty_waitPress, Difficulty_waitRelease, Difficulty_next, Difficulty_previous, Difficulty_play} Difficulty_state;
void TickFct_Difficulty(){
  switch(Difficulty_state) { // Transitions
  
     case Difficulty_init:// Initial transition
        Difficulty_state = Difficulty_display;
        lc.clearDisplay(0);
        lcd.clear();
        DifficultyIndex = 0;
        break;
        
     case Difficulty_display:
        Difficulty_state = Difficulty_waitPress;
        break;

     case Difficulty_waitPress:
        if(digitalRead(right) == HIGH){
          rightPress = 1;
          Difficulty_state = Difficulty_waitRelease; 
        }
        else if (digitalRead(up) == HIGH){
          upPress = 1;
          Difficulty_state = Difficulty_waitRelease; 
        }
        else if (digitalRead(down) == HIGH){
          downPress = 1;
          Difficulty_state = Difficulty_waitRelease; 
        }
        else{
          Difficulty_state = Difficulty_waitPress;
        }
        break;

     case Difficulty_waitRelease:
        if (digitalRead(right) == HIGH || digitalRead(down) == HIGH || digitalRead(up) == HIGH){
          Difficulty_state = Difficulty_waitRelease;  
        }
        else{
          if (upPress){
            Difficulty_state = Difficulty_next; 
            upPress = 0; 
          }
          else if (downPress){
            Difficulty_state = Difficulty_previous; 
            downPress = 0; 
          }
          else if (rightPress){
            Difficulty_state = Difficulty_play; 
            rightPress = 0; 
          }
          else{
            Difficulty_state = Difficulty_waitPress;  
          }  
        }
        break;

     case Difficulty_next:
        Difficulty_state = Difficulty_display;
        lcd.clear();
        break;

     case Difficulty_previous:
        Difficulty_state = Difficulty_display;
        lcd.clear();
        break;

     case Difficulty_play:
        Difficulty_state = Difficulty_init;
        break;
        
     default:
        Difficulty_state = Difficulty_init;
        break;
   } // Transitions

  switch(Difficulty_state) { // state actions  

     case Difficulty_display:
        lcd.setCursor(2, 0);
        lcd.print(diff[DifficultyIndex]);
        lcd.setCursor(0, 1);
        lcd.write(byte(1));
        lcd.setCursor(15, 1);
        lcd.write(byte(0));
          break;

     case Difficulty_next:
        if (DifficultyIndex < DSIZE - 1){
          DifficultyIndex++;  
        }
        else{
          DifficultyIndex = DSIZE - 1;  
        }
        break;

     case Difficulty_previous:
        if (DifficultyIndex > 0){
          DifficultyIndex--;  
        }
        else{
          DifficultyIndex = 0;  
        }
        break;

     case Difficulty_play:
        if (DifficultyIndex == 0){
          maxLevel = 1;
        }
        else if (DifficultyIndex == 1){
          maxLevel = 3; 
        }
        else if (DifficultyIndex == 2){
          maxLevel = 5;
        }
        else{
          maxLevel = 1;  
        }
        diffChoice = 1;
        break;
        
     default:
        break;
  } //state actions
}


//This state machine will wait for and capture user input on the buttons
//and then move the LED on the maze accordingly, restarting if wall was hit or went out of bounds
enum ButtonMove_BM_states { Button_waitPress, Button_waitRelease, Button_check} BM_state;
void TickFct_ButtonMove(){
  switch(BM_state) { // Transitions
        
     case Button_waitPress: // Initial transition - wait for user to press button
        if ((digitalRead(right) == LOW) && (digitalRead(up) == LOW) && (digitalRead(down) == LOW) ){
          BM_state = Button_waitPress;  
        }
        else if ((digitalRead(right) == HIGH)){
          goRight = 1;
          BM_state = Button_waitRelease; 
        }
        else if ((digitalRead(up) == HIGH)){
          goUp = 1;
          BM_state = Button_waitRelease;  
        }
        else if ((digitalRead(down) == HIGH)){
          goDown = 1;
          BM_state = Button_waitRelease;  
        }
        else{
          BM_state = Button_waitPress;
        }
        break;
        
     case Button_waitRelease: //wait for user to release button
        if (goUp){
          if (digitalRead(up) == HIGH){
            BM_state = Button_waitRelease;  
          }
          else{
            BM_state = Button_check;
          }
        }
        else if (goRight){
          if (digitalRead(right) == HIGH){
            BM_state = Button_waitRelease;  
          }
          else{
            BM_state = Button_check; 
          }
        }
        else if (goDown){
          if (digitalRead(down) == HIGH){
            BM_state = Button_waitRelease;  
          }
          else{
            BM_state = Button_check;
          }
        }
        else{
            BM_state = Button_waitPress;
        }
        break;

     case Button_check: 
        BM_state = Button_waitPress;
        
     default:
        BM_state = Button_waitPress;
   } // Transitions

  switch(BM_state) { // state actions
     case Button_check: //check which button was pressed and respond accordingly
        if (goUp){
          //litUp is 1 if the LED you're trying to move to is on
          litUp = lc.getLed(currRow - moveUp, currCol);
          //if the LED is on or if it is out of bounds, 
          //move player to starting position and mark a wrong move
          if (litUp || ((currRow - moveUp) < 0)){
            currCol = initCol;
            currRow = initRow;
            wrongMove = 1;
            
          }
          //otherwise move player to specified spot and mark a valid move
          else{
            currCol = currCol;
            currRow = currRow - moveUp;
            validMove = 1;  
          }
          goUp = 0; //reset flag
        }
        else if (goRight){
          litUp = lc.getLed(currRow, currCol + moveRight);
          if (litUp || ((currCol + moveRight) > 7)){
            currCol = initCol;
            currRow = initRow;
            wrongMove = 1;
          }
          else{
            currCol += moveRight;
            currRow = currRow;  
            validMove = 1;
          } 
          goRight = 0;
        } 
        else if (goDown){
          litUp = lc.getLed(currRow + moveDown, currCol);
          if (litUp || ((currRow + moveDown) > 7)){
            currCol = initCol;
            currRow = initRow;
            wrongMove = 1;
          }
          else{
            currCol = currCol;
            currRow += moveDown;
            validMove = 1;  
          }
          goDown = 0;
        }    

        if (wrongMove){ //take a life and play tune to indicate invalid move
          wrongMove = 0;
          lives--;
          tone(buzzer, F5_freq);
          delay(100);
          tone(buzzer, G5_freq);
          delay(100);
          noTone(buzzer);  
        }

        if (validMove){ //play a tune when valid move is made
          validMove = 0;
          tone(buzzer, F5_freq); 
          delay(50);
          noTone(buzzer); 
        }

        //check if user has lost all lives and restart game if so
        if (lives <= 0){
           gameOver = 1;
           delay(500);          
        }
            
        //check if the player has reached the exit, and if so set the levelWon flag
        //and return player to starting positon
        if ((currRow == finalRow) && (currCol == finalCol)){
          levelWon = 1; 
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
        lcd.setCursor(2, 0);
        lcd.print(funcNames[MenuIndex]);
        lcd.setCursor(2, 1);
        lcd.print(funcNames[MenuIndex+1]);
        lcd.setCursor(0, 1);
        lcd.write(byte(1));
        lcd.setCursor(15, 1);
        lcd.write(byte(0));
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
        else if (MenuIndex == 4 || MenuIndex == 5){
          choice = 3;  
        }
        else if (MenuIndex == 6 || MenuIndex == 7){
          choice = 4;  
        }
        else{
          choice = 0;  
        }
    
     default: 
        break;
  }
}

//This function displays a little animation and plays
//a tune to indicate level was passed.
//Only one state since animation needs to play at once.
enum LevelWon_States { LW_celebration } LW_state;
void TickFct_LevelWon(){
  switch(LW_state) { // Transitions
     case LW_celebration: // Initial transition
        LW_state = LW_celebration;
        break;
        
     default:
        LW_state = LW_celebration;
   } // Transitions

  switch(LW_state) { // state actions
     case LW_celebration:
        lc.clearDisplay(0);
        tone(buzzer, A5_freq);
        circle1();
        delay(500);
        lc.clearDisplay(0);
        tone(buzzer, D5_freq);
        circle2();
        delay(500);
        lc.clearDisplay(0);
        tone(buzzer, F5_freq);
        circle3();
        delay(500);
        lc.clearDisplay(0);
        tone(buzzer, G5_freq);
        circle4();
        delay(500);
        noTone(buzzer); 
        lc.clearDisplay(0);      
        break;
     default:
        break;
  } // BM_state actions
}

//This function shows a message telling the user they won and plays a tune
enum GameWon_States { GW_celebration } GW_state;
void TickFct_GameWon(){
  switch(GW_state) { // Transitions
     case GW_celebration: // Initial transition
        GW_state = GW_celebration;
        break;
        
     default:
        GW_state = GW_celebration;
   } // Transitions

  switch(GW_state) { // state actions
     case GW_celebration:
        lc.clearDisplay(0);
        tone(buzzer, G5_freq);
        charU();
        delay(800);
        lc.clearDisplay(0);
        tone(buzzer, F5_freq);
        charW();
        delay(800);
        lc.clearDisplay(0);
        tone(buzzer, D5_freq);
        charO();
        delay(800);
        lc.clearDisplay(0);
        tone(buzzer, A5_freq);
        charN();
        delay(800);
        lc.clearDisplay(0);
        tone(buzzer, A5_freq);
        happyFace();
        delay(1000);
        noTone(buzzer); 
        lc.clearDisplay(0);      
        break;
     default:
        break;
  } //state actions
}

//This function tells the user they lost, and plays a tune
enum GameLost_States { GL_lost } GL_state;
void TickFct_GameLost(){
  switch(GL_state) { // Transitions
     case GL_lost: // Initial transition
        GL_state = GL_lost;
        break;
        
     default:
        GL_state = GL_lost;
   } // Transitions

  switch(GL_state) { // state actions
     case GL_lost:
        lc.clearDisplay(0);
        tone(buzzer, A5_freq);
        circle4();
        delay(500);
        lc.clearDisplay(0);
        tone(buzzer, G5_freq);
        circle3();
        delay(500);
        lc.clearDisplay(0);
        tone(buzzer, F5_freq);
        circle2();
        delay(500);
        lc.clearDisplay(0);
        tone(buzzer, D5_freq);
        circle1();
        delay(800);
        lc.clearDisplay(0);
        tone(buzzer, D5_freq);
        sadFace();
        delay(500);
        noTone(buzzer); 
        lc.clearDisplay(0);      
        break;
     default:
        break;
  } //state actions
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
  Song_state = Song_init;
  BM_state = Button_waitPress;
  Difficulty_state = Difficulty_init;

  //create special arrow characters
  lcd.createChar(0, rightArrow);
  lcd.createChar(1, leftArrow);
}
//======END SETUP======//

void loop() {
  // put your main code here, to run repeatedly:

  //Display menu and choice options until choice is made with enter button
  if (choice == 0){
    TickFct_Menu();  
  }
  //Play breathing function
  else if (choice == 1){
    TickFct_Breathe();  
  }
  //Play motivational phrases function
  else if (choice == 2){
    TickFct_Phrases();  
  }
  //Play song melodies function
  else if (choice == 3){
    TickFct_Songs();  
  }
  //Play arcade game function
  else if (choice == 4){
    if (diffChoice == 0){ //Wait for user to select difficulty
      TickFct_Difficulty();  
    }
    else{
      //turn off lcd while playing game
      lcd.noDisplay();
      //as long as the game is not over, iterate over the levels 
      if (!gameOver){
        if (!levelWon){
          //as long as level isn't won, call ButtonMove function and adjust position  
          levels(currLevel);
          TickFct_ButtonMove();
          player(currRow, currCol);
        }
        else{
          //if not final level, move on to next level and update postion
          if (currLevel < maxLevel){
            TickFct_LevelWon();
            levelWon = 0;
            currRow = initRow;
            currCol = initCol;
            currLevel++;    
          }
          //if final level passed, display winning message, update variables, and end game
          else if (currLevel == maxLevel){
            TickFct_LevelWon();
            delay(250);
            TickFct_GameWon();
            delay(500);
            lc.clearDisplay(0);
            levelWon = 0;
            currRow = initRow;
            currCol = initCol;
            currLevel = initLevel;
            lives = 3;
            choice = 0;
          }
        }
      }
      else{
        //if game is lost, update variables to restart and display losing message
        gameOver = 0;
        lives = 3;
        currLevel = initLevel;
        currRow = initRow;
        currCol = initCol;
        levelWon = 0;
        diffChoice = 0;
        TickFct_GameLost();
        delay(250);
        choice = 0;
      }
      lcd.display();
    }
  }
  else{
      choice = 0;  
    }
}
