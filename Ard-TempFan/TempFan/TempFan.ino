#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

boolean fan_on = false;

#define ENABLE 5
#define DIRA 3
#define DIRB 4

void setup() {
  Serial.begin( 9600);
  
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);

  digitalWrite(DIRA,HIGH); //one way
  digitalWrite(DIRB,LOW);
  digitalWrite(ENABLE,LOW); // enable off
}

/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

void loop() {
  float temperature;
  float humidity;

  if( measure_environment( &temperature, &humidity ) == true )
  {
    if (temperature > 29.0) {
      digitalWrite(ENABLE,HIGH);
      if(! fan_on) {
        Serial.println("High temperature - turn on fan");
        fan_on = true;
      }
    } else {
      digitalWrite(ENABLE,LOW);
      if(fan_on) {
        Serial.println("Low temperature - turn off fan");
        fan_on = false;
      }
    }
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, H = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" );

  }
}
