#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Twitter.h>

LiquidCrystal lcd(0, 1, 2, 6, 7, 8);

char *mainMenu[] = {"Choose user", "Add new user"};
char *twitterHandles[] = {"@EchoEsq", "@SindreIvers", "@KevinMidboe", "@OdinBN", "@Hozar132"};
char *alphabet[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", 
                    "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "0", "1", "2", "3",
                    "4", "5", "6", "7", "8", "9"};

byte CoffeeEmpty[8] = {
  0b00000, 0b00000, 0b00000, 0b11111,
  0b10001, 0b10001, 0b10001, 0b01110};

byte CoffeeHank[8] = {
  0b00000, 0b00000, 0b00000, 0b00000,
  0b11000, 0b01000, 0b11000, 0b00000};

byte Coffee1[8] = {
  0b00000, 0b00000, 0b00000, 0b11111,
  0b10001, 0b10001, 0b11111, 0b01110};

byte Coffee2[8] = {
  0b00000, 0b00000, 0b00000, 0b11111,
  0b10001, 0b11111, 0b11111, 0b01110};

byte Coffee3[8] = {
  0b00000, 0b00000, 0b00000, 0b11111,
  0b11111, 0b11111, 0b11111, 0b01110};

byte CoffeeSmoke1[8] = {
  0b00101, 0b01010, 0b00101, 0b11111,
  0b11111, 0b11111, 0b11111, 0b01110};

byte CoffeeSmoke2[8] = {
  0b01010, 0b00101, 0b01010, 0b11111,
  0b11111, 0b11111, 0b11111, 0b01110};


//byte mac[] = {0x90, 0xA2, 0xDa, 0x0d, 0xA7, 0x51};
Twitter twitter("2307428619-jTdwfFJ4r9aYuaYHQ2YeqBWQNOy6nSg6aTRequb");
char message[] = "Didn't have anyone to send to... :(";

const int buttonPin = 9;
int buttonState = 0;
int animatorSelector = 2;
int UD = 1;
int LR = 1;

int i = 1;
int j = 0;
int k = 0;
int lengthMainMenu = 2;
int lengthTwitterHandles = 5;
int lengthAlphabet = 36;

boolean boolMainMenu = true;
boolean boolTwitterHandles = false;
boolean waterBoiling = false;
int selected = 0;


void setup() 
{
  lcd.begin(16, 2);

  pinMode(buttonPin, INPUT);

  //Ethernet.begin(mac);

  lcd.write("Ready!");
  delay(1000);
  lcd.clear();

  lcd.createChar(1, CoffeeHank);
  lcd.createChar(2, CoffeeEmpty);
  lcd.createChar(3, Coffee1);
  lcd.createChar(4, Coffee2);
  lcd.createChar(5, Coffee3);
  lcd.createChar(6, CoffeeSmoke1);
  lcd.createChar(7, CoffeeSmoke2); 
}

void loop() 
{
  while (boolMainMenu == true)
  {
    LR = analogRead(A0);
    UD = analogRead(A1);
    delay(300); 
	if (UD == 1022)
    {
      i = 0;
      lcd.clear();
      lcd.write(">");
      lcd.write(mainMenu[i]);
      lcd.setCursor(1,1);
      lcd.write(mainMenu[i + 1]);
      delay(800);
      selected = 0;
    }
	if (UD == 0)
	{
	  i = 0;
	  lcd.clear();
	  lcd.setCursor(1, 0);
	  lcd.write(mainMenu[i]);
	  lcd.setCursor(0, 1);
	  lcd.write(">");
	  lcd.write(mainMenu[i + 1]);
	  delay(800);
	  selected = 1;
	}
	LR = analogRead(A0);
	UD = analogRead(A1);
    if (LR == 1022)
    {
      if (selected == 0)
      {
        boolMainMenu = false;
        boolTwitterHandles = true;
        i = 0;
        lcd.clear();
        lcd.write(">");
        lcd.write(twitterHandles[i]);
        lcd.setCursor(1, 1);
        lcd.write(twitterHandles[i + 1]);
      }
      else
        if (selected == 1)
        {
          LR = analogRead(A0);
          UD = analogRead(A1);
          delay(300);
          if (UD == 1022)
          {
            k++;
            if (k == 37)
            {
              k = 0;
            }
          }
          if (UD == 0)
          {
            k--;
            if (k == -1)
            {
              k = 37;
            }
          }

          lcd.clear();
          lcd.write(alphabet[k]);
          lcd.write(alphabet[k + 1]);

          /*for(int k = 0; k < lengthTwitterHandles; k++)
          {

          }*/

          if (LR == 0)
          {
            //Save the typed-inn name
            //Add @ to the name
            lcd.clear();
            lcd.write(">");
            lcd.write(mainMenu[i]);
            lcd.setCursor(1,1);
            lcd.write(mainMenu[i + 1]);
            delay(800);
          }
        }
    }
  }

  while(boolTwitterHandles == true)
  {	
    LR = analogRead(A0);
	UD = analogRead(A1);
	delay(300);	
	if (UD == 0)
	{
	  lcd.clear();
	  lcd.write(">");
	  lcd.write(mainMenu[i]);
	  lcd.setCursor(1,1);
	  lcd.write(mainMenu[i + 1]);
	  delay(800);
	  i++;
	  if (i == lengthMainMenu)
	  {
	    i = 0;
	  }
	}
	if (UD == 1022)
	{
	  lcd.clear();
	  lcd.write(">");
	  lcd.write(mainMenu[i + 1]);
	  lcd.setCursor(1,1);
	  lcd.write(mainMenu[i]);
	  delay(800);
	  i--;
	  if (i == 0)
	  {
        //i = lengthMainMenu;
	    i = 1;
	  }
	}
	LR = analogRead(A0);
	UD = analogRead(A1);
    if (LR == 1022)
    {
      //ERROR
      //Invalid conversion from 'char*' to 'char'
      //message[] = twitterHandles[i];
      waterBoiling = false;
      boolTwitterHandles = false;
      lcd.clear();
    }
    LR = analogRead(A0);
	UD = analogRead(A1);
    if (LR == 0)
    {
      boolTwitterHandles = false;
      boolMainMenu = true;
      i = 0;
      lcd.clear();
      lcd.write(">");
      lcd.write(mainMenu[i]);
      lcd.setCursor(1, 1);
      lcd.write(mainMenu[i + 1]);
    }
  }

  //Maybe (while(waterBoiling == true && buttonState == LOW))
  while(waterBoiling == true)
  {
    lcd.setCursor(9, 1);
    lcd.write(animatorSelector);
    lcd.setCursor(10, 1);
    lcd.write(1);
    animatorSelector++;
    delay(3000);
    lcd.clear();

    if(animatorSelector > 5)
    {
      animatorSelector = 2;
    }

    buttonState = digitalRead(buttonPin);
    if(buttonState == HIGH)
    {
      twitter.post(message);
      waterBoiling = false;
      buttonState = digitalRead(buttonPin);
    }
  }

  while(waterBoiling == false && buttonState == HIGH)
  {
    lcd.clear();
    lcd.setCursor(9, 1);
    lcd.write(6 + j);
    lcd.setCursor(10, 1);
    lcd.write(1);
    j++;
    delay(1200);
    if (j == 2)
    {
      j = 0;
    }
    buttonState = digitalRead(buttonPin);
    if(buttonState == LOW)
    {
      boolMainMenu = true;
      lcd.clear();
    }
  }
}
