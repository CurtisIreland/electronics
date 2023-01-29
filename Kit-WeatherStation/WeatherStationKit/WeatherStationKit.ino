/*********
  Gabor Mate
  https://github.com/Mategm
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BH1750.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
 
BH1750 lightMeter;
Adafruit_BMP085 pressure;
double baseline; // For the pressure

#define ALTITUDE 87.0 // Altitude of Sarszentmiklos, Hungary, Central-Europe in meters

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins) SDA = D2; SCL = D1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 14     // Digital pin connected to the DHT sensor as D5
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Its works for my 128x64 OLED 0,96" screen
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  Wire.begin();
  dht.begin();
  
  lightMeter.begin();
  lightMeter.configure(BH1750::CONTINUOUS_HIGH_RES_MODE_2);

  display.setTextSize(1); //1-8; 1 is default, not needed
  display.setTextColor(WHITE); // Not relevant for one or two color OLED display but needed for the code
  display.setCursor(0,0); //0,0 to 120,57
  display.println("Weather Station by!");
  display.println("Mategm!");
  display.display();

  if (pressure.begin()){
    //Serial.println("BMP180 init success");
    }
  else{
    Serial.println("BMP180 init fail\n\n");
    delay(5000);
    while(1); // Pause forever.
  }
  delay(2000);
  display.clearDisplay();
}
void loop() {
  char status;
  double T,P,p0,a;
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Read temperature as Fahrenheit (isFahrenheit = true; default C as false)
  float l = lightMeter.readLightLevel();

  // pressure
  p0 = pressure.readSealevelPressure(ALTITUDE);
/***
  status = pressure.startTemperature();
  delay(status);
  if (status != 0) {
    delay(status); // Wait for the measurement to complete:
    status = pressure.getTemperature(T);
    if (status != 0) {      
      status = pressure.startPressure(3);
      if (status != 0) {
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0) {
          p0 = pressure.sealevel(P,ALTITUDE); // we're at 113 meters (Sarszentmiklos, Hungary)
        }
      }
    }
  }
***/
   //Convert to string and cut de second decimal numbers
  String temp = String(t,1); 
  String humi = String(h,1);
  String lux  = String(l,1);
  String pres = String(p0,1);
  
  
  // Calculate lux and pressure position according to its number length
  int pad1 = (lux.length() * 6) + 19;
  int pad2 = (pres.length() *6) + 19;

  // clear the display
  display.clearDisplay();

  // display temperature
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.setCursor(85,0);
  display.print(temp);
  display.drawCircle(115, 1, 1, WHITE); // degree sign
  display.setCursor(118,0); // repositioning the Celsius display
  display.print("C");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.print("Humidity:   ");
  display.setCursor(85,10);
  display.print(humi);
  display.print(" ");
  display.print("%"); 

  // display light data
  display.setTextSize(1);
  display.setCursor(0,20);
  display.print("Illuminance: ");
  display.setCursor((SCREEN_WIDTH - pad1),20); 
  display.print(lux);
  display.print(" lx");

  // pressure
  display.setCursor(0,30);
  display.setTextSize(1);
  display.print("Pressure: ");
  display.setCursor((SCREEN_WIDTH - pad2),30); 
  display.print(pres);
  display.print(" P");
  
  display.display();
  delay(5000); // display the values for 5 seconds before update
}
