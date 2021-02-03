#include "player.hpp"

void Player::initGyro() {
  for (int i = 0;i < AVERAGENUM_INIT;i++) {
    M5.IMU.getGyroData(&gyroX_,&gyroY_,&gyroZ_);
    init_gyroX_ += gyroX_;
    init_gyroY_ += gyroY_;
    init_gyroZ_ += gyroZ_;
    delay(5);
  }
  init_gyroX_ /= AVERAGENUM_INIT;
  init_gyroY_ /= AVERAGENUM_INIT;
  init_gyroZ_ /= AVERAGENUM_INIT;
}

void Player::setup() {
  M5.IMU.Init();
  initGyro();
  //bmm150_.Init();
}

Player::e_direction Player::update() {
  M5.IMU.getGyroData(&gyroX_, &gyroY_, &gyroZ_);
  gyroX_ -= init_gyroX_;
  gyroY_ -= init_gyroY_;
  gyroZ_ -= init_gyroZ_;
  M5.IMU.getAccelData(&accX_, &accY_, &accZ_);

  MahonyAHRSupdateIMU(gyroX_* DEG_TO_RAD, gyroY_ * DEG_TO_RAD, gyroZ_ * DEG_TO_RAD, accX_, accY_, accZ_,
      &pitch_, &roll_, &yaw_);

  //Serial.printf("PR %5.2f %5.2f\n", pitch_, roll_);
  // Pick the direction on the biggest amplitude
  e_direction dir = NONE;
  if (abs(pitch_) > abs(roll_)) {
    // pitch is left and right
    if (abs(pitch_) > 10) {
      dir = pitch_ < 0 ? LEFT : RIGHT;
    }
  } else {
    // roll is up and down
    if (abs(roll_) > 10) {
      dir = roll_ < 0 ? UP : DOWN;
    }
  }
  return dir;
}