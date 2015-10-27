const int analogInPin = A1;

int sensorValue = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(analogInPin);

  // print the results to the serial monitor:
  Serial.print("sensor = " );
  Serial.println(sensorValue);
  delay(250);
}
