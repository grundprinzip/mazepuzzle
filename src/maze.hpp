#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <vector>

#include <Arduino.h>
#include <M5Stack.h>

class Maze {

  M5Display lcd;
  // X offset
  int x;
  // Y offset
  int y;

  int width;
  int height;

  // Start position
  int start = 0;
  // Exit position
  int exit_ = -1;

  // This is for the maze data.
  std::vector<bool> maze;

private:
  void drawCell(int i, uint16_t color);

public:
  static const uint16_t WALL = BLACK;
  static const uint16_t FLOOR = LIGHTGREY;
  static const uint16_t ENTRY = GREEN;
  static const uint16_t EXIT = RED;
  static const uint8_t FACTOR = 15;
  using ptr_t = std::shared_ptr<Maze>;

  Maze(M5Display d, int xo, int yo, int w, int h)
      : lcd(d), x(xo), y(yo), width(w), height(h) {
    // Prepare the maze.
    maze.resize(width * height, true);
  }

  inline int exit() const { return exit_; }
  inline int to_x(int cell) const { return cell % width; }
  inline int to_y(int cell) const { return cell / width; }
  inline int xoffset() const { return x; }
  inline int yoffset() const { return y; }

  inline bool valid(int cell) const {
    return cell > 0 && cell < (width * height);
  }

  inline bool same_row(int cell1, int cell2) const {
    return cell1 / width == cell2 / width;
  }

  inline bool is_wall(int cell) const { return valid(cell) && maze[cell]; }

  inline bool is_floor(int cell) const { return !is_wall(cell); }

  inline int north(int cell) const {
    auto tmp = cell - width;
    return valid(tmp) ? tmp : -1;
  }
  inline int south(int cell) const {
    auto tmp = cell + width;
    return valid(tmp) ? tmp : -1;
  }
  inline int east(int cell) const {
    return valid(cell + 1) && same_row(cell + 1, cell) ? cell + 1 : -1;
  }
  inline int west(int cell) const {
    return valid(cell - 1) && same_row(cell - 1, cell) ? cell - 1 : -1;
  }

  inline std::vector<int> nbs(int cell) const {
    std::vector<int> result;
    int nb_n, nb_s, nb_e, nb_w;
    nb_n = cell - width;
    nb_s = cell + width;
    nb_e = cell - 1;
    nb_w = cell + 1;

    if (valid(nb_n) && maze[nb_n])
      result.push_back(nb_n);
    if (valid(nb_s) && maze[nb_s])
      result.push_back(nb_s);
    if (same_row(nb_e, cell) && maze[nb_e])
      result.push_back(nb_e);
    if (same_row(nb_w, cell) && maze[nb_w])
      result.push_back(nb_w);
    return result;
  }

  /**
   */
  inline void generate() {
    maze[0] = false;

    std::vector<int> walls;
    walls.push_back(1);
    walls.push_back(width);

    while (!walls.empty()) {
      auto cell_idx = random(walls.size());
      const auto cell = walls[cell_idx];
      walls.erase(walls.begin() + cell_idx);

      int nb_n, nb_s, nb_e, nb_w;
      nb_n = cell - width;
      nb_s = cell + width;
      nb_w = cell - 1;
      nb_e = cell + 1;
      if (valid(nb_n) && valid(nb_s) && is_floor(nb_n) ^ is_floor(nb_s)) {
        maze[cell] = false;

        if (is_floor(nb_n)) {
          maze[nb_s] = false;
          const auto &nn = nbs(nb_s);
          walls.insert(walls.end(), nn.begin(), nn.end());
        } else {
          maze[nb_n] = false;
          const auto &nn = nbs(nb_n);
          walls.insert(walls.end(), nn.begin(), nn.end());
        }
      } else if (same_row(nb_e, cell) && same_row(nb_w, cell) &&
                 is_floor(nb_e) ^ is_floor(nb_w)) {
        maze[cell] = false;
        if (is_floor(nb_e)) {
          maze[nb_w] = false;
          const auto &nn = nbs(nb_w);
          walls.insert(walls.end(), nn.begin(), nn.end());
        } else {
          maze[nb_e] = false;
          const auto &nn = nbs(nb_e);
          walls.insert(walls.end(), nn.begin(), nn.end());
        }
      }
    }
  }

  // Every bit of the maze vector is translated into pixels according to the
  // factor. A factor of two draws for every maze pixel a 2x2 block on the
  // screen.
  void draw();
};