#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include <queue>
#include <utility>
#include <set>
#include <vector>

enum Direction {
  LEFT,
  RIGHT,
  UP,
  DOWN,
  NONE // start
};

std::vector<std::string> matrix;
std::vector<std::vector<bool>> visited;
std::set<std::pair<int,int>> path;

bool opposite_direction(Direction dir1, Direction dir2) {
  return 
      dir1 == LEFT && dir2 == RIGHT ||
      dir1 == RIGHT && dir2 == LEFT ||
      dir1 == UP && dir2 == DOWN ||
      dir1 == DOWN && dir2 == UP;
}

bool valid_move(char src, char dest, Direction dir) {
  if(src == 'S' || dest == 'S')
    return true;

  switch(src) {
    case '|':
      return (dest == '|' && (dir == UP || dir == DOWN))
            || ((dest == '7' || dest == 'F') && dir == UP)
            || ((dest == 'L' || dest == 'J') && dir == DOWN);
    case '-':
      return (dest == '-' && (dir == RIGHT || dir == LEFT))
            || ((dest == 'J' || dest == '7') && dir == RIGHT)
            || ((dest == 'F' || dest == 'L') && dir == LEFT);
    case 'L':
      return (dir == UP && (dest == '|' || dest == '7' || dest == 'F'))
            || (dir == RIGHT && (dest == '7' || dest == 'J' || dest == '-'));
    case 'J':
      return (dir == LEFT && (dest == '-' || dest == 'L' || dest == 'F'))
            || (dir == UP && (dest == '|' || dest == '7' || dest == 'F'));
    case '7':
      return (dir == LEFT && (dest == '-' || dest == 'F' || dest == 'L'))
            || (dir == DOWN && (dest == '|' || dest == 'L' || dest == 'J'));
    case 'F':
      return (dir == RIGHT && (dest == '-' || dest == '7' || dest == 'J'))
            || (dir == DOWN && (dest == '|' || dest == 'J' || dest == 'L'));
  }

  return false;
}

inline bool in_bounds(int x, int y) {
  return y < matrix.size() && y >= 0 && x < matrix[0].size() && x >= 0;
}

int dfs(int x, int y, int counter, Direction last_dir) {
  if(matrix[y][x] == 'S' && last_dir != NONE) {
    std::cout << "result: " << counter << std::endl;
    return counter / 2;
  }

  path.insert(std::make_pair(x, y));

  // gore
  if(in_bounds(x, y - 1) && valid_move(matrix[y][x], matrix[y-1][x], UP) && !opposite_direction(last_dir, UP)) {
    if(dfs(x, y-1, counter+1, UP))
      return counter/2;
  }
  // dole
  if(in_bounds(x, y + 1) && valid_move(matrix[y][x], matrix[y+1][x], DOWN) && !opposite_direction(last_dir, DOWN)) {
    if(dfs(x, y+1, counter+1, DOWN))
      return counter/2;
  }
  // levo
  if(in_bounds(x - 1, y) && valid_move(matrix[y][x], matrix[y][x-1], LEFT) && !opposite_direction(last_dir, LEFT)) {
    if(dfs(x-1, y, counter+1, LEFT))
      return counter/2;
  }
  // desno
  if(in_bounds(x + 1, y) && valid_move(matrix[y][x], matrix[y][x+1], RIGHT) && !opposite_direction(last_dir, RIGHT)) {
    if(dfs(x+1, y, counter+1, RIGHT))
      return counter/2;
  }

  path.erase(path.find(std::make_pair(x, y)));
  return 0;
}

bool is_inside(int y, int x) {
  int count_intersect = 0;
  // idi skroz do levog ruba i broji intersecte
  for(int i=x;i >= 0;i--) {
    if(path.find({i, y}) != path.end()) {
      if(matrix[y][i] == '|' || matrix[y][i] == 'L' || matrix[y][i] == 'J')
        count_intersect++;
    }
  }

  return count_intersect % 2 == 1;
}

int main() {
  std::fstream file("./input.txt");

  std::string input;
  while(file) {
    getline(file, input);
    if(input.size() != 0)
      matrix.push_back(input);
  }
  file.close();

  // find starting position
  int start_x, start_y;
  for(int i=0;i < matrix.size();i++) {
    for(int j=0;j < matrix[i].size();j++) {
      if(matrix[i][j] == 'S') {
        start_y = i;
        start_x = j;
        break;
      }
    }
  }
  // initialize visited fields
  for(int i=0;i < matrix.size();i++) {
    std::vector<bool> temp;
    for(int j=0;j < matrix[j].size();j++)
      temp.push_back(false);
    visited.push_back(temp);
  }

  dfs(start_x, start_y, 0, NONE);

  int count_inside = 0;
  for(int i=0;i < matrix.size();i++) {
    for(int j=0;j < matrix[i].size();j++) {
      if(path.find({j, i}) == path.end())
        count_inside += is_inside(i, j);
    }
  }

  std::cout << "part2: " << count_inside << std::endl;
  return 0;
}
