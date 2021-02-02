#include "maze.hpp"

void Maze::drawCell(int i, uint16_t color) {
  // Row col pos
    int row = i / width;
    int col = i % width;

    for (int fy = 0; fy < FACTOR; ++fy) {
      for (int fx = 0; fx < FACTOR; ++fx) {

        int adj_col = col * FACTOR + fx;
        int adj_row = row * FACTOR + fy;
        if (color != WALL) lcd.drawPixel(x + adj_col, y + adj_row, color);
      }
    }
}

void Maze::draw() {

  // Draw a rectangle of all WALL
  lcd.fillRect(x,y, width * FACTOR, height * FACTOR, WALL);

  for (int i = 0; i < maze.size(); ++i) {

    uint16_t color = maze[i] ? WALL : FLOOR;
    // We always start from the top left
    if (i == 0) {
      color = ENTRY;
    }
    drawCell(i, color);
  }

  // Find exactly one bit in the end to draw the exit
  for (int i=maze.size() - 1; i > 0; i -= width) {
    // If it's not a wall draw an exit.
    if (!maze[i]) {
      exit_ = i;
      drawCell(i, EXIT);
      break;
    }
  }

}