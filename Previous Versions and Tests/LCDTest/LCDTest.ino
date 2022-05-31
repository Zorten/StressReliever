#include <LedControl.h>
#include "Levels.h"
#include <LiquidCrystal.h>

const int rs = 13, en = 5, d4 = 4, d5 = 3, d6 = 2, d7 = 1;
const int button = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int i = 0;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lc.shutdown(0, false);
  lc.setIntensity(0, 3);
  lc.clearDisplay(0);
  pinMode(button, INPUT);
  lcd.clear();
  lcd.setCursor(16, 0);
  lcd.print("And still, I rise");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (i < 18){
    lcd.scrollDisplayLeft();
    i++;  
  }
  
}
