const int buttonPin = A8;
// button pins: A10, A9, A8

boolean buttonState = LOW;

void setup() {
  Serial.begin(9600);
  Serial.println("Hello");
  
  pinMode(buttonPin, INPUT_PULLUP);
  
}

void loop() {
  if (digitalRead(buttonPin) == LOW)
  {
    Serial.println("LOW");
  }

  if (digitalRead(buttonPin) == HIGH)
  {
    Serial.println("HIGH");
  }
  delay(200); 
}
