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


#define FSS18 &FreeSans18pt7b
#define FF19 &FreeSans18pt7b

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

  player = std::make_shared<Player>(M5.Lcd, maze);
  player->setup();

  maze->draw();
}

void loop(void) {
  if (!done) {

    auto dir = player->update();
    if (dir != Player::NONE) {
      //player->clear();
      if (player->next(dir)) {
        done = player->done();
        player->draw(RED);
      }
    }
  } else {
    M5.Lcd.setFreeFont(FF19);
    M5.Lcd.setTextColor(RED);
    M5.Lcd.drawString("Win!", 220, 100, 1);
    delay(5000);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.drawString("Win!", 220, 100, 1);
    maze->generate();
    maze->draw();
    player->reset();
    dir = Player::NONE;
    done = false;
  }
  delay(10);
}
