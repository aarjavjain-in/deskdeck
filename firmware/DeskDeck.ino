#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "USB.h"
#include "USBHIDConsumerControl.h"
#include "USBHIDKeyboard.h"

// Screen configuration (0.96" 128x64 OLED)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Hardware pin definitions
#define PIN_SW1 4
#define PIN_SW2 5
#define PIN_SW3 6
#define PIN_SW4 7

#define PIN_ENC_A 10
#define PIN_ENC_B 11
#define PIN_ENC_SW 12

#define I2C_SDA 8
#define I2C_SCL 9

// USB HID objects
USBHIDKeyboard Keyboard;
USBHIDConsumerControl ConsumerControl;

// State tracking
int lastEncStateA;
String lastAction = "Ready";

void updateDisplay(String statusText) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("--- DeskDeck S3 ---");
  
  display.setCursor(0, 20);
  display.print("Status: ");
  display.println(statusText);
  
  display.setCursor(0, 42);
  display.println("[1] Dev   [2] Mute");
  display.println("[3] Snip  [4] Desk");
  
  display.display();
}

void setup() {
  // Mechanical switches with internal pullups
  pinMode(PIN_SW1, INPUT_PULLUP);
  pinMode(PIN_SW2, INPUT_PULLUP);
  pinMode(PIN_SW3, INPUT_PULLUP);
  pinMode(PIN_SW4, INPUT_PULLUP);

  // Rotary encoder pins
  pinMode(PIN_ENC_A, INPUT_PULLUP);
  pinMode(PIN_ENC_B, INPUT_PULLUP);
  pinMode(PIN_ENC_SW, INPUT_PULLUP);
  lastEncStateA = digitalRead(PIN_ENC_A);

  // Initialize USB HID
  Keyboard.begin();
  ConsumerControl.begin();
  USB.begin();

  // Initialize I2C and OLED
  Wire.begin(I2C_SDA, I2C_SCL);
  if (display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    updateDisplay("Ready");
  }
}

void loop() {
  // Key 1: Open Terminal / Dev Shortcut (Win + R -> cmd)
  if (digitalRead(PIN_SW1) == LOW) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    Keyboard.releaseAll();
    delay(100);
    Keyboard.println("cmd");
    updateDisplay("Terminal Opened");
    delay(300);
  }

  // Key 2: Toggle Discord / Global Mute (Ctrl + Shift + M)
  if (digitalRead(PIN_SW2) == LOW) {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('m');
    Keyboard.releaseAll();
    updateDisplay("Mic Toggled");
    delay(300);
  }

  // Key 3: Windows Snip Tool (Win + Shift + S)
  if (digitalRead(PIN_SW3) == LOW) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('s');
    Keyboard.releaseAll();
    updateDisplay("Screenshot Snipped");
    delay(300);
  }

  // Key 4: Switch Desktop (Ctrl + Win + Right Arrow)
  if (digitalRead(PIN_SW4) == LOW) {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_RIGHT_ARROW);
    Keyboard.releaseAll();
    updateDisplay("Next Workspace");
    delay(300);
  }

  // Rotary encoder volume dial
  int currentEncStateA = digitalRead(PIN_ENC_A);
  if (currentEncStateA != lastEncStateA && currentEncStateA == 1) {
    if (digitalRead(PIN_ENC_B) != currentEncStateA) {
      ConsumerControl.press(CONSUMER_CONTROL_VOLUME_INCREMENT);
      ConsumerControl.release();
      updateDisplay("Volume Up");
    } else {
      ConsumerControl.press(CONSUMER_CONTROL_VOLUME_DECREMENT);
      ConsumerControl.release();
      updateDisplay("Volume Down");
    }
  }
  lastEncStateA = currentEncStateA;

  // Knob center click (Play / Pause toggle)
  if (digitalRead(PIN_ENC_SW) == LOW) {
    ConsumerControl.press(CONSUMER_CONTROL_PLAY_PAUSE);
    ConsumerControl.release();
    updateDisplay("Play / Pause");
    delay(300);
  }

  delay(5);
}
