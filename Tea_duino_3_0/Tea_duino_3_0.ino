#include <LiquidCrystal.h>
#include <JoystickController.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Twitter.h>

LiquidCrystal lcd(0, 1, 2, 6, 7, 8);

char *menuTree[] = {"mainMenu", "Users", "Add user"};
char *mainMenu[] = {"Choose user", "Add user"};

char *twitterHandles[] = {"@KevinMidboe ", "@EchoEsq ", "@SindreIvers ", "@Hozar132 ", "@Odinbn "};
char *donePhrases[] = {"Vannet er ferdig! ", "TEATIME ", "my man, the water is done! ", "Kom å hent meg! "};

char *errorWhereIsBoiler [] = {"Sett vannkokeren", "pa knappen."}; 
char *startWaterMessage [] = {"Skru pa vannet!"};

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

Twitter twitter("2307428619-TIM5H7Lh6L9HrrQLMynR5cinWpNbiUTt2827myM");
byte mac[] = {0x90, 0xA2, 0xDa, 0x0d, 0xA7, 0x51};

const int buttonPin = 3;
const int resetPin = 3;
int buttonState;
int UD = 1;
int LR = 1;

boolean movedDown = false;
int upDownCount = 0;
int menuSelector = 0;
int mainMenuController = 0;
int animationSelector = 2;

char* username;

char buf[100];

int lengthMainMenu = sizeof(mainMenu);
int lengthUserMenu = sizeof(twitterHandles);

JoystickController controller;

void setup()
{
	lcd.begin(16,2);
	Ethernet.begin(mac);

	lcd.createChar(1, CoffeeHank);
	lcd.createChar(2, CoffeeEmpty);
	lcd.createChar(3, Coffee1);
	lcd.createChar(4, Coffee2);
	lcd.createChar(5, Coffee3);
	lcd.createChar(6, CoffeeSmoke1);
	lcd.createChar(7, CoffeeSmoke2);

	pinMode(buttonPin, INPUT);

	upDownCount = writeMainMenu(movedDown, upDownCount);
}

void loop()
{
	while (menuTree[menuSelector] == menuTree[0])
	{
		delay(200);
		controller.update();
		int direct = controller.getDirection();
		switch (direct)
		{
			case JoystickController::DIRECTION_RIGHT:
			{
				movedDown = false;
				upDownCount--;
				if (upDownCount < 0)
				{
					upDownCount = 0;
				}
				upDownCount = writeMainMenu(movedDown, upDownCount);
				break;
			}
			case JoystickController::DIRECTION_LEFT:
			{
				movedDown = true;
				upDownCount = writeMainMenu(movedDown, upDownCount);
				upDownCount++;
				break;
			}
			case JoystickController::DIRECTION_DOWN:
			{
				menuSelector = 1;
				upDownCount = 0;
				upDownCount = writeUserMenu(movedDown, upDownCount);
				delay(400);
				break;
			}
		}
	}
	while (menuTree[menuSelector] == menuTree[1])
	{
		delay(200);
		controller.update();
		int direct = controller.getDirection();
		switch (direct)
		{
			case JoystickController::DIRECTION_RIGHT:
			{
				movedDown = false;
				upDownCount--;
				if (upDownCount < 0)
				{
					upDownCount = 0;
				}
				upDownCount = writeUserMenu(movedDown, upDownCount);
				break;
			}
			case JoystickController::DIRECTION_LEFT:
			{
				movedDown = true;
				upDownCount = writeUserMenu(movedDown, upDownCount);
				upDownCount++;
				break;
			}
			case JoystickController::DIRECTION_DOWN:
			{
				username = twitterHandles[upDownCount];
				buttonState = analogRead(buttonPin);
				if (buttonState != 0)
				{
					lcd.clear();
					lcd.write(errorWhereIsBoiler[0]);
					lcd.setCursor(0,1);
					lcd.write(errorWhereIsBoiler[1]);
					delay(2000);
					upDownCount = writeUserMenu(movedDown, upDownCount);
				}
				else
				{
					checkWaterON(username);
				}
			}
			case JoystickController::DIRECTION_UP :
			{
				menuSelector = 0;
				upDownCount = 0;
				upDownCount = writeMainMenu(movedDown, upDownCount);
				break;
			}
		}
	}
}

int writeMainMenu(boolean movedDown, int upDownCount)
{
	int i = upDownCount;

	lcd.clear();

	if (movedDown == false)
	{
		lcd.write(">");
		lcd.write(mainMenu[i]);
		lcd.setCursor(1,1);
		lcd.write(mainMenu[i + 1]);
	}
	else
	{
		lcd.setCursor(1,0);
		lcd.write(mainMenu[i]);
		lcd.setCursor(0,1);
		lcd.write(">");
		lcd.write(mainMenu[i + 1]);
	}
	return i;
}

int writeUserMenu(boolean movedDown, int upDownCount)
{
	int i = upDownCount;

	lcd.clear();

	if (movedDown == false)
	{
		lcd.write(">");
		lcd.write(twitterHandles[i]);
		lcd.setCursor(1,1);
		lcd.write(twitterHandles[i + 1]);
	}
	else
	{
		lcd.setCursor(1,0);
		lcd.write(twitterHandles[i]);
		lcd.setCursor(0,1);
		lcd.write(">");
		lcd.write(twitterHandles[i + 1]);
	}
	return i;
}

void checkWaterON(char* username)
{
	lcd.clear();
	lcd.write(startWaterMessage[0]);
	delay(1500);

	buttonState = analogRead(buttonPin);
	if (buttonState != 0)
	{
		waterOn(username);
	}
	else
	{
		checkWaterON(username);
	}
}

void waterOn(char* username)
{
	buttonState = analogRead(buttonPin);
	while (buttonState != 0)
	{
		lcd.clear();
		lcd.setCursor(6, 1);
	    lcd.write(animationSelector);
	    lcd.setCursor(7, 1);
	    lcd.write(1);
		lcd.setCursor(9, 1);
	    lcd.write(animationSelector);
	    lcd.setCursor(10, 1);
	    lcd.write(1);
	    animationSelector++;
	    delay(1500);
	    if (animationSelector == 6)
	    {
	    	animationSelector = 2;
	    }
	    waterOn(username);
	}
	tweet(username);
}

 void tweet(char* username)
{
	String str(username);
        randomSeed(analogRead(4));
	long i = random(4);
        int j(i);
	String str2(donePhrases[j]);
	str += str2;
        i = random(20);
        int k(i);
        str += k;
        char charBuf[140];
	str.toCharArray(charBuf, 140);
        twitter.post(charBuf);
	/*if ()
        {
          int status = twitter.wait(&lcd);
          if(status == 403)
          {
            if (j == 0)
            {
              j++;
            }
            else
              if (j == 4)
              {
                j--;
              }
              else
              {
                j++;
              }
          String str3(username);
          String str4(donePhrases[j]);
          str3 += str4;
          char charBuf1[140];
          str3.toCharArray(charBuf1, 140);
          twitter.post(charBuf1);
          }
        }*/
        
        waitForPickup();
}

void waitForPickup()
{
	buttonState = analogRead(buttonPin);
	while (buttonState == 0)
	{
		lcd.clear();
		lcd.setCursor(6, 1);
		lcd.write(6);
		lcd.setCursor(7, 1);
		lcd.write(1);
		lcd.setCursor(9, 1);
		lcd.write(6);
		lcd.setCursor(10, 1);
		lcd.write(1);

		delay(1500);
		lcd.clear();
		lcd.setCursor(6, 1);
		lcd.write(7);
		lcd.setCursor(7, 1);
		lcd.write(1);
		lcd.setCursor(9, 1);
		lcd.write(7);
		lcd.setCursor(10, 1);
		lcd.write(1);
		delay(1500);
		waitForPickup();
	}
	if (buttonState != 0)
	{
                reset();
	}
}

void reset()
{
  lcd.clear();
  lcd.write("RESET!");
  delay(3000);
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
}
