#include "GameLevel.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <Arduino.h>
#include <Wire.h>

// 0X3C is the most common I2C address for these 0.96" OLEDs
#define I2C_ADDRESS 0x3C

SSD1306AsciiWire oled;

GameLevel game;
uint8_t level = 1;
bool pressed = false;
bool playing = false;
bool isMatch = true;
bool checkEnabled = false; // to start checking if user input is given
uint8_t playing_iterator = 0;
bool push_button_1_last_state = LOW;

void lightUpLed500ms(uint8_t led) {
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
}

// For plain text only
void writeOled(const __FlashStringHelper* message) {
  oled.clear();
  oled.set2X();
  oled.setCursor(0, 0);
  oled.print(message);
}

// For text + a number (Level)
void writeOled(const __FlashStringHelper* message, uint8_t value) {
  oled.clear();
  oled.set2X();
  oled.setCursor(0, 0);
  oled.print(message);
  oled.print(value); 
}

void setup() {
  Wire.begin();
  Wire.setClock(400000L);

  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(Stang5x7); 
  oled.displayRemap(1);
  writeOled(F("READY "));

  delay(100);
  pinMode(PUSH_BUTTON, INPUT);
  pinMode(PUSH_RED_BUTTON, INPUT);
  pinMode(PUSH_GREEN_BUTTON, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
}

void loop() {

  bool push_button_1_current_state = digitalRead(PUSH_BUTTON);

  if (!playing && push_button_1_current_state == HIGH &&
      push_button_1_last_state == LOW) {
    writeOled(F("Starting \n \n Level "), level);
    // start level button
    game.generateLevel(level);
    game.play();
    playing = true;
  }

  push_button_1_last_state = push_button_1_current_state;

  if (playing) {

    bool push_play_input_red = digitalRead(PUSH_RED_BUTTON);
    bool push_play_input_green = digitalRead(PUSH_GREEN_BUTTON);

    if (push_play_input_red == HIGH) {
      USER_INPUT[playing_iterator] = RED_LED;
      playing_iterator++;
      lightUpLed500ms(RED_LED);
      checkEnabled = true;
    }
    if (push_play_input_green == HIGH) {
      USER_INPUT[playing_iterator] = GREEN_LED;
      playing_iterator++;
      lightUpLed500ms(GREEN_LED);
      checkEnabled = true;
    }

    if (playing_iterator > 0 && checkEnabled) {
      isMatch = game.checkUserInput();
      checkEnabled = false;
    }

    if (!isMatch) {
      playing = false;
      playing_iterator = 0;
      isMatch = true;
      writeOled(F("LOSE \n :("));
    }

    if (playing_iterator == level && isMatch) {
      playing = false;
      playing_iterator = 0;
      level++;
      game.reset();

      writeOled(F("YOU WIN"));
    }
  }

  delay(100);
}
