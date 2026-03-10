#ifndef GAME_LEVEL_H  // Header Guard: Start
#define GAME_LEVEL_H

#include <Arduino.h>

static const uint8_t RED_LED = 4;
static const uint8_t GREEN_LED = 3;
static const uint8_t PUSH_BUTTON = 6;
static const uint8_t PUSH_RED_BUTTON = 13;
static const uint8_t PUSH_GREEN_BUTTON = 8;

extern const uint8_t LED_PINS[];
extern const uint8_t LED_TOTAL;
extern uint8_t USER_INPUT[20];

class GameLevel{
    public:
        uint8_t steps[20];
        uint8_t max_difficulty = 20;
        uint8_t current_diff = 0;
        uint8_t current_checking_index = 0;

        void generateLevel(int difficulty);
        void play();
        bool checkUserInput();
        void reset();
};

#endif