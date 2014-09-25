#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Twitter.h>

LiquidCrystal lcd(0, 1, 2, 6, 7, 8);

char *menuTree[] = {"mainMenu", "Users", "Add user"};
char *mainMenu[] = {"Choose user", "Add user"};
char *twitterHandles[] = {"@EchoEsq", "@SindreIvers", "@KevinMidboe", "@Odinbn", "@Hozar132"};
char *itsDone[] = {"Vannet er ferdig!", "TEATIME!", "my Man, the water is done!"};

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
int resetPin = 3;
int buttonState;
int UD = 1;
int LR = 1;

boolean movedDown = false;
int upDownCount = 0;
int menuSelector = 0;
int mainMenuController = 0;
int animationSelector = 2;

char* username;

int lengthMainMenu = sizeof(mainMenu);
int lengthUserMenu = sizeof(twitterHandles);

void setup()
{
	pinMode(resetPin, OUTPUT);

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
		LR = analogRead(A0);
		UD = analogRead(A1);
		if (UD == 0)
		{
			movedDown = false;
			upDownCount--;
			if (upDownCount < 0)
			{
				upDownCount = 0;
			}
			upDownCount = writeMainMenu(movedDown, upDownCount);
		}
		if (UD == 1022)
		{
			movedDown = true;
			upDownCount = writeMainMenu(movedDown, upDownCount);
			upDownCount++;
		}
		if (LR == 0)
		{
			menuSelector = 1;
			upDownCount = 0;
			upDownCount = writeUserMenu(movedDown, upDownCount);
			delay(400);
		}
	}
	while (menuTree[menuSelector] == menuTree[1])
	{
		delay(200);
		LR = analogRead(A0);
		UD = analogRead(A1);
		if (UD == 0)
		{
			movedDown = false;
			upDownCount--;
			if (upDownCount < 0)
			{
				upDownCount = 0;
			}
			upDownCount = writeUserMenu(movedDown, upDownCount);
		}
		if (UD == 1022)
		{
			movedDown = true;
			upDownCount = writeUserMenu(movedDown, upDownCount);
			upDownCount++;
		}
		if (LR == 0)
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
		if (LR == 1022)
		{
			menuSelector = 0;
			upDownCount = 0;
			upDownCount = writeMainMenu(movedDown, upDownCount);
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
		waterOnAndTweet(username);
	}
	else
	{
		checkWaterON(username);
	}
}

void waterOnAndTweet(char* username)
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
	    waterOnAndTweet(username);
	}
	if (twitter.post(username)) 
	{
	    int status = twitter.wait(&lcd);
	    if (status == 200) 
	    {
	    	waitForPickup();
	    } 
	    else 
	    	if(status == 403)
		    {
		    	lcd.clear();
		    	lcd.write("Duplicate tweet!");
		    	lcd.setCursor(0,1);
		    	lcd.write(status);
		    	delay(65000);
		    	twitter.post(username);

		    	// int status = twitter.wait(&lcd);
		    	// if(status == 200)
	    		// {
	    		// 	waitForPickup();		    			
	    		// }
	    		// else
	    		// {
	    		// 	waterOnAndTweet(username);
	    		// }
		    }
  	}
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
	while (buttonState != 0)
	{
		lcd.clear();
		lcd.write("RESET!");
		digitalWrite(resetPin, HIGH);
		delay(3000);
	}
}
