#include <Arduino_LED_Matrix.h>

uint8_t logo[104] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,2,2,2,0,0,0,2,2,2,0,0,
    0,2,0,0,0,2,0,2,0,0,0,2,0,
    2,0,0,0,0,0,2,0,0,3,0,0,2,
    2,0,3,3,3,0,2,0,3,3,3,0,2,
    2,0,0,0,0,0,2,0,0,3,0,0,2,
    0,2,0,0,0,2,0,2,0,0,0,2,0,
    0,0,2,2,2,0,0,0,2,2,2,0,0
};

Arduino_LED_Matrix matrix;

void setup() {
  matrix.begin();
  // display the image
  matrix.setGrayscaleBits(2);
  matrix.draw(logo);

}

void loop() {

}