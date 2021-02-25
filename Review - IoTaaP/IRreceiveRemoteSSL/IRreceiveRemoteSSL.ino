/*****************************************
 * This application uses IoTaaP Cloud 
 * for its MQTTS service
 * 
 * Sign up at https://console.iotaap.io/ 
 * to create an account and MQTTS service
 * 
 * This uses IRremote library version 2.8.0.
 * Later version will not work
 */

#include <IRremote.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

int IR_RECEIVE_PIN = 15;
IRrecv IrReceiver(IR_RECEIVE_PIN);

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASS";
const char* mqtt_server = "";
const int   mqtt_port = 8883;
const char* mqtt_user = "";
const char* mqtt_pass = "";

WiFiClientSecure espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

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

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("connected");
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
    client.setServer(mqtt_server, mqtt_port);

    // In case the interrupt driver crashes on setup, give a clue
    // to the user what's going on.
    Serial.println("Enabling IRin");
    IrReceiver.enableIRIn();  // Start the receiver

    Serial.print(F("Ready to receive IR signals at pin "));
    Serial.println(IR_RECEIVE_PIN);
}

void loop() {
    decode_results results;
    unsigned long irSignal;
    if (!client.connected()) {
      reconnect();
    }
    client.loop();

    if (IrReceiver.decode(&results)) {
        char gen_topic[25];
        Serial.println();
        irSignal = results.value;
        sprintf(gen_topic, "/%s/irSignal", mqtt_user);
        snprintf (msg, MSG_BUFFER_SIZE, "%d", irSignal);
        client.publish(gen_topic, msg);

        Serial.print("IR Code: ");
        Serial.println(irSignal);

        IrReceiver.resume(); // Receive the next value
    }
    delay(100);
}
