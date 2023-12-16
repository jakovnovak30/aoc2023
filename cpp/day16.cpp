#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

std::vector<std::string> matrix;
bool **visited;

enum Direction {
  LEFT,
  RIGHT,
  UP,
  DOWN
};

inline bool in_bounds(int x, int y) {
  return x >= 0 && x < matrix[0].size() && y >= 0 && y < matrix.size();
}

std::pair<int, int> from_dir(Direction dir) {
  switch (dir) {
    case LEFT:
      return { -1, 0 };
      break;
    case RIGHT:
      return { 1, 0 };
      break;
    case UP:
      return { 0, -1 };
      break;
    case DOWN:
      return { 0, 1 };
      break;
  }

  return {};
}

void send_beam(int x, int y, Direction lastDir) {
  if(!in_bounds(x, y))
    return;

  // mark visited
  // if(visited[y][x])
  //   return;
  visited[y][x] = true;

  // change direction
  Direction nextDir = lastDir;
  if(matrix[y][x] == '.') {
    send_beam(x + from_dir(nextDir).first, y + from_dir(nextDir).second, nextDir);
  }
  else if(matrix[y][x] == '/') {
    if(nextDir == RIGHT)
      nextDir = UP;
    else if(nextDir == DOWN)
      nextDir = LEFT;
    else if(nextDir == UP)
      nextDir = RIGHT;
    else
     nextDir = DOWN;

    send_beam(x + from_dir(nextDir).first, y + from_dir(nextDir).second, nextDir);
  }
  else if(matrix[y][x] == '\\') {
    if(nextDir == RIGHT)
      nextDir = DOWN;
    else if(nextDir == DOWN)
      nextDir = RIGHT;
    else if(nextDir == LEFT)
      nextDir = UP;
    else
     nextDir = LEFT;

    send_beam(x + from_dir(nextDir).first, y + from_dir(nextDir).second, nextDir);
  }
  else if(matrix[y][x] == '-') {
    if(nextDir == LEFT || nextDir == RIGHT) {
      send_beam(x + from_dir(nextDir).first, y + from_dir(nextDir).second, nextDir);
    }
    else {
      matrix[y][x] = 'H'; // disable the split to avoid infinite loop
      send_beam(x - 1, y, LEFT);
      send_beam(x + 1, y, RIGHT);
    }
  }
  else if(matrix[y][x] == '|') {
    if(nextDir == UP || nextDir == DOWN) {
      send_beam(x + from_dir(nextDir).first, y + from_dir(nextDir).second, nextDir);
    }
    else {
      matrix[y][x] = 'V'; // disable the split to avoid infinite loop
      send_beam(x, y - 1, UP);
      send_beam(x, y + 1, DOWN);
    }
  }
  else if(matrix[y][x] == 'V') {
    if(nextDir == UP || nextDir == DOWN)
      send_beam(x + from_dir(nextDir).first, y + from_dir(nextDir).second, nextDir);
    else
      return;
  }
  else {
    if(nextDir == LEFT || nextDir == RIGHT)
      send_beam(x + from_dir(nextDir).first, y + from_dir(nextDir).second, nextDir);
    else
      return;
  }

}

void init_visited() {
  visited = new bool *[matrix.size()];
  for(int i=0;i < matrix.size();i++) {
    bool *temp = new bool[matrix[i].size()];
    for(int j=0;j < matrix[i].size();j++)
      temp[j] = false;

    visited[i] = temp;
  }
}

void reset_visited() {
  for(int i=0;i < matrix.size();i++) {
    for(int j=0;j < matrix[i].size();j++)
      visited[i][j] = false;
  }
}

void delete_visited() {
  for(int i=0;i < matrix.size();i++)
    delete visited[i];
  delete visited;
}

void reset_matrix() {
  for(int i=0;i < matrix.size();i++) {
    for(int j=0;j < matrix[i].size();j++) {
      if(matrix[i][j] == 'H')
        matrix[i][j] = '-';
      else if(matrix[i][j] == 'V')
        matrix[i][j] = '|';
    }
  }
}

int count_energized(int start_x, int start_y, Direction start_dir) {
  send_beam(start_x, start_y, start_dir);

  int counter = 0;
  for(int i=0;i < matrix.size();i++)
    for(int j=0;j < matrix[i].size();j++)
      counter += visited[i][j];

  reset_visited();
  reset_matrix();

  return counter;
}

int main() {
  std::fstream file("./input.txt");

  std::string input;
  while(file) {
    getline(file, input);

    if(input.size() == 0)
      continue;

    matrix.push_back(input);
  }
  file.close();

  // init visited
  init_visited();

  // probaj sve kombinacije, vrati maximum!
  int count_max = -1;

  // top row
  for(int k=0; k < matrix[0].size(); k++) {
    int count1 = count_energized(0, k, RIGHT);
    int count2 = count_energized(0, k, DOWN);
    int count3 = count_energized(0, k, LEFT);

    count_max = std::max(count_max, std::max(count1, std::max(count2, count3)));
  }
  // bottom row
  for(int k=0;k < matrix[0].size();k++) {
    int count1 = count_energized(matrix.size()-1, k, RIGHT);
    int count2 = count_energized(matrix.size()-1, k, UP);
    int count3 = count_energized(matrix.size()-1, k, LEFT);

    count_max = std::max(count_max, std::max(count1, std::max(count2, count3)));
  }

  // leftmost column
  for(int k=0;k < matrix.size();k++) {
    int count1 = count_energized(k, 0, RIGHT);
    int count2 = count_energized(k, 0, UP);
    int count3 = count_energized(k, 0, DOWN);

    count_max = std::max(count_max, std::max(count1, std::max(count2, count3)));
  }
  // rightmost column
  for(int k=0;k < matrix.size();k++) {
    int count1 = count_energized(k, matrix[0].size()-1, LEFT);
    int count2 = count_energized(k, matrix[0].size()-1, UP);
    int count3 = count_energized(k, matrix[0].size()-1, DOWN);

    count_max = std::max(count_max, std::max(count1, std::max(count2, count3)));

  }

  std::cout << "solution: " << count_max << std::endl;

  // delete visited
  delete_visited();
  return 0;
}
