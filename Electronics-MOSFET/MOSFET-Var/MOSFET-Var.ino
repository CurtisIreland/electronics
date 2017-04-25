const int analogInPin = A0;
const int mosGatePin = 3;

int inValue = 0;
int outValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(mosGatePin, OUTPUT);
}

void loop() {
  inValue = analogRead(analogInPin);
  outValue = map(inValue, 0, 1023, 0, 255);
  analogWrite(mosGatePin, outValue);

  // print the results to the serial monitor:
  Serial.print("input = " );
  Serial.print(inValue);
  Serial.print("\t MOSFET = ");
  Serial.println(outValue);
  delay(250);
}
