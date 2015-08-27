#define LED_PIN 12
#define BUTTON_PIN 2

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  
  digitalWrite(LED_PIN, digitalRead(BUTTON_PIN));
}

void loop() {
  digitalWrite(LED_PIN, digitalRead(BUTTON_PIN));
}
