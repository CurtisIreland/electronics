int serial = 13;
int sclk = 11;
int latch = 12;

int i = 0;

void setup() { 
  pinMode(serial, OUTPUT);
  pinMode(sclk, OUTPUT);
  pinMode(latch, OUTPUT);

  digitalWrite(sclk, LOW);
  digitalWrite(latch, LOW);

  shiftOut(serial, sclk, MSBFIRST, B0);
  digitalWrite(latch, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

}
