byte state = LOW;
uint8_t testpin = 12;

void setup() {
  // put your setup code here, to run once:
	pinMode(testpin, OUTPUT);
	digitalWrite(testpin, state);
}

void loop() {
  // put your main code here, to run repeatedly:
  state = ! state;
	digitalWrite(testpin, state);
//  delay(5);
}
