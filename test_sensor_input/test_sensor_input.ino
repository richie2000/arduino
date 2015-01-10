// Setup constants
#define sensorLeft   0 // input from left sensor
#define sensorRight  1 // input from right sensor
#define ledLeft      9 // left LED
#define ledRight     8 // right LED

void setup() {
  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);
  //Serial.begin(9600);
  
  // Flash 3 times at startup
  for(int x = 0; x <= 5; x++) {
    toggleLed(ledLeft);
    toggleLed(ledRight);
    delay(200);
  }
}


void loop() {
  checkSensor(sensorLeft, ledLeft);
  checkSensor(sensorRight, ledRight);
  delay(150);
}


void checkSensor(int sensor, int led) {
  //Serial.println(sensor);
  if (analogRead(sensor) > 200) {
    // Object in blindspot.
    toggleLed(led);
  }
  else {
    // No car in blindspot. Write LOW
    digitalWrite(led, LOW);
  }
}


void toggleLed(int led) {
  if (digitalRead(led) == true) {
      digitalWrite(led, LOW);
    } else {
      digitalWrite(led, HIGH);
    }
}

