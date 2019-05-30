int UD = 0;
int LR = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  UD = analogRead(A0);
  LR = analogRead(A1);
  if (UD == 0)
  {
    Serial.println("This is down!");
  }
  else
  if (UD == 1023)
  {
    Serial.println("This is up!");
  }
  else
  if (LR == 0)
  {
    Serial.println("This is Left!");
  }
  else
  if (LR == 1023)
  {
    Serial.println("This is Right!");
  } 
  delay(10);
}
