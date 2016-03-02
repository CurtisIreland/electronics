int led_array[] = { 10, 11, 12, 13 };
int led_count = 4;
volatile int led_pos = 0;

int button_pin = 2;

void setup() {
  for (int this_led = 0; this_led < led_count; this_led++) {
    pinMode(led_array[this_led], OUTPUT);
    digitalWrite(led_array[this_led], LOW);
  }

  pinMode(button_pin, INPUT);
  attachInterrupt(0, change, FALLING);
}

void loop() {
  for (int this_led = 0; this_led < led_count; this_led++) {
    if( this_led == led_pos ) {
      digitalWrite(led_array[this_led], HIGH);
    } else {
      digitalWrite(led_array[this_led], LOW);
    }
  }
}

void change() {
  led_pos = (led_pos + 1) % 4;
}

