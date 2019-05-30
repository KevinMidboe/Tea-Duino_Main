#include <SPI.h> // needed in Arduino 0019 or later
#include <Ethernet.h>
#include <Twitter.h>

// The includion of EthernetDNS is not needed in Arduino IDE 1.0 or later.
// Please uncomment below in Arduino IDE 0022 or earlier.
//#include <EthernetDNS.h>


// Ethernet Shield Settings
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xA7, 0x51 };

const int buttonPin = 9;
int buttonState = 0;

// If you don't specify the IP address, DHCP is used(only in Arduino 1.0 or later).
//byte ip[] = { 192, 168, 2, 250 };

// Your Token to Tweet (get it from http://arduino-tweet.appspot.com/)
Twitter twitter("2307428619-jTdwfFJ4r9aYuaYHQ2YeqBWQNOy6nSg6aTRequb");

// Message to post
char msg[] = "@KevinMidboe Vannet er ferdig!";

void setup()
{
  delay(1000);
  Ethernet.begin(mac);
  // or you can use DHCP for autoomatic IP address configuration.
  // Ethernet.begin(mac);
  Serial.begin(9600);
  Serial.println("Ready!");
  pinMode(buttonPin, INPUT);
  buttonState = LOW;
}

void loop()
{
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH)
  {
    Serial.println("connecting ...");
    if (twitter.post(msg)) {
    // Specify &Serial to output received response to Serial.
    // If no output is required, you can just omit the argument, e.g.
    // int status = twitter.wait();
    int status = twitter.wait(&Serial);
    if (status == 200) {
      Serial.println("OK.");
    } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("connection failed.");
  }
  delay(1000);
  }
}

