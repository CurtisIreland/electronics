#define RED_LED 12
#define GREEN_LED 11
#define BLUE_LED 10
#define BUTTON_PIN 2

int choose_led = 0;
volatile int state = LOW;

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  
  attachInterrupt(0, blink, CHANGE);
}

void loop() {
  if (choose_led == 0) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }
  
  choose_led = (choose_led + 1) % 2;
  delay(500);
}

void blink()
{
  state = !state;
  digitalWrite(BLUE_LED, state);
}
