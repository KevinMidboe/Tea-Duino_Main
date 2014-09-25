// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int UD = 0;
int LR = 0;
const int buttonPin = 2;

char* myString [] = {"@EchoEsq", "@SindreIvers", "@KevinMidboe", "@Odinbn", "@hozar132"};
int length = 0;

int buttonState = 0;

int i = 0;

void setup(){
  lcd.begin(16, 2);

  pinMode(buttonPin, INPUT);

  Serial.begin(9600);
}

void loop()
{
  //Get length of myString
  length = 5;
    
  UD = analogRead(0);
  LR = analogRead(1);
  /*
  if (UD > 512)
  {
    Serial.print("UP");
    delay(800);
  }
  else
    if (UD < 512)
  {
    Serial.print("Down");
    delay(800);
  }
  else
  if (LR < 512 && UD > 512)
  {
    Serial.print("Right");
    delay(800);
  } 
  else
  if (LR < 512)
  {
    Serial.print("Left");
    delay(800);
  }*/
  
   buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH)
  {
    lcd.clear();
    lcd.write(">");
    lcd.write(myString[i]);
    lcd.setCursor(1,1);
    lcd.write(myString[i + 1]);
    delay(800);
    i++;
  }
  if (i >= length)
  {
    i = 0;
  }
}
