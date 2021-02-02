#pragma once
#include <cstdint>
#include <memory>
#include <M5Stack.h>
#include <cmath>

#include "maze.hpp"

class Player {

  Maze::ptr_t maze_;

  // The initial position is always top left because that's how the maze is
  // initialized.
  int position_ = 0;
  int previous_position_ = 0;

  M5Display lcd_;

public:
  using ptr_t = std::shared_ptr<Player>;

  enum e_direction { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };

  Player(M5Display d, const Maze::ptr_t &maze) : maze_(maze), lcd_(d) {}

  template <e_direction d> inline bool move() {
    int new_pos = -1;
    switch (d) {
    case UP:
      new_pos = maze_->north(position_);
      break;
    case DOWN:
      new_pos = maze_->south(position_);
      break;
    case LEFT:
      new_pos = maze_->west(position_);
      break;
    case RIGHT:
      new_pos = maze_->east(position_);
      break;
    }
    if (maze_->valid(new_pos) && !maze_->is_wall(new_pos)) {
      previous_position_ = position_;
      position_ = new_pos;
      return true;
    }
    return false;
  }

  inline void clear() {
    int radius = Maze::FACTOR / 2 - 2;
    int x = maze_->xoffset() + maze_->to_x(position_) * Maze::FACTOR + radius;
    int y = maze_->yoffset() + maze_->to_y(position_) * Maze::FACTOR + radius;
    lcd_.fillCircle(x,y, radius, Maze::FLOOR);
  }

  // This function will draw the player as a circle, in contrast to the
  // rectangle, the circle uses the center as the anchorpoint, which means we
  // have to adjust the positioning accordingly.
  inline void draw(uint16_t color) {
    // Half of the pixel expansion minus two free pixels.
    int radius = Maze::FACTOR / 2 - 2;

    // Draw an animation along the two centerpoints.
    int old_x = maze_->xoffset() + maze_->to_x(previous_position_) * Maze::FACTOR + radius;
    int old_y = maze_->yoffset() + maze_->to_y(previous_position_) * Maze::FACTOR + radius;

    int x = maze_->xoffset() + maze_->to_x(position_) * Maze::FACTOR + radius;
    int y = maze_->yoffset() + maze_->to_y(position_) * Maze::FACTOR + radius;

    int dir_x = old_x < x ? 1 : -1;
    int dir_y = old_y < y ? 1 : -1;
    while (old_x != x || old_y != y) {
      lcd_.fillCircle(old_x,old_y, radius, color);
      delay(10);
      lcd_.fillCircle(old_x,old_y, radius, Maze::FLOOR);
      if (old_x != x) old_x += dir_x;
      if (old_y != y) old_y += dir_y;
    }
    lcd_.fillCircle(x,y, radius, color);
  }

  inline bool left() { return move<LEFT>(); }
  inline bool right() { return move<RIGHT>(); }
  inline bool up() { return move<UP>(); }
  inline bool down() { return move<DOWN>(); }
  inline int pos() const { return position_; }
  inline bool next(e_direction d) {
    if (d == UP) return up();
    if (d == DOWN) return down();
    if (d == LEFT) return left();
    return right();
  }
  inline bool done() { return position_ == maze_->exit(); }
};