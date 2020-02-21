#include <Arduino.h>

#include <WiFi.h>
#include <Update.h>
#include <WebServer.h>

#include <HTTPClient.h>
#include <ArduinoJson.h>

int number = 0; //declear the variables
int first_digit = 0;
int second_digit = 0;
int third_digit = 0;
int fourth_digit = 0;
int timer_event = 0;
int CA_1 = 18;
int CA_2 = 19;
int CA_3 = 25;
int CA_4 = 26;
int clk = 27;
int latch = 14;
int data = 12;
int count = 0;
int digits[4] ;
int CAS[4] = {18, 19, 25, 26};
byte numbers[10] {B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110};
//byte combinations for each number 0-9

/* */
// const char* ssid = "BasementElectronics";
// const char* password = "AmateurHour";
const char* ssid = "IN-BB_RPT";
const char* password = "VisualSynthesis";
const char* ca = \ 
"-----BEGIN CERTIFICATE-----\n" \  
"MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\n" \  
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \  
"DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n" \  
"SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\n" \  
"GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\n" \  
"AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\n" \  
"q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\n" \  
"SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\n" \  
"Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\n" \  
"a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\n" \  
"/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\n" \  
"AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\n" \  
"CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\n" \  
"bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\n" \  
"c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\n" \  
"VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\n" \  
"ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\n" \  
"MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\n" \  
"Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\n" \  
"AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\n" \  
"uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\n" \  
"wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\n" \  
"X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\n" \  
"PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\n" \  
"KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\n" \  
"-----END CERTIFICATE-----\n";
/* */
WebServer server(80);
 
/*
 * Server Index Page
 */
 
const char* serverIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')" 
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

int getSubs() {
  int subCount = 0;
  HTTPClient http;
  http.begin("https://www.googleapis.com/youtube/v3/channels?part=statistics&id=UCpzhAgl4pjwTiSOHJIQeXhQ&key=AIzaSyDsARt3plel8YNDUp13EnXAzr2RGLFspiw", ca); //HTTP
  int httpCode = http.GET();

  if(httpCode > 0) {
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
//    Serial.println(payload);

      const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + 2*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 400;
      DynamicJsonDocument doc(capacity);

      deserializeJson(doc, payload);

//      const char* kind = doc["kind"]; // "youtube#channelListResponse"
//      const char* etag = doc["etag"]; // "\"Fznwjl6JEQdo1MGvHOGaz_YanRU/oImXgqM2klh8VFN6BFDmCiywkVU\""

//      int pageInfo_totalResults = doc["pageInfo"]["totalResults"]; // 1
//      int pageInfo_resultsPerPage = doc["pageInfo"]["resultsPerPage"]; // 1

//      JsonObject items_0 = doc["items"][0];
//      const char* items_0_kind = items_0["kind"]; // "youtube#channel"
//      const char* items_0_etag = items_0["etag"]; // "\"Fznwjl6JEQdo1MGvHOGaz_YanRU/9ELn5ydX28E9dMTkLoTlR9LR2QM\""
//      const char* items_0_id = items_0["id"]; // "UCpzhAgl4pjwTiSOHJIQeXhQ"

//      JsonObject items_0_statistics = items_0["statistics"];
//      const char* items_0_statistics_viewCount = items_0_statistics["viewCount"]; // "535356"
//      const char* items_0_statistics_commentCount = items_0_statistics["commentCount"]; // "0"
//      const char* items_0_statistics_subscriberCount = items_0_statistics["subscriberCount"]; // "2270"
//      bool items_0_statistics_hiddenSubscriberCount = items_0_statistics["hiddenSubscriberCount"]; // false
//      const char* items_0_statistics_videoCount = items_0_statistics["videoCount"]; // "163"
//      subCount = atoi(items_0_statistics_subscriberCount);
      subCount = atoi(doc["items"][0]["statistics"]["subscriberCount"]);
      Serial.print("Subscription count: ");
      Serial.println(subCount);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    delay(5000);
    ESP.restart();
  }
  http.end();
  return(subCount);
}

void setup() {
  Serial.begin(115200); //serial start and pin config
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();

  pinMode(CA_1, OUTPUT);
  pinMode(CA_2, OUTPUT);
  pinMode(CA_3, OUTPUT);
  pinMode(CA_4, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(data, OUTPUT);
  digitalWrite(CA_1, HIGH);
  digitalWrite(CA_2, HIGH);
  digitalWrite(CA_3, HIGH);
  digitalWrite(CA_4, HIGH);
}

void loop() {
  number = getSubs();
//  number = 1234;
  cathode_high(); // blank the screen
  if (number > 9999) { //check the number is 0-9999
    number = 0;
  }
  break_number(number);

  for(int i = 0; i < 720000; i++) {
    server.handleClient();
    display_number();
    delay(5);
  }
}

void break_number(long num) { // seperate the input number into 4 single digits

  first_digit = num / 1000;
  digits[0] = first_digit;

  int first_left = num - (first_digit * 1000);
  second_digit = first_left / 100;
  digits[1] = second_digit;
  int second_left = first_left - (second_digit * 100);
  third_digit = second_left / 10;
  digits[2] = third_digit;
  fourth_digit = second_left - (third_digit * 10);
  digits[3] = fourth_digit;
}

void display_number() { //scanning

  cathode_high(); //black screen
  digitalWrite(latch, LOW); //put the shift register to read
  shiftOut(data, clk, LSBFIRST, numbers[digits[count]]); //send the data
  digitalWrite(CAS[count], LOW); //turn on the relevent digit
  digitalWrite(latch, HIGH); //put the shift register to write mode
  count++; //count up the digit
  if (count == 4) { // keep the count between 0-3
    count = 0;
  }
}

void cathode_high() { //turn off all 4 digits
  digitalWrite(CA_1, HIGH);
  digitalWrite(CA_2, HIGH);
  digitalWrite(CA_3, HIGH);
  digitalWrite(CA_4, HIGH);
}
