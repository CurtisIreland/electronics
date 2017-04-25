const int nMOS = 3;

void setup() {
  pinMode(nMOS, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(nMOS, HIGH);
  digitalWrite(13, HIGH);
  delay(750);
  digitalWrite(nMOS, LOW);
  digitalWrite(13, LOW);
  delay(750);
}
