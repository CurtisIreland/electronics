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
  i = (i + 1) % 3;
  digitalWrite(latch, LOW);

  if (i != 0) {
    digitalWrite(serial, LOW);
  } else {
    digitalWrite(serial, HIGH);    
  }

  digitalWrite(sclk, HIGH);
  digitalWrite(sclk, LOW);
  digitalWrite(latch, HIGH);

  delay(1000);
}
