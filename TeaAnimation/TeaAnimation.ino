#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Twitter.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(0, 1, 2, 6, 7, 8);

byte CoffeeEmpty[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b01110
};

byte CoffeeHank[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11000,
  0b01000,
  0b11000,
  0b00000
};

byte Coffee1[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b10001,
  0b10001,
  0b11111,
  0b01110
};

byte Coffee2[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b10001,
  0b11111,
  0b11111,
  0b01110
};

byte Coffee3[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b01110
};

byte CoffeeSmoke1[8] = {
  0b00101,
  0b01010,
  0b00101,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b01110
};

byte CoffeeSmoke2[8] = {
  0b01010,
  0b00101,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b01110
};

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xA7, 0x51 };
Twitter twitter("2307428619-jTdwfFJ4r9aYuaYHQ2YeqBWQNOy6nSg6aTRequb");
char msg[] = "@KevinMidboe @SindreIvers";

int buttonPin = 9;

int animationSelector = 2;
int buttonState = 0;

boolean waterDone = false;
boolean tweeted = false;

void setup() 
{ 
  Ethernet.begin(mac); 
  //Serial.begin(9600);
  
  lcd.createChar(1, CoffeeHank);
  lcd.createChar(2, CoffeeEmpty);
  lcd.createChar(3, Coffee1);
  lcd.createChar(4, Coffee2);
  lcd.createChar(5, Coffee3);
  lcd.createChar(6, CoffeeSmoke1);
  lcd.createChar(7, CoffeeSmoke2);
  
  lcd.begin(16, 2);
  
  pinMode(buttonPin, INPUT);
  buttonState = LOW;
}

void loop() 
{
  while (waterDone == false)
  {
    lcd.setCursor(9, 1);
    lcd.write(animationSelector);
    lcd.setCursor(10, 1);
    lcd.write(1);
    animationSelector++;
    delay(3000);
    lcd.clear();
    
    if (animationSelector > 5)
    {
      animationSelector = 2;
    }
    
    buttonState = digitalRead(buttonPin);
    if(buttonState == HIGH)
    {
      waterDone = true;
    }
  }
   buttonState = digitalRead(buttonPin);
   if (waterDone == true && buttonState == HIGH)
   {
     while(tweeted == false)
     { 
       Serial.println("connecting ...");
       if (twitter.post(msg)) 
       {
         // Specify &Serial to output received response to Serial.
         // If no output is required, you can just omit the argument, e.g.
         // int status = twitter.wait();
         int status = twitter.wait(&Serial);
         if (status == 200) 
         {
           Serial.println("OK.");
           tweeted = true;
         } 
         else 
         {
           Serial.print("failed : code ");
           Serial.println(status);
         }
       } 
       else 
       {
         Serial.println("connection failed.");
       }
     }
   }
   while(buttonState == HIGH && tweeted == true)
   {
     delay(1000);
     lcd.clear();
     lcd.setCursor(9, 1);
     lcd.write(6);
     lcd.setCursor(10, 1);
     lcd.write(1);
     delay(1200);
   
     lcd.clear();
     lcd.setCursor(9, 1);
     lcd.write(7);
     lcd.setCursor(10, 1);
     lcd.write(1);
     delay(1200);
       
     buttonState = digitalRead(buttonPin);
  }

  if (buttonState == LOW && tweeted == true)
  {
    lcd.clear();
    lcd.write(">Choose User");
    lcd.setCursor(1, 1);
    lcd.write("Add new User");
    delay(4000);
  }
}



