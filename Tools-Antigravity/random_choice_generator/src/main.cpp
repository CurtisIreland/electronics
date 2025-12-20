#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

// Initialize TFT
TFT_eSPI tft = TFT_eSPI(); 

// Button Pins (Standard T-Display)
#define BTN_1 0
#define BTN_2 35

// Choices
const char* choices[] = {
    "Electronics", "Play a Game", "Development", "Touch Grass"
};
const int numChoices = sizeof(choices) / sizeof(choices[0]);

// State
bool spinning = false;

void drawCenteredText(const char* text, int y) {
    int len = strlen(text);
    // Estimated width per char: Size 4 ~= 24px, Size 3 ~= 18px, Size 2 ~= 12px
    // Screen width 240.
    // "Electronics" (11) * 24 = 264 (>240!) -> Use Size 3 (198px)
    // "Development" (11) -> Size 3
    // "Play a Game" (11) -> Size 3
    // "Touch Grass" (11) -> Size 3
    
    // Simple dynamic sizing
    if (len > 10) tft.setTextSize(3);
    else tft.setTextSize(4);
    
    tft.drawString(text, 120, y);
}

void setup() {
    Serial.begin(115200);
    
    // Init Buttons
    pinMode(BTN_1, INPUT_PULLUP); // Usually GPIO0 is pulled up, but good practice
    pinMode(BTN_2, INPUT);        // GPIO35 is input only
                                  
    // Init Display
    tft.init();
    tft.setRotation(1); // Landscape
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM); // Middle Center
    
    // Title
    tft.setTextSize(2);
    tft.drawString("Random Choice", 120, 30);
    
    tft.setTextSize(3);
    tft.drawString("Push Btn 1", 120, 120);
    
    // Seed random from uninitialized pin
    randomSeed(analogRead(37)); 
}

void showResult(const char* text, uint16_t color) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(color, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    drawCenteredText(text, 120);
    
    // Instructions to reset
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.setTextSize(1);
    tft.drawString("Btn 2 to Reset", 120, 220);
}

void spinWheel() {
    int loops = 20;
    int delayTime = 50;
    
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(4);
    
    for (int i = 0; i < loops; i++) {
        int idx = random(numChoices);
        
        // Random colors
        uint16_t color = random(0xFFFF);
        tft.setTextColor(color, TFT_BLACK);
        
        // Draw centered
        tft.fillRect(0, 80, 240, 80, TFT_BLACK); // Clear text area
        drawCenteredText(choices[idx], 120);
        
        delay(delayTime);
        delayTime += 10; // Slow down effect
    }
    
    // Final result
    int finalIdx = random(numChoices);
    showResult(choices[finalIdx], TFT_GREEN);
}

void resetUI() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Random Choice", 120, 30);
    tft.setTextSize(3);
    tft.drawString("Push Btn 1", 120, 120);
}

void loop() {
    // Check buttons (Active Low)
    if (digitalRead(BTN_1) == LOW) {
        delay(50); // Debounce
        if (digitalRead(BTN_1) == LOW) {
            spinWheel();
            // Wait for release
            while(digitalRead(BTN_1) == LOW) delay(10);
        }
    }
    
    if (digitalRead(BTN_2) == LOW) {
        delay(50);
        if (digitalRead(BTN_2) == LOW) {
            resetUI();
            while(digitalRead(BTN_2) == LOW) delay(10);
        }
    }
}
