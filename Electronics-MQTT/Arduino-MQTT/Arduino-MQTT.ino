#include <SPI.h>
#include <Servo.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#define SERVO_CONTROL 6
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
Servo servo;
EthernetClient ethClient;
// mqtt configuration
const char* server = "192.168.5.10";
const char* topic = "example/pot";
const char* clientName = "com.be.arduino";
PubSubClient client(ethClient);
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] angle:");
    String data;
    for (int i = 0; i < length; i++) {
        data += (char)payload[i];
    }
    double angle = ((data.toInt() * 180) / 100);
    constrain(angle, 0, 180);
    servo.write((int) angle);
    Serial.println((int) angle);
}
void mqttReConnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(clientName)) {
            Serial.println("connected");
            client.subscribe(topic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}
void setup()
{
    Serial.begin(9600);
    client.setServer(server, 1883);
    client.setCallback(callback);
    servo.attach(SERVO_CONTROL);
    if (Ethernet.begin(mac) == 0) {
        Serial.println("Failed to configure Ethernet using DHCP");
    }
    delay(1500); // Allow the hardware to sort itself out
}
void loop()
{
    if (!client.connected()) {
        mqttReConnect();
    }
    client.loop();
}
