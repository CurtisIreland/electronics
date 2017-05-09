int serial = 13;
int sclk = 11;
int latch = 12;

void setup() {
  pinMode(serial, OUTPUT);
  pinMode(sclk, OUTPUT);
  pinMode(latch, OUTPUT);

  digitalWrite(sclk, LOW);
  digitalWrite(latch, LOW);

  shiftOut(serial, sclk, MSBFIRST, B0);
  digitalWrite(latch, HIGH);

  delay(1000);

  digitalWrite(latch, LOW);
  digitalWrite(serial, HIGH);
  digitalWrite(sclk, HIGH);
  digitalWrite(sclk, LOW);

  digitalWrite(latch, HIGH);

  delay(1000);

  digitalWrite(latch, LOW);
  digitalWrite(serial, LOW);
  digitalWrite(sclk, HIGH);
  digitalWrite(sclk, LOW);

  digitalWrite(latch, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

}
