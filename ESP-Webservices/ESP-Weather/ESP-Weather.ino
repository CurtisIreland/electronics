#include <WiFi.h>

#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <LiquidCrystal.h>

const char* ssid = "WIFI_SSID";
const char* password = "WiFi_Password";

String api_url = "http://dataservice.accuweather.com/currentconditions/v1/";
String api_location = "55488"; // Toronto

String api_key = "<insert API key here>";
String api_details = "true";

String web_url = api_url + api_location + "?apikey=" + api_key + "&details=" + api_details;

const int RS = 5, EN = 18, d4 = 19, d5 = 21, d6 = 22, d7 = 23;   
LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);

void setup() {
  Serial.begin(115200); //serial start and pin config
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  lcd.begin(16, 2);
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print("Getting data...");
}

void loop() {
  HTTPClient http;
  http.begin(web_url);
  int httpCode = http.GET();

  if(httpCode > 0) {
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();

      Serial.println(payload);

      const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 33*JSON_OBJECT_SIZE(2) + 58*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(8) + JSON_OBJECT_SIZE(31) + 2940;
      DynamicJsonDocument doc(capacity);      

      deserializeJson(doc, payload);

      String json_temp = doc[0]["Temperature"]["Metric"]["Value"];
      String json_tempunt = doc[0]["Temperature"]["Metric"]["Unit"];
      String json_winddir = doc[0]["Wind"]["Direction"]["Localized"];
      String json_windspd = doc[0]["Wind"]["Speed"]["Metric"]["Value"];
      String json_windspdunt = doc[0]["Wind"]["Speed"]["Metric"]["Unit"];

      Serial.print("Temp: ");
      Serial.print(json_temp );
      Serial.println(" C");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("T: " + json_temp + " " + json_tempunt);
      lcd.setCursor(0, 1);
      lcd.print("W: " + json_windspd + " " + json_windspdunt + " " + json_winddir);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

  delay(3600000); // Sleep for an hour
}
