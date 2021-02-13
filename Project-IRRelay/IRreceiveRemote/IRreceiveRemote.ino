#include <IRremote.h>
#include <WiFi.h>
#include <PubSubClient.h>

int IR_RECEIVE_PIN = 15;
IRrecv IrReceiver(IR_RECEIVE_PIN);

const char* ssid = "BasementElectronics";
const char* password = "AmateurHour";
const char* mqtt_server = "vm-apps01.in.in-works.net";

WiFiClient espClient;
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
    if (client.connect(clientId.c_str())) {
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
    client.setServer(mqtt_server, 1883);

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
        Serial.println();
        irSignal = results.value;
        snprintf (msg, MSG_BUFFER_SIZE, "%d", irSignal);
        client.publish("irSignal", msg);

        IrReceiver.resume(); // Receive the next value
    }
    delay(100);
}
