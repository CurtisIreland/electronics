const int logicSwitchPin = 2;
const int ledOrOutPin = 3;
const int ledAndOutPin = 4;
const int ledXorOutPin = 5;

const int inputXPin = 11;
const int inputYPin = 12;
const int outputPin = 13;

// mode: 0 - OR, 1 - AND, 2 - XOR
int mode = 0;
int x = LOW;
int y = LOW;
int output = LOW;

void setup() {
  pinMode(logicSwitchPin, INPUT);
  pinMode(ledOrOutPin, OUTPUT);
  pinMode(ledAndOutPin, OUTPUT);
  pinMode(ledXorOutPin, OUTPUT);

  pinMode(inputXPin, INPUT);
  pinMode(inputYPin, INPUT);
  pinMode(outputPin, OUTPUT);
}

void loop() {
  x = digitalRead(inputXPin);
  y = digitalRead(inputYPin);

  switch (mode) {
    case 0: // x OR y
      digitalWrite(ledOrOutPin, HIGH);
      digitalWrite(ledAndOutPin, LOW);
      digitalWrite(ledXorOutPin, LOW);

      output = (x || y);
      digitalWrite(outputPin, output);
      break;
    case 1: // x AND y
      digitalWrite(ledOrOutPin, LOW);
      digitalWrite(ledAndOutPin, HIGH);
      digitalWrite(ledXorOutPin, LOW);

      output = (x && y);
      digitalWrite(outputPin, output);
      break;
    case 2: // x XOR y
      digitalWrite(ledOrOutPin, LOW);
      digitalWrite(ledAndOutPin, LOW);
      digitalWrite(ledXorOutPin, HIGH);

      output = ((x || y) && !(x && y));
      digitalWrite(outputPin, output);
      break;
    default:
      digitalWrite(ledOrOutPin, LOW);
      digitalWrite(ledAndOutPin, LOW);
      digitalWrite(ledXorOutPin, LOW);    
  }
  
  if (digitalRead(logicSwitchPin)) {
    mode = (mode + 1) % 3;
  }
  
  delay(250);
}
