#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

// Setup constants
//#define SENSOR_LEFT  0   // input from left sensor
//#define SENSOR_RIGHT 1   // input from right sensor
#define GPS_RX       0
#define GPS_TX       1
#define TEMP_PROBE   5   // input from temperature sensor
#define BUTTON       7   // 
#define LED_RIGHT    8   // right LED
#define LED_LEFT     9   // left LED

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(TEMP_PROBE);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
// Setup LCD
LiquidCrystal lcd(6, 3, 10, 16, 14, 15);
TinyGPS gps;
SoftwareSerial ss(GPS_RX,GPS_TX);

int buttonState = 0;

void setup() {
  pinMode(LED_LEFT , OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  lcd.begin(20, 4);
  lcd.setCursor(0,0);
  lcd.print("Initializing...");
  
  ss.begin(38400);
  Serial1.begin(38400);
  
  // Flash 3 times at startup
  /*for(int x = 0; x <= 5; x++) {
    toggleLed(LED_LEFT);
    toggleLed(LED_RIGHT);
    delay(200);
  }*/
  
  // Start library
  sensors.begin();
  //lcd.command(LCD_CLEARDISPLAY);
  lcd.clear();
}


void loop() {
  // call sensors.requestTemperatures() to issue a global temperature request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  getGpsData();

  /*Serial.print("Temp: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print("C  (");
  Serial.print(sensors.getTempFByIndex(0));
  Serial.println("F)");*/
  
  //checkSensor(SENSOR_LEFT , LED_LEFT);
  //checkSensor(SENSOR_RIGHT, LED_RIGHT);

  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  if(digitalRead(BUTTON)) {
    lcd.setCursor(5,0);
    lcd.print(sensors.getTempCByIndex(0));
    lcd.print("C");
  } else {
    lcd.setCursor(5,0);
    lcd.print(sensors.getTempFByIndex(0));
    lcd.print("F");
  }
  delay(150);
  
}


void getGpsData() {
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }
  
  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
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

