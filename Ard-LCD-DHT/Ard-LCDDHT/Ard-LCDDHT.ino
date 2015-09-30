// include the library code:
#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Initialize the DHT sensor
  dht.begin();

  // Print a message to the LCD.
  lcd.print("Initializing");
  lcd.setCursor(0, 1);
  lcd.print("Sensor");
  lcd.blink();
}

void loop() {
  // Wait a couple seconde between measurements
  delay(2000);
  
  // Get the sensor data
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // Turn off cursor blink and clear the LCD
  lcd.noBlink();
  lcd.clear();
  
  if (isnan(h) || isnan(t)) {
    lcd.print("Sensor Failure");
    return;
  }

  // Print the temperature on the first line
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" C");

  // Print the humidity on the second line
  lcd.setCursor(0, 1);
  lcd.print("Humid: ");
  lcd.print(h);
  lcd.print("%");
}

