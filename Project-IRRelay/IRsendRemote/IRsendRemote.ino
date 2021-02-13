#include <IRremote.h>
#include <WiFi.h>
#include <PubSubClient.h>

IRsend IrSender;

const char* ssid = "BasementElectronics";
const char* password = "AmateurHour";
const char* mqtt_server = "vm-apps01.in.in-works.net";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String messageIR;

  for (int i = 0; i < length; i++) {
    messageIR += (char)payload[i];
  }

//  Serial.print("As a proper string: ");
//  Serial.println(msg);
//  Serial.println(atol(msg), HEX);
  IrSender.sendNECStandard(messageIR.toInt(), 32, 2);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("irSignal");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
    Serial.begin(115200);

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__));
    
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    Serial.print(F("Ready to send IR signals at pin "));
    Serial.println(IR_SEND_PIN);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
