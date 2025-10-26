/*
  =====================================================
  Title   : Task A — LED Mode Controller (Final 3 LEDs)
  Author  : Malaika batool
  Reg No. : 23-NTU-CS-1044
  Date    : 26/10/2025
  Description:
    Button A (GPIO14): Cycles LED modes
      1. Both OFF
      2. Alternate Blink
      3. Both ON
      4. Fade (LED1 only)
    Button B (GPIO27): Resets mode
    OLED (GPIO21 SDA, 22 SCL): Displays mode info
  =====================================================
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define SDA_PIN 21
#define SCL_PIN 22
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Pin setup ---
#define LED1 16  // fade
#define LED2 17  // blink
#define LED3 18  // on/off
#define BUTTON_A 14
#define BUTTON_B 27

int mode = 0;
unsigned long lastPress = 0;

void showMode() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.print("Mode: ");
  if (mode == 0) display.println("BOTH OFF");
  if (mode == 1) display.println("ALT BLINK");
  if (mode == 2) display.println("BOTH ON");
  if (mode == 3) display.println("FADE");
  display.display();
}

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  showMode();
}

void loop() {
  if (digitalRead(BUTTON_A) == LOW && millis() - lastPress > 300) {
    mode = (mode + 1) % 4;
    showMode();
    lastPress = millis();
  }
  if (digitalRead(BUTTON_B) == LOW && millis() - lastPress > 300) {
    mode = 0;
    showMode();
    lastPress = millis();
  }

  static int fade = 0;
  static int dir = 1;

  switch (mode) {
    case 0: // both off
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      break;

    case 1: // alternate blink
      digitalWrite(LED2, millis() / 500 % 2);
      digitalWrite(LED3, !digitalRead(LED2));
      digitalWrite(LED1, LOW);
      break;

    case 2: // both on
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      break;

    case 3: // fade on LED1
      fade += dir * 10;
      if (fade <= 0 || fade >= 255) dir = -dir;
      analogWrite(LED1, fade);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      delay(10);
      break;
  }
}
