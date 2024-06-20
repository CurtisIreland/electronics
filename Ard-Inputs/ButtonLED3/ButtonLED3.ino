#define RED_LED 8
#define GREEN_LED 9
#define BLUE_LED 10
#define BUTTON_PIN 2

int interval=200;
unsigned long previousMillis=0;
unsigned long currentMillis=0;

int choose_led = 0;

void change_led() {
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval){
    if (choose_led == 0) {
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      Serial.println("RED");
    } else if (choose_led == 1) {
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, LOW);
      Serial.println("GREEN");
    } else if (choose_led == 2) {
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, HIGH);
      Serial.println("BLUE");
    }

    choose_led = (choose_led + 1) % 3;
    previousMillis = millis();
  }
}

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);

  Serial.begin(115200);
  attachInterrupt(0, change_led, FALLING);
}
  

void loop() {
  
}
