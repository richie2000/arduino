#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// Setup constants
#define sensorLeft   0 // input from left sensor
#define sensorRight  1 // input from right sensor
#define ledLeft      9 // left LED
#define ledRight     8 // right LED
#define TEMP_PROBE   5

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(TEMP_PROBE);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

//LiquidCrystal lcd();

void setup() {
  Serial.println("Temp Probe test");
  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);
  //lcd.begin(20, 4);
  //lcd.print("Hello Spiff");
  //Serial.begin(9600);
  
  // Flash 3 times at startup
  for(int x = 0; x <= 5; x++) {
    toggleLed(ledLeft);
    toggleLed(ledRight);
    delay(200);
  }
  
  // Start library
  sensors.begin();
}


void loop() {
  //lcd.setCursor(0,1);
  //lcd.print(millis() / 1000);
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print("C  (");
  Serial.print(sensors.getTempFByIndex(0));
  Serial.println("F)");
  
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

