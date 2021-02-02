#include <Arduino.h>
#include <M5Stack.h>
#include <cstdlib>
#include <memory>

#include "maze.hpp"
#include "player.hpp"

#define M5STACKFIRE_MICROPHONE_PIN 34
#define M5STACKFIRE_SPEAKER_PIN 25 // speaker DAC, only 8 Bit

#define HORIZONTAL_RESOLUTION 320
#define VERTICAL_RESOLUTION 240
#define POSITION_OFFSET_Y 20
#define SIGNAL_LENGTH HORIZONTAL_RESOLUTION

uint16_t oldSignal[SIGNAL_LENGTH];
uint16_t adcBuffer[SIGNAL_LENGTH];

#define SAMPLING_TIME_US 25
#define ANALOG_SIGNAL_INPUT M5STACKFIRE_MICROPHONE_PIN

// This is the main game context
Maze::ptr_t maze;
Player::ptr_t player;
Player::e_direction dir = Player::RIGHT;
bool done = false;

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(115200);

  dacWrite(M5STACKFIRE_SPEAKER_PIN, 0); // make sure that the speaker is quite
  M5.Lcd.begin();
  M5.Power.begin();
  M5.Lcd.fillScreen(WHITE);
  maze = std::make_shared<Maze>(M5.Lcd, 10, 10, 13, 13);
  maze->generate();
  maze->draw();

  player = std::make_shared<Player>(M5.Lcd, maze);
}

void loop(void) {
  if (!done) {
    // One step of the player
    player->clear();
    // Check "left":
    Player::e_direction left;
    if (dir == Player::RIGHT)
      left = Player::UP;
    else if (dir == Player::DOWN)
      left = Player::RIGHT;
    else if (dir == Player::LEFT)
      left = Player::DOWN;
    else
      left = Player::LEFT;

    if (player->next(left)) {
      dir = left;
    } else {
      while (!player->next(dir)) {
        if (dir == Player::RIGHT)
          dir = Player::DOWN;
        else if (dir == Player::DOWN)
          dir = Player::LEFT;
        else if (dir == Player::LEFT)
          dir = Player::UP;
        else
          dir = Player::RIGHT;
      }
    }
    done = player->done();
    if (!done) {
      player->draw(RED);
    }
  }
  delay(10);
}
