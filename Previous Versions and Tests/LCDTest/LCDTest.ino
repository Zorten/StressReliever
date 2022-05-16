#include <LedControl.h>
#include "Levels.h"
#include <LiquidCrystal.h>

const int rs = 13, en = 5, d4 = 4, d5 = 3, d6 = 2, d7 = 1;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lc.shutdown(0, false);
  lc.setIntensity(0, 3);
  lc.clearDisplay(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.print("INHALE");
  lc.setIntensity(0,0);
  allOn();
  delay(1000);
  lc.setIntensity(0, 1);
  delay(1000);
  lc.setIntensity(0, 2);
  delay(1000);
  lc.setIntensity(0, 3);
  delay(1000);
  lc.setIntensity(0, 4);
  delay(2000);
}
