#include "GameLevel.h"

const uint8_t LED_PINS[] = {RED_LED, GREEN_LED};
const uint8_t LED_TOTAL = 2;
uint8_t USER_INPUT[20];

void GameLevel::generateLevel(int difficulty) {
  if (difficulty > max_difficulty) {
    difficulty = max_difficulty;
  }
  current_diff = difficulty;

  for (int i = 0; i < difficulty; i++) {
    uint8_t random_index = random(0, LED_TOTAL);
    steps[i] = LED_PINS[random_index];
  }

  current_checking_index = 0;
}

void GameLevel::play() {
  for (int i = 0; i < current_diff; i++) {
    Serial.println(steps[i]);
    digitalWrite(steps[i], HIGH);
    delay(1000);
    digitalWrite(steps[i], LOW);
    delay(1000);
  }
}

bool GameLevel::checkUserInput() {

  if (steps[current_checking_index] != USER_INPUT[current_checking_index]) {
    current_checking_index = 0;
    return false;
  }
  current_checking_index++;
  return true;
}
void GameLevel::reset() { current_checking_index = 0; }