#include <dht_nonblocking.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "reyax_vars.h"

/* Uncomment according to your sensortype. */
//#define DHT_SENSOR_TYPE DHT_TYPE_11
//#define DHT_SENSOR_TYPE DHT_TYPE_21
#define DHT_SENSOR_TYPE DHT_TYPE_22

static const int DHT_SENSOR_PIN = 4;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

WiFiClient espClient;
PubSubClient client(espClient);

#define MSG_BUFFER_SIZE  (512)
char msg[MSG_BUFFER_SIZE];

const char* ssid = "BasementElectronics";
const char* password = "AmateurHour";

const String reyax_client = reyax_user + "0002";

const char* msg_format = "{\"action\": \"notification/insert\", \"deviceId\": \"%s\", \"notification\": {\"notification\": \"dht\", \"parameters\": {\"temperature\": \"%.1f\",\"humidity\": \"%.1f\"}}}";

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
    // Attempt to connect
    if (client.connect(reyax_client.c_str(), reyax_user.c_str(), reyax_pass.c_str())) {
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

void setup( )
{
  Serial.begin( 115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setBufferSize(1024);
  client.setKeepAlive(60);
}

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every five seconds. */
  if( millis( ) - measurement_timestamp > 5000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

void loop( )
{
  float temperature;
  float humidity;

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if( measure_environment( &temperature, &humidity ) == true )
  {
    snprintf (msg, MSG_BUFFER_SIZE, msg_format, reyax_device.c_str(), temperature, humidity);
    client.publish("api/request", msg);
    Serial.print("MQTT Message: ");
    Serial.println(msg);

    
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, H = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" );
  }
}
